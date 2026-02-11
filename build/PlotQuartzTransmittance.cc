#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TPad.h"

int main() {
    std::string filename = "Quartz_jgs2_Dataset.csv";
    double thickness_cm = 2.0; 
    bool correctForReflection = true; // Set to true to separate Bulk Abs from Surface Reflection

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        return 1;
    }

    std::vector<double> lambda_nm, T_percent;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        double l, t;
        char comma;
        if (ss >> l >> comma >> t) {
            lambda_nm.push_back(l);
            T_percent.push_back(t);
        }
    }

    auto InterpolateT = [&](double lambda) {
        if (lambda <= lambda_nm.front()) return T_percent.front();
        if (lambda >= lambda_nm.back()) return T_percent.back();
        for (size_t i = 0; i < lambda_nm.size() - 1; ++i) {
            if (lambda >= lambda_nm[i] && lambda <= lambda_nm[i+1]) {
                double w = (lambda - lambda_nm[i]) / (lambda_nm[i+1] - lambda_nm[i]);
                return T_percent[i] + w * (T_percent[i+1] - T_percent[i]);
            }
        }
        return 0.0;
    };

    int nPoints = 500;
    std::vector<double> plotLambda(nPoints), plotT(nPoints), plotAbsLength(nPoints);
    double lMin = 180.0, lMax = 1400.0; 
    double dL = (lMax - lMin) / (nPoints - 1);

    for (int i = 0; i < nPoints; ++i) {
        double lambda = lMin + i * dL;
        plotLambda[i] = lambda;

        double Ti_raw = InterpolateT(lambda) / 100.0;
        
        // --- Reflection Correction Logic ---
        // Quartz n~1.46 -> R ~ 0.035. Total T_max is ~0.93.
        // If we don't correct, a T of 0.90 looks like it has absorption, 
        // when in reality it's almost perfectly transparent bulk-wise.
        double R = 0.035; 
        double T_internal = Ti_raw / ((1-R)*(1-R)); 
        
        if (lambda < 200.0) T_internal = 1e-9;
        T_internal = std::clamp(T_internal, 1e-9, 0.999999);

        plotT[i] = T_internal;
        plotAbsLength[i] = -thickness_cm / std::log(T_internal);
    }

    // --- Plotting ---
    TCanvas* c1 = new TCanvas("c1", "Quartz Optical Properties", 1200, 500);
    c1->Divide(2, 1);

    // Pad 1: Transmittance (Linear)
    c1->cd(1);
    gPad->SetGrid();
    TGraph* gT = new TGraph(nPoints, plotLambda.data(), plotT.data());
    gT->SetTitle("Internal Transmittance (Corrected for Reflection);Wavelength (nm);T (fraction)");
    gT->SetLineColor(kRed+1);
    gT->SetLineWidth(2);
    gT->GetXaxis()->SetRangeUser(100, 900);
    gT->Draw("AL");

    // Pad 2: Absorption Length (Log)
    c1->cd(2);
    gPad->SetGrid();
    gPad->SetLogy();
    TGraph* gAbs = new TGraph(nPoints, plotLambda.data(), plotAbsLength.data());
    gAbs->SetTitle("Bulk Absorption Length;Wavelength (nm);Abs Length (cm)");
    gAbs->SetLineColor(kBlue+1);
    gAbs->SetLineWidth(2);
    gAbs->GetXaxis()->SetRangeUser(100, 900);
    gAbs->Draw("AL");

    c1->SaveAs("Quartz_Optical_Analysis.png");
    return 0;
}
