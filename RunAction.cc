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
    analysisManager->OpenFile("beta_check_9.root");

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
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_1PE", "Muon Energy with >=1 Detected Cherenkov photons", 1500, 0, 15); //id 13
    
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
    analysisManager->CreateH1("primPhotonVsEnergy", "Muon Energy when primary photons generated", 1500, 0, 15); //20
    analysisManager->CreateH1("secPhotonVsEnergy",  "Muon Energy when secondary photons generated", 1500, 0, 15); //21
    
    //;;;;;;;;;;;;;;;----
    
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_2PE", "Muon Energy with >=2 Detected Cherenkov photons", 1500, 0, 15); //id 22
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_3PE", "Muon Energy with >=3 Detected Cherenkov photons", 1500, 0, 15); //id 23
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_3.5PE", "Muon Energy with >=3.5 Detected Cherenkov photons", 1500, 0, 15); //id 24
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_4PE", "Muon Energy with >=4 Detected Cherenkov photons", 1500, 0, 15); //id 25
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold_4.5PE", "Muon Energy with >=4.5 Detected Cherenkov photons", 1500, 0, 15); //id 26
    
    
    //;;;;;;;;;;;;;;;'''
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_2PE", "Muon energy (events with >=2 Cherenkov photons)", 1500, 0, 15); //id 27
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_3PE", "Muon energy (events with >=3 Cherenkov photons)", 1500, 0, 15); //id 28
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_4PE", "Muon energy (events with >=4 Cherenkov photons)", 1500, 0, 15); //id 29
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold_4.5PE", "Muon energy (events with >=4.5 Cherenkov photons)", 1500, 0, 15); //id 30
    
    
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

void RunAction::BeginOfRunAction(const G4Run*) {
    ConstructHistograms();
    
    reflectedPhotons = 0;
    tirPhotons = 0;
    totalGeneratedPhotons = 0;
    totalPhotonsAtPMT = 0;
    totalDetectedPhotons = 0;
    eventID = 0;
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    
    G4double amplificationFactor = (totalGeneratedPhotons > 0)
        ? static_cast<G4double>(totalDetectedPhotons) / totalGeneratedPhotons : 0.0;

    G4cout << "\n[Summary]\nGenerated: " << totalGeneratedPhotons
           << ", Detected: " << totalDetectedPhotons
           << ", Reflections: " << reflectedPhotons
           << ", TIR: " << tirPhotons
           << ", Amplification Factor: " << amplificationFactor
           << "\n" << G4endl;

    analysisManager->Write();
    analysisManager->CloseFile(false);
}

// Counters
void RunAction::IncrementReflectedPhotons() { reflectedPhotons++; }
void RunAction::IncrementTIRPhotons()       { tirPhotons++; }
void RunAction::IncrementGeneratedPhotons() { totalGeneratedPhotons++; }
void RunAction::IncrementPhotonsAtPMT() { totalPhotonsAtPMT++; }
void RunAction::IncrementDetectedPhotons()  { totalDetectedPhotons++; }
