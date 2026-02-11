#include "Quartz_Transmittance.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

// ------------------------------------------------------------
// Linear interpolation + extrapolation
// ------------------------------------------------------------
static G4double InterpolateT(
    G4double E,
    const std::vector<G4double>& Ec,
    const std::vector<G4double>& Tc)
{
    // Deep UV extrapolation
    if (E < Ec.front()) {
        G4double x1 = Ec[0], x2 = Ec[1];
        G4double y1 = Tc[0], y2 = Tc[1];
        G4double T = y1 + (E - x1) * (y2 - y1) / (x2 - x1);
        return std::max(0.0, T);
    }

    // IR extrapolation
    if (E > Ec.back()) {
        size_t n = Ec.size();
        G4double x1 = Ec[n-2], x2 = Ec[n-1];
        G4double y1 = Tc[n-2], y2 = Tc[n-1];
        G4double T = y2 + (E - x2) * (y2 - y1) / (x2 - x1);
        return std::max(0.0, T);
    }

    // Interpolation inside range
    for (size_t i = 0; i < Ec.size() - 1; ++i) {
        if (E >= Ec[i] && E <= Ec[i+1]) {
            G4double w = (E - Ec[i]) / (Ec[i+1] - Ec[i]);
            return Tc[i] + w * (Tc[i+1] - Tc[i]);
        }
    }

    return 0.0; // safety
}

// ------------------------------------------------------------
// Public interface
// ------------------------------------------------------------
G4MaterialPropertiesTable*
LoadQuartzTransmittanceCSV(
    const std::string& filename,
    G4double thickness_cm,
    G4double lambdaMin_nm,
    G4double lambdaMax_nm,
    G4int nPoints)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        G4cerr << "ERROR: Cannot open " << filename << G4endl;
        return nullptr;
    }

    std::vector<G4double> lambda_nm, T;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        G4double l, t;
        char comma;
        ss >> l >> comma >> t;
        lambda_nm.push_back(l);
        T.push_back(t);
    }

    if (lambda_nm.size() < 2) {
        G4cerr << "ERROR: Not enough points in " << filename << G4endl;
        return nullptr;
    }

    // Convert wavelength to energy
    std::vector<G4double> Ec(lambda_nm.size());
    for (size_t i=0;i<lambda_nm.size();++i)
        Ec[i] = (1240.0 / lambda_nm[i]) * eV;

    // Reverse CSV order if needed (lambda increasing -> E decreasing)
    std::reverse(Ec.begin(), Ec.end());
    std::reverse(T.begin(), T.end());

    // Build energy grid
    std::vector<G4double> energyGrid(nPoints);
    std::vector<G4double> absGrid(nPoints);
    G4double dLambda = (lambdaMax_nm - lambdaMin_nm) / (nPoints - 1);
    for (int i=0;i<nPoints;i++) {
        G4double lambda = lambdaMin_nm + i*dLambda;
        energyGrid[nPoints-1-i] = (1240.0 / lambda) * eV;
        G4double Ti_percent = InterpolateT(energyGrid[nPoints-1-i], Ec, T);
        
        // FIX: Convert percent to fraction
        G4double Ti = Ti_percent / 100.0;
        
        // Physical Correction: JGS2 is opaque below ~200nm
        if (lambda >= 200.0) {
            //Ti = Ti_percent / 100.0; // Effectively 0% transmittance
            Ti = 0.73;
        }

        if (lambda < 200.0) Ti = 0.73; //sharp cut off (a characteristic of jgs2)
        if (Ti <= 0.0) Ti = 1e-9;
        if (Ti >= 1.0) Ti = 0.999999;
        absGrid[nPoints-1-i] = (-thickness_cm * cm) / std::log(Ti);
    }

    auto* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("ABSLENGTH", energyGrid.data(), absGrid.data(), nPoints);

    return mpt;
}

