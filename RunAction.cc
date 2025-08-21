#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
    : reflectedPhotons(0), tirPhotons(0), totalGeneratedPhotons(0), totalPhotonsAtPMT(0), totalDetectedPhotons(0), eventID(0) {
    
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile("output_withCone_4.root");

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
    
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov", "Muon energy (events with Cherenkov photons)", 300, 0, 3.5); //id 3
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov", "Muon Energy with >=1 Detected Cherenkov photons", 300, 0, 3.5); //id 4
    
    analysisManager->CreateH1("hGeneratedWavelength", "Wavelength of Generated Photons;Wavelength (nm);Count", 800, 100, 900); //id 5
    analysisManager->CreateH1("hAtPMTWavelength", "Wavelength of Photons at PMT;Wavelength (nm);Count", 800, 100, 900); //id 6
    analysisManager->CreateH1("hDetectedWavelength", "Wavelength of Detected Photons;Wavelength (nm);Count", 800, 100, 900); //id 7
    
    analysisManager->CreateH1("hYield", "Cherenkov Yield;Photons/mm;Events", 1000, 0, 100); //id 8
    
    analysisManager->CreateH2(
    "Energy_vs_Yield",
    "Muon Energy vs Photon Yield;Muon Energy (GeV);Yield (photons/mm)", 
    300, 0, 3.5, 
    100, 0.0, 100.0
    ); //id 9
    
    //------------histograms for 4-fold filtered events------------------------------------------------
        analysisManager->CreateH1("hGeneratedPhotons_4-fold", "Photons Generated per Event", 2000, 0, 2000); //id 10  
    analysisManager->CreateH1("hPMTPhotons_4-fold", "Photons Reaching PMT per Event", 1000, 0, 1000); //id 11
    analysisManager->CreateH1("hDetectedPhotons_4-fold", "Photons Detected per Event", 1000, 0, 1000); //id 12
    
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov_4-fold", "Muon energy (events with Cherenkov photons)", 300, 0, 3.5); //id 13
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov_4-fold", "Muon Energy with >=1 Detected Cherenkov photons", 300, 0, 3.5); //id 14
    
    analysisManager->CreateH1("hGeneratedWavelength_4-fold", "Wavelength of Generated Photons;Wavelength (nm);Count", 800, 100, 900); //id 15
    analysisManager->CreateH1("hAtPMTWavelength_4-fold", "Wavelength of Photons at PMT;Wavelength (nm);Count", 800, 100, 900); //id 16
    analysisManager->CreateH1("hDetectedWavelength_4-fold", "Wavelength of Detected Photons;Wavelength (nm);Count", 800, 100, 900); //id 17
    
    analysisManager->CreateH1("hYield_4-fold", "Cherenkov Yield;Photons/mm;Events", 100, 0, 10); //id 18
    
    analysisManager->CreateH2(
    "Energy_vs_Yield_4-fold",
    "Muon Energy vs Photon Yield;Muon Energy (GeV);Yield (photons/mm)", 
    300, 0, 3.5, 
    100, 0.0, 10.0
    ); //id 19
    //------------------------------------------------------------------------------------------------------

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
