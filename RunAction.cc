#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
    : reflectedPhotons(0), tirPhotons(0), totalGeneratedPhotons(0), totalDetectedPhotons(0), eventID(0) {
    
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile("output_4.root");

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
    analysisManager->CreateH1("hGeneratedPhotons", "Photons Generated per Event", 700, 0, 700); //id 0
    analysisManager->CreateH1("hDetectedPhotons", "Photons Detected per Event", 700, 0, 700); //id 1
    analysisManager->CreateH1("MuonEnergyWithGeneratedCherenkov", "Muon energy (events with Cherenkov photons)", 300, 0, 3); //id 2
    analysisManager->CreateH1("MuonEnergyWithDetectedCherenkov", "Muon Energy with >=1 Detected Cherenkov photons", 100, 0, 5); //id 3

}

void RunAction::BeginOfRunAction(const G4Run*) {
    ConstructHistograms();
    
    auto man = G4AnalysisManager::Instance();
    man->CreateNtuple("CherenkovStats", "Photon Yield Stats");
//    man->CreateNtupleIColumn("nCherenkov");        // Total Cherenkov photons
//    man->CreateNtupleDColumn("muonLength");        // Muon track length (mm)
    man->CreateNtupleDColumn("yieldPerMM");        // Cherenkov/mm
    man->FinishNtuple();
    //auto analysisManager = G4AnalysisManager::Instance();
    man->CreateH1("Yield", "Cherenkov Photon Yield per Event;Photons/mm;# Events", 100, 0, 10); 



    reflectedPhotons = 0;
    tirPhotons = 0;
    totalGeneratedPhotons = 0;
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
void RunAction::IncrementDetectedPhotons()  { totalDetectedPhotons++; }
