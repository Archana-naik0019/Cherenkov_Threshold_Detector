#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"


EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    ResetEventCounters();
    
    ResetBookletHits(); //4-fold
    //fBookletHits = {false, false, false, false}; //redundant

}

void EventAction::EndOfEventAction(const G4Event*event) {
    auto* analysisManager = G4AnalysisManager::Instance();
    auto* runAction = const_cast<RunAction*>(
        static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));

    analysisManager->FillH1(0, eventGeneratedPhotons);
    analysisManager->FillH1(1, eventPhotonsAtPMT);  
    analysisManager->FillH1(2, eventDetectedPhotons);
    
   auto* runManager = G4RunManager::GetRunManager();
   auto* generatorAction = static_cast<const PrimaryGeneratorAction*>(runManager->GetUserPrimaryGeneratorAction());
   
   G4double muonEnergy = generatorAction->GetMuonEnergy();

   if (eventGeneratedPhotons > 0) {
       auto* analysisManager = G4AnalysisManager::Instance();
       analysisManager->FillH1(3, muonEnergy);
}
       
   if (eventDetectedPhotons >= 1) { //finding the correct number from slides
       auto* analysisManager = G4AnalysisManager::Instance();
       analysisManager->FillH1(4, muonEnergy);
}

    if (fMuonTrackLength > 0) {
        auto* analysisManager = G4AnalysisManager::Instance();
        G4double yield = fGeneratedCherenkovPhotons / (fMuonTrackLength);  // photons/mm
        analysisManager->FillH1(8, yield);
        
        analysisManager->FillH2(0, muonEnergy, yield); //for photon yield per mm
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    //auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(18, fTheta/deg);
    analysisManager->FillH1(19, fPhi/deg);

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // NEW histograms
    if (eventPrimGeneratedPhotons > 0) {
        analysisManager->FillH1(20, muonEnergy);   // parentID == 1
    }

    if (eventSecGeneratedPhotons > 0) {
        analysisManager->FillH1(21, muonEnergy);   // parentID != 1
    }
    
    //-----------------------------for 4-fold------------------------------------------
    if (AllBookletsHit()) {
    G4cout << "Muon passed through all 4 booklets this event" << G4endl;
    ///............storing stats for muons that satisffy 4-fold criteria...///
        analysisManager->FillH1(9, eventGeneratedPhotons);
        analysisManager->FillH1(10, eventPhotonsAtPMT);  
        analysisManager->FillH1(11, eventDetectedPhotons);

        if (eventGeneratedPhotons >= 1) {
            analysisManager->FillH1(12, muonEnergy);
        }
        if (eventGeneratedPhotons >= 2) {
            analysisManager->FillH1(27, muonEnergy);
        }
        if (eventGeneratedPhotons >= 3) {
            analysisManager->FillH1(28, muonEnergy);
        }
        if (eventGeneratedPhotons >= 4) {
            analysisManager->FillH1(29, muonEnergy);
        }
        if (eventGeneratedPhotons >= 4.5) {
            analysisManager->FillH1(30, muonEnergy);
        }
        
        
        if (eventDetectedPhotons >= 1) {
            analysisManager->FillH1(13, muonEnergy);
        }
        if (eventDetectedPhotons >= 2) {
            analysisManager->FillH1(22, muonEnergy);
        }
        if (eventDetectedPhotons >= 3) {
            analysisManager->FillH1(23, muonEnergy);
        }
        if (eventDetectedPhotons >= 3.5) {
            analysisManager->FillH1(24, muonEnergy);
        }
        if (eventDetectedPhotons >= 4) {
            analysisManager->FillH1(25, muonEnergy);
        }
        if (eventDetectedPhotons >= 4.5) {
            analysisManager->FillH1(26, muonEnergy);
        }

        if (fMuonTrackLength > 0) {
            G4double yield = fGeneratedCherenkovPhotons / fMuonTrackLength;  // photons/mm
            analysisManager->FillH1(17, yield);
            analysisManager->FillH2(1, muonEnergy, yield);
        }
    ///..................................................................///
    } else {
    G4cout << "Muon did NOT pass through all booklets (hit pattern: "
           << fBookletHits[0] << " "
           << fBookletHits[1] << " "
           << fBookletHits[2] << " "
           << fBookletHits[3] << ")" << G4endl;
    }

    //---------------------------------------------------------------------------------


    G4cout << "[DEBUG] EndOfEventAction — Gen: " << eventGeneratedPhotons
       << ", Det: " << eventDetectedPhotons << G4endl;
  
    G4cout << "Filled histogram with " << eventDetectedPhotons << " detected photons." << G4endl;
    runAction->eventID++;
    

}

//}
