#include "SensitiveDetector.hh"
#include "G4OpticalPhoton.hh"
#include "G4Step.hh"
#include "RunAction.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4Track* track = aStep->GetTrack();

    // Only consider optical photons
    if (track->GetDefinition() != G4OpticalPhoton::Definition())
        return false;

    // Optional: restrict to PMT volume name if needed
    // if (track->GetVolume()->GetName() != "PMT") return false;

    G4ThreeVector position = track->GetPosition();
    G4double energy = track->GetTotalEnergy();
    G4double time = track->GetLocalTime();
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4cout << "[PMT Hit] Photon at " << position
           << " | E: " << energy / eV << " eV | t: " << time / ns << " ns" << G4endl;

    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, position.x() / mm);
    analysisManager->FillNtupleDColumn(1, position.y() / mm);
    analysisManager->FillNtupleDColumn(2, energy / eV);
    analysisManager->FillNtupleDColumn(3, time / ns);
    analysisManager->FillNtupleDColumn(4, edep / eV);
    analysisManager->AddNtupleRow();

    if (edep > 0) {
        G4cout << "[DEBUG] Energy Deposited: " << edep / eV << " eV at " << position << G4endl;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
/*
G4String sdName = GetName();

    auto* analysisManager = G4AnalysisManager::Instance();

    if (sdName == "PMT_SD") {
        // Only record optical photons
        if (track->GetDefinition() != G4OpticalPhoton::Definition())
            return false;

        G4ThreeVector position = track->GetPosition();
        G4double energy = track->GetTotalEnergy();
        G4double time = track->GetLocalTime();

        analysisManager->FillNtupleDColumn(0, position.x() / mm);
        analysisManager->FillNtupleDColumn(1, position.y() / mm);
        analysisManager->FillNtupleDColumn(2, energy / eV);
        analysisManager->FillNtupleDColumn(3, time / ns);
        analysisManager->AddNtupleRow();
    }
    else if (sdName == "Scint_SD") {
        // Record energy deposit for charged particles
        G4double edep = aStep->GetTotalEnergyDeposit();
        if (edep <= 0) return false;

        G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
        G4double time = track->GetGlobalTime();

        analysisManager->FillNtupleDColumn(0, position.x() / mm);
        analysisManager->FillNtupleDColumn(1, position.y() / mm);
        analysisManager->FillNtupleDColumn(2, edep / MeV);
        analysisManager->FillNtupleDColumn(3, time / ns);
        analysisManager->AddNtupleRow();
    }
*/

