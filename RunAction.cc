#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

#include "PrimaryGeneratorAction.hh"

RunAction::RunAction()
    : reflectedPhotons(0), tirPhotons(0), totalGeneratedPhotons(0), totalPhotonsAtPMT(0), totalDetectedPhotons(0), eventID(0) {
    
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    //analysisManager->OpenFile("Recalc_LCE_16atm.root");
    analysisManager->OpenFile("2D_EandPhotDist_16atm.root");

    // Photon hit details
    analysisManager->CreateNtuple("Hits", "Photon Hits");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("energy");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->FinishNtuple();

}

RunAction::~RunAction() {
    delete G4AnalysisManager::Instance();
}

void RunAction::ConstructHistograms() {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateH1("hGeneratedPhotons", "Photons Generated per Event", 2000, 0, 2000); //id 0  
    analysisManager->CreateH1("hPMTPhotons", "Photons Reaching PMT per Event", 1000, 0, 1000); //id 1
    analysisManager->CreateH1("hDetectedPhotons", "Photons Detected per Event", 1000, 0, 1000); //id 2
    
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov", "Muon energy (events with Cherenkov photons)", 1500, 0, 15); //id 3
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov", "Muon Energy with >=1 Detected Cherenkov photons", 1500, 0, 15); //id 4
    
    analysisManager->CreateH1("hGeneratedWavelength", "Wavelength of Generated Photons;Wavelength (nm);Count", 800, 100, 900); //id 5
    analysisManager->CreateH1("hAtPMTWavelength", "Wavelength of Photons at PMT;Wavelength (nm);Count", 800, 100, 900); //id 6
    analysisManager->CreateH1("hDetectedWavelength", "Wavelength of Detected Photons;Wavelength (nm);Count", 800, 100, 900); //id 7
    
    analysisManager->CreateH1("hYield", "Cherenkov Yield;Photons/mm;Events", 1000, 0, 100); //id 8
    
    analysisManager->CreateH2(
    "Energy_vs_Yield",
    "Muon Energy vs Photon Yield;Muon Energy (GeV);Yield (photons/mm)", 
    1500, 0, 15, 
    100, 0.0, 100.0
    ); //id 0(h2)
    
    //------------histograms for 4-fold filtered events------------------------------------------------
        analysisManager->CreateH1("hGeneratedPhotons_4-fold", "Photons Generated per Event", 2000, 0, 2000); //id 9  
    analysisManager->CreateH1("hPMTPhotons_4-fold", "Photons Reaching PMT per Event", 1000, 0, 1000); //id 10
    analysisManager->CreateH1("hDetectedPhotons_4-fold", "Photons Detected per Event", 1000, 0, 1000); //id 11
    
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold", "Muon energy (events with Cherenkov photons)", 1500, 0, 15); //id 12
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_1PE", "Muon Energy with >=1 Detected Cherenkov photons", 10000, 0, 100); //id 13**************
    
    analysisManager->CreateH1("hGeneratedWavelength_4-fold", "Wavelength of Generated Photons;Wavelength (nm);Count", 800, 100, 900); //id 14
    analysisManager->CreateH1("hAtPMTWavelength_4-fold", "Wavelength of Photons at PMT;Wavelength (nm);Count", 800, 100, 900); //id 15
    analysisManager->CreateH1("hDetectedWavelength_4-fold", "Wavelength of Detected Photons;Wavelength (nm);Count", 800, 100, 900); //id 16
    
    analysisManager->CreateH1("hYield_4-fold", "Cherenkov Yield;Photons/mm;Events", 100, 0, 10); //id 17
    
    analysisManager->CreateH2(
    "Energy_vs_Yield_4-fold",
    "Muon Energy vs Photon Yield;Muon Energy (GeV);Yield (photons/mm)", 
    300, 0, 3.5, 
    100, 0.0, 10.0
    ); //id 1(h2)
    //------------------------------------------------------------------------------------------------------

    //~~~~~~~~~~~~~~~~~~~~checking theta, phi~~~~~~~~~~~~~~~~~~~~~~~~~~~
    analysisManager->CreateH1("theta_mom", "Muon theta (momentum)", 180, 0., 180.); //id 18
    analysisManager->CreateH1("phi_mom",   "Muon phi (momentum)",   360, -180., 180.); //id 19
    
    //NEW (check for below threshold non-zero entries)
    analysisManager->CreateH1("primPhotonfromMuonVsEnergy", "Muon Energy when primary photons generated", 1500, 0, 15); //20
    analysisManager->CreateH1("secPhotonfromElectronVsEnergy",  "Muon Energy when secondary photons generated", 1500, 0, 15); //21
    
    //;;;;;;;;;;;;;;;----
    
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_2PE", "Muon Energy with >=2 Detected Cherenkov photons", 10000, 0, 100); //id 22
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_3PE", "Muon Energy with >=3 Detected Cherenkov photons", 10000, 0, 100); //id 23
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_3.5PE", "Muon Energy with >=3.5 Detected Cherenkov photons", 10000, 0, 100); //id 24
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_4PE", "Muon Energy with >=4 Detected Cherenkov photons", 10000, 0, 100); //id 25
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_4.5PE", "Muon Energy with >=4.5 Detected Cherenkov photons", 10000, 0, 100); //id 26
    
    
    //;;;;;;;;;;;;;;;'''
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_2PE", "Muon energy (events with >=2 Cherenkov photons)", 10000, 0, 100); //id 27
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_3PE", "Muon energy (events with >=3 Cherenkov photons)", 10000, 0, 100); //id 28
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_4PE", "Muon energy (events with >=4 Cherenkov photons)", 10000, 0, 100); //id 29
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_4.5PE", "Muon energy (events with >=4.5 Cherenkov photons)", 10000, 0, 100); //id 30
    
    analysisManager->CreateH1("MuonEnergyforAll_4-fold", "Muon energy (events with 4-fold coincidence)", 10000, 0, 100); //id 31
    
    
    analysisManager->CreateH1("MuonKE","Muon kinetic energy",10000,0,100); // id 32
    
    analysisManager->CreateH1("PrimPhotons_4-fold","Muon energy (events with cherenkov photon from a muon parent",1500,0,15); // id 33
    analysisManager->CreateH1("SecPhotons_4-fold","Muon energy (events with cherenkov photon from a non-muon parent",1500,0,15); // id 34
    
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&quartz to air gap&&&&&&&&&&&
    analysisManager->CreateH1(
    "hAtQuartzBottomWavelength",
    "Photons striking quartz bottom;Wavelength (nm)",
    800, 100, 900); //id 35
    analysisManager->CreateH1(
    "hAtQuartzBottomWavelength_4-fold",
    "Photons striking quartz bottom;Wavelength (nm)",
    800, 100, 900); //id 36
    
    analysisManager->CreateH1(
    "hLossFraction",
    "Photon loss fraction at quartz-air gap per event;Loss fraction;Events",
    1000, 0.0, 1.0); //id 37
    
    analysisManager->CreateH2(
    "Theta_vs_LossFraction",
    "Theta vs Photon Loss Fraction;#muon_theta (deg);Loss Fraction",
    180, 0.0, 180.0,     // theta
    1000, 0.0, 1.0 );      // loss fraction);
    
    analysisManager->CreateH1(
    "hPhotonIncidenceAngle",
    "Photon Incidence Angle at Quartz-Air;#theta_{i} (deg);Counts",
    180, 0.0, 180.0); //check what exactly it gives

    //&&&&&&&check if angular dep of loss function is significant
    
    fLossVsThetaH2 =
    analysisManager->CreateH2(
        "LossVsTheta",
        "Photon loss fraction vs photon incidence angle;"
        "Photon incidence angle (deg);Loss fraction",
        90, 0., 90.,        // X: angle
        1000, 0., 1.0        // Y: loss fraction
    );
    
    
    analysisManager->CreateH1("theta_mom_4-fold", "Muon theta (momentum)", 180, 0., 180.); 
    analysisManager->CreateH1("phi_mom_4-fold",   "Muon phi (momentum)",   360, -180., 180.);
    
    //2D plot for phot-dist and parent muon energy//
    analysisManager->CreateH2(
    "Energy_vs_DetectedPhotons",
    "Muon Energy vs Detected Cherenkov Photons;"
    "Muon Energy (GeV);Detected Photons per Event",
    20000, 0., 200.,     // X: muon energy
    1000, 0., 1000.    // Y: detected photons
    );
    
    analysisManager->CreateH2(
    "Energy_vs_GeneratedPhotons",
    "Muon Energy vs Generated Cherenkov Photons;"
    "Muon Energy (GeV);Generated Photons per Event",
    20000, 0., 200.,
    5000, 0., 5000.
    );
    
/*    analysisManager->CreateH2(
    "Energy_vs_PMTPhotons",
    "Muon Energy vs Photons Reaching PMT;"
    "Muon Energy (GeV);Photons at PMT per Event",
    20000, 0., 200.,
    5000, 0., 5000.
    );
    
    analysisManager->CreateH2(
    "Energy_vs_QuartzBottomPhotons_4-fold",
    "Muon Energy vs Photons Striking Quartz Bottom;"
    "Muon Energy (GeV);Photons at Quartz Bottom per Event",
    20000, 0., 200.,
    5000, 0., 5000.
     );
*/

    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    
/*    analysisManager->CreateH1("hDetectedPhotons_2PE", "Photons Detected per Event", 1000, 0, 1000); //id 27
    analysisManager->CreateH1("hDetectedPhotons_3PE", "Photons Detected per Event", 1000, 0, 1000); //id 28
    analysisManager->CreateH1("hDetectedPhotons_3.5PE", "Photons Detected per Event", 1000, 0, 1000); //id 29
    analysisManager->CreateH1("hDetectedPhotons_4PE", "Photons Detected per Event", 1000, 0, 1000); //id 30
    analysisManager->CreateH1("hDetectedPhotons_4.5PE", "Photons Detected per Event", 1000, 0, 1000); //id 31
    
    
    analysisManager->CreateH1("hDetectedPhotons_4-fold_2PE", "Photons Detected per Event", 1000, 0, 1000); //id 32
    analysisManager->CreateH1("hDetectedPhotons_4-fold_3PE", "Photons Detected per Event", 1000, 0, 1000); //id 33
    analysisManager->CreateH1("hDetectedPhotons_4-fold_3.5PE", "Photons Detected per Event", 1000, 0, 1000); //id 34
    analysisManager->CreateH1("hDetectedPhotons_4-fold_4PE", "Photons Detected per Event", 1000, 0, 1000); //id 35
    analysisManager->CreateH1("hDetectedPhotons_4-fold_4.5PE", "Photons Detected per Event", 1000, 0, 1000); //id 36 */

}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
void RunAction::IncrementPhotonsAtQuartzBottom()
{
    totalPhotonsAtQuartzBottom++;
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void RunAction::BeginOfRunAction(const G4Run*) {
    ConstructHistograms();
    
    reflectedPhotons = 0;
    tirPhotons = 0;
    totalGeneratedPhotons = 0;
    totalPhotonsAtPMT = 0;
    totalDetectedPhotons = 0;
    totalPhotonsAtQuartzBottom = 0;
    eventID = 0;
    
    incidentPerAngle.assign(nBins, 0);//&&&&&&&
    lostPerAngle.assign(nBins, 0);//&&&&&&&&&&&

}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    
    G4double amplificationFactor = (totalGeneratedPhotons > 0)
        ? static_cast<G4double>(totalDetectedPhotons) / totalGeneratedPhotons : 0.0;

/*    G4cout << "\n[Summary]\nGenerated: " << totalGeneratedPhotons
           << ", Detected: " << totalDetectedPhotons
           << ", Reflections: " << reflectedPhotons
           << ", TIR: " << tirPhotons
           << ", Amplification Factor: " << amplificationFactor
           << "\n" << G4endl;
*/
//for the angular dependence check on loss function&&&&&&&&&&&&

 for (int i = 0; i < nBins; i++) {
    if (incidentPerAngle[i] > 0) {
        G4double lossFrac =
            (G4double) lostPerAngle[i] / incidentPerAngle[i];

        analysisManager->FillH2(
            fLossVsThetaH2,
            i + 0.5,
            lossFrac
        );
    }
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    analysisManager->Write();
    analysisManager->CloseFile(false);
}

void RunAction::IncrementIncidentAtAngle(G4int bin)
{
    if (bin >= 0 && bin < nBins)
        incidentPerAngle[bin]++;
}

void RunAction::IncrementLostAtAngle(G4int bin)
{
    if (bin >= 0 && bin < nBins)
        lostPerAngle[bin]++;
}

// Counters
void RunAction::IncrementReflectedPhotons() { reflectedPhotons++; }
void RunAction::IncrementTIRPhotons()       { tirPhotons++; }
void RunAction::IncrementGeneratedPhotons() { totalGeneratedPhotons++; }
void RunAction::IncrementPhotonsAtPMT() { totalPhotonsAtPMT++; }
void RunAction::IncrementDetectedPhotons()  { totalDetectedPhotons++; }
