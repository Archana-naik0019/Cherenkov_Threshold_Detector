#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"


EventAction::EventAction()
//*******************************added to get photon yield
 : G4UserEventAction(),
   fTotalMuonStepLengthInGas(0.),
   fTotalCherenkovPhotonsInGas(0)
{}
//**************************************
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    ResetEventCounters();
    
    fTotalMuonStepLengthInGas = 0.; //*******for yield
    fTotalCherenkovPhotonsInGas = 0; //********for yield

}

void EventAction::EndOfEventAction(const G4Event*event) {
    auto* analysisManager = G4AnalysisManager::Instance();
    auto* runAction = const_cast<RunAction*>(
        static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));

    analysisManager->FillH1(0, eventGeneratedPhotons);  
    analysisManager->FillH1(1, eventDetectedPhotons);
    
   auto* runManager = G4RunManager::GetRunManager();
   auto* generatorAction = static_cast<const PrimaryGeneratorAction*>(runManager->GetUserPrimaryGeneratorAction());
   
   G4double muonEnergy = generatorAction->GetMuonEnergy();

   if (eventGeneratedPhotons > 0) {
       auto* analysisManager = G4AnalysisManager::Instance();
       analysisManager->FillH1(2, muonEnergy);
}
       
   if (eventDetectedPhotons > 0) {
       auto* analysisManager = G4AnalysisManager::Instance();
       analysisManager->FillH1(3, muonEnergy);
}

    G4cout << "[DEBUG] EndOfEventAction — Gen: " << eventGeneratedPhotons
       << ", Det: " << eventDetectedPhotons << G4endl;
  
    G4cout << "Filled histogram with " << eventDetectedPhotons << " detected photons." << G4endl;
    runAction->eventID++;


//*******************************************************************************************
if (fTotalMuonStepLengthInGas > 0) {

        G4double yield = GetCherenkovPerUnitLengthInGas();
        
        auto analysisManager = G4AnalysisManager::Instance();
        //G4double yield = AddCherenkovPhotonsInGas / AddMuonStepLengthInGas;
        analysisManager->FillNtupleDColumn(0, yield);  // Column 0 for yield
        analysisManager->AddNtupleRow();
        
        //auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillH1(eventGeneratedPhotons, fTotalCherenkovPhotonsInGas);

    }
//*******************************************************************************************
}
//*****************************************************************section to get photon yield***************************************************
void EventAction::AddMuonStepLengthInGas(G4double len) {
    fTotalMuonStepLengthInGas += len;
}

void EventAction::AddCherenkovPhotonsInGas(G4int count) {
    fTotalCherenkovPhotonsInGas += count;
}

G4double EventAction::GetCherenkovPerUnitLengthInGas() const {
    return fTotalMuonStepLengthInGas > 0 ? fTotalCherenkovPhotonsInGas / fTotalMuonStepLengthInGas : 0.0;
}

//************************************************************
