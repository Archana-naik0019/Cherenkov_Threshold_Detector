//tester brac commented in line 155

#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include <fstream>

#include "BookScintillatorArray.hh"
#include "G4LogicalVolumeStore.hh"

#include "QEManager.hh"
static QEManager qeManager("../QE_Curves_Q_1nm.csv");


SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {


    /*
    static std::ofstream outfile("check_12_info100.txt", std::ios::app); //"app" to append event-wise info in same file, "out" would be over-writing it
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    
    
    static G4int lastEventID = -1;
    if (eventID != lastEventID) {
       auto* eventAction = const_cast<EventAction*>(
           static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));  //adding number of detected and generated photons for each event
       //G4int gen = eventAction ? eventAction->GeteventGeneratedPhotons() : -1;
       //G4int det = eventAction ? eventAction->GeteventDetectedPhotons() : -1;

        outfile << "\n\n"
                << "======================================" << std::endl;
        outfile << "========  Event " << eventID << " Start  =========" << std::endl;
        outfile << "======================================" << std::endl;
        
        lastEventID = eventID;
    }
    
    //logging secondaries
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    if (secondaries) {
        for (const auto& sec : *secondaries) {
            G4int parentID = sec->GetParentID();
            const G4VProcess* creator = sec->GetCreatorProcess();

            G4String procName = creator ? creator->GetProcessName() : "primary";
            
             // Get particle name
            G4String partName = sec->GetDefinition()->GetParticleName();
            
            // Get volume name
            G4VPhysicalVolume* secVolume = sec->GetVolume();
            G4String volName = (secVolume) ? secVolume->GetName() : "Unknown";
            
            
            // Parent name (if available)
            G4String parentName = "unknown";
            if (parentID > 0) {
                // Geant4 does not keep full parent track pointer directly in secondary list
                // We can approximate or tag as "secondary from parentID X"
                parentName = "ParentID_" + std::to_string(parentID);
            } else {
                parentName = "primary";
            }
            
            // Wavelength (if optical photon)
            G4double wavelength_nm = -1.;
            if (partName == "opticalphoton") {
                G4double photonEnergy = sec->GetTotalEnergy();
                wavelength_nm = (1.239841939e3) / (photonEnergy / CLHEP::eV);
            }
            
            //dumping the info in a file
            outfile << "[Secondary] | ParentID: " << parentID
                    //<< " | Parent: " << parentName
                    << " | Created by: " << procName
                    //<< trackID
                    << " | Particle: " << partName
                    << " | Volume: " << volName
                    << " | E: " << sec->GetKineticEnergy() / CLHEP::MeV << " MeV";
                    //<< std::endl;
                    
            if (wavelength_nm > 0)
                outfile << " | λ: " << wavelength_nm << " nm";
            outfile << std::endl;
            
            
            ///G4cout << "[Secondary] ParentID: " << parentID
                   //<< " | Parent: " << parentName
                   ///<< " | Created by: " << procName
                   //<< trackID
                   ///<< " | Particle: " << partName
                   ///<< " | Volume: " << volName
                   ///<< " | E: " << sec->GetKineticEnergy() / CLHEP::MeV << " MeV";
                   
                   ///if (wavelength_nm > 0)
                      ///G4cout << " | λ: " << wavelength_nm << " nm";
                   ///G4cout << G4endl;
                   
        }
    }*/
   //commenting above part due to storage issues, required to obtain track_info.txt)
    
    G4Track* track = step->GetTrack();
    
    //*******************************************************************************
    if (track->GetDefinition()->GetParticleName() == "mu-"  ||
        track->GetDefinition()->GetParticleName() == "mu+") {	//''''''''''''
    G4double stepLength = step->GetStepLength();
    EventAction* eventAction = static_cast<EventAction*>(
    G4EventManager::GetEventManager()->GetUserEventAction());
    eventAction->AddMuonTrackLength(stepLength);
    }
    //*****************************************************************************
    
    //----------for 4-fold via scint----------------------------------------attempt1-----------
    //auto eventAction = const_cast<EventAction*>(
    //static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction())
    //);
    //auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    //if (volume == Logic_Booklet1) eventAction->SetBookletHit(0);
    //if (volume == Logic_Booklet2) eventAction->SetBookletHit(1);
    //if (volume == Logic_Booklet3) eventAction->SetBookletHit(2);
    //if (volume == Logic_Booklet4) eventAction->SetBookletHit(3);
   //---------------------------------------------------------------------attempt2--------
   auto* eventAction = const_cast<EventAction*>(
        static_cast<const EventAction*>(
            G4RunManager::GetRunManager()->GetUserEventAction()));

    // Get current logical volume
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // Retrieve your booklet logicals by name (only the first time)
    static auto* logicB1 = G4LogicalVolumeStore::GetInstance()->GetVolume("Logic_Booklet1");
    static auto* logicB2 = G4LogicalVolumeStore::GetInstance()->GetVolume("Logic_Booklet2");
    static auto* logicB3 = G4LogicalVolumeStore::GetInstance()->GetVolume("Logic_Booklet3");
    static auto* logicB4 = G4LogicalVolumeStore::GetInstance()->GetVolume("Logic_Booklet4");

    if (volume == logicB1){
        eventAction->SetBookletHit(0); //added laterr
    }
    if (volume == logicB2){
        eventAction->SetBookletHit(1); //added laterr
    }
    if (volume == logicB3){
        eventAction->SetBookletHit(2); //added laterr
    }
    if (volume == logicB4){
        eventAction->SetBookletHit(3); //added laterr
    }
    //----------------------------------------------------------------------------------------

    
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) return;

G4double energy = track->GetTotalEnergy(); // in MeV
G4double wavelength = 1239.841939 / (energy* 1.0e6);     // in nm 

    auto* runAction = const_cast<RunAction*>(
        static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    //auto* eventAction = const_cast<EventAction*>(
        //static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction())); //re-defined on line 42

    if (!runAction || !eventAction) return;

    // Count generated photon
    if (track->GetCurrentStepNumber() == 1) {
    
    //only ids other than muon/////
    G4int parentID = step->GetTrack()->GetParentID();
    //if (parentID != 1) {
    ////////
    
        runAction->IncrementGeneratedPhotons();
        eventAction->IncrementEventGeneratedPhotons();
        
        // Separate counters===============================
    if (parentID == 1) {
        runAction->IncrementGeneratedPhotons();
        eventAction->IncrementPrimGeneratedPhotons();   // New method
        
        G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4double muonEnergy = eventAction->GetMuonEnergy();
        static G4int lastLoggedEventID = -1;
        static std::ofstream primEventsFile("PrimPhotonEvents.txt", std::ios::app);

        if (eventID != lastLoggedEventID) {
           primEventsFile << eventID
           << " | MuonEnergy = " << muonEnergy/CLHEP::GeV << " GeV"
           << std::endl;
           lastLoggedEventID = eventID;
        }
        
    } else {
        runAction->IncrementGeneratedPhotons();
        eventAction->IncrementSecGeneratedPhotons();    // New method
        
        G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4double muonEnergy = eventAction->GetMuonEnergy();
        static G4int lastLoggedEventID = -1;
        static std::ofstream primEventsFile("SecPhotonEvents.txt", std::ios::app);

        if (eventID != lastLoggedEventID) {
           primEventsFile << eventID
           << " | MuonEnergy = " << muonEnergy/CLHEP::GeV << " GeV"
           << std::endl;
           lastLoggedEventID = eventID;
        }
    }
    ////////////====================================
        
        auto* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(analysisManager->GetH1Id("hGeneratedWavelength"), wavelength);
        
        if (eventAction->AllBookletsHit()) {
        analysisManager->FillH1(analysisManager->GetH1Id("hGeneratedWavelength_4-fold"), wavelength);
        }//trial
    }
//    }
    
    //******************************************************************************
        if (track->GetDefinition()->GetParticleName() == "opticalphoton" &&
            track->GetCreatorProcess() &&
            track->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
    
            eventAction->IncrementGeneratedCherenkovPhotons();
        }

    //*********************************************************************************

//////////////////////////////////////////////////////////////////////////////////////////////////////working counting without opabs,without qe//////////
//G4StepPoint* preStepPoint = step->GetPreStepPoint();
//G4StepPoint* postStepPoint = step->GetPostStepPoint();

//if (preStepPoint && postStepPoint) {
//    G4VPhysicalVolume* preVolume = preStepPoint->GetPhysicalVolume();
//    G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();

//    if (postVolume && postVolume->GetName() == "PMT" &&
//        (!preVolume || preVolume->GetName() != "PMT")) {
        
//        runAction->IncrementPhotonsAtPMT();
//        eventAction->IncrementEventPhotonsAtPMT();

//        G4cout << "[Detected] Optical Photon entered the PMT!" << G4endl;
//        track->SetTrackStatus(fStopAndKill);
//    }
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////counting without opabs, with qe/////////////////////////

G4StepPoint* preStepPoint = step->GetPreStepPoint();
G4StepPoint* postStepPoint = step->GetPostStepPoint();

if (preStepPoint && postStepPoint) {
    G4VPhysicalVolume* preVolume = preStepPoint->GetPhysicalVolume();
    G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();

if (postVolume && postVolume->GetName() == "PMT" &&
    (!preVolume || preVolume->GetName() != "PMT")) {
    
        runAction->IncrementPhotonsAtPMT(); //additional record keeping
        eventAction->IncrementEventPhotonsAtPMT();
        auto* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(analysisManager->GetH1Id("hAtPMTWavelength"), wavelength);
        
        if (eventAction->AllBookletsHit()) {
        analysisManager->FillH1(analysisManager->GetH1Id("hAtPMTWavelength_4-fold"), wavelength);
        }


///////////////////////////////////////////////////////////////////////////////

G4double qe = qeManager.GetQE(wavelength);
G4double rnd = G4UniformRand();

if (rnd < qe) {
    runAction->IncrementDetectedPhotons();
    eventAction->IncrementEventDetectedPhotons();
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(analysisManager->GetH1Id("hDetectedWavelength"), wavelength);
    
    if (eventAction->AllBookletsHit()) {
    analysisManager->FillH1(analysisManager->GetH1Id("hDetectedWavelength_4-fold"), wavelength);
        }
     
//*/    G4cout << "[Detected] Photon at " << wavelength << " nm, QE = " << qe << G4endl;
//*/} else {
//*/    G4cout << "[Missed] Photon at " << wavelength << " nm, QE = " << qe << G4endl;
//*/}

track->SetTrackStatus(fStopAndKill);


////// Count detected photon (PMT volume)////////////////////////////////////////////////////////////vvvimp(not to be removed)
//    if (track->GetVolume()->GetName() == "PMT") {
//      G4StepPoint* postStepPoint = step->GetPostStepPoint();
//        if (postStepPoint && postStepPoint->GetPhysicalVolume() &&
//          postStepPoint->GetPhysicalVolume()->GetName() == "PMT") {

//          runAction->IncrementDetectedPhotons();
//          eventAction->IncrementEventDetectedPhotons();

//          G4cout << "[Detected] Optical Photon hit the PMT!" << G4endl;
//          track->SetTrackStatus(fStopAndKill);
//    }
////////////////////////////////////////////////////////////////////////////////////////////////attempt1

// With QE — only count if absorbed at PMT surface
//	G4StepPoint* postStepPoint = step->GetPostStepPoint();
//	const G4VProcess* process = postStepPoint->GetProcessDefinedStep();

//	if (process && process->GetProcessName() == "OpAbsorption") {
//	    auto* volume = postStepPoint->GetPhysicalVolume();
//	    if (volume && volume->GetName() == "PMT") {
//		runAction->IncrementDetectedPhotons();
//		eventAction->IncrementEventDetectedPhotons();

//		G4cout << "[Detected via QE] Optical Photon absorbed at PMT!" << G4endl;
//		track->SetTrackStatus(fStopAndKill);
//	    }
//	}
//    }
////////////////////////////////###############################################################////attempt2

//if (track->GetVolume()->GetName() == "PMT") {
    //Apply QE check logic ---
    //G4double energy = track->GetTotalEnergy(); // in MeV
    //G4double wavelength_nm = 1240.0 / (energy / eV); // convert to nm
//    int roundedWavelength = std::round(wavelength_nm);

//    auto it = qeByRoundedWavelength.find(roundedWavelength);
//    if (it != qeByRoundedWavelength.end()) {
//        double efficiency = it->second;
//        double random = G4UniformRand();

//        if (random < efficiency) {
//            runAction->IncrementDetectedPhotons();
//            eventAction->IncrementEventDetectedPhotons();
//            G4cout << "[Detected] Optical Photon with λ=" << wavelength_nm << " nm hit PMT!" << G4endl;
//            track->SetTrackStatus(fStopAndKill);
//        } else {
            // Not detected: optional to kill or allow to continue
//            track->SetTrackStatus(fStopAndKill);
//        }
//    } else {
        // QE not defined for this wavelength — skip or kill
//        track->SetTrackStatus(fStopAndKill);
//}
//}

///////////////////////////////////////////////////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    // Reflection/TIR handling
//    G4StepPoint* postStepPoint = step->GetPostStepPoint();
//    if (postStepPoint->GetStepStatus() == fGeomBoundary) {
//        const G4VProcess* process = postStepPoint->GetProcessDefinedStep();

//        if (process && process->GetProcessName() == "OpBoundary") {
//            auto* boundary = dynamic_cast<const G4OpBoundaryProcess*>(process);
//            if (boundary) {
//                G4OpBoundaryProcessStatus status = boundary->GetStatus();

//                if (status == FresnelReflection) {
//                    runAction->IncrementReflectedPhotons();
//                    G4cout << "[Reflection] Photon reflected at " << track->GetPosition() << G4endl;
//                } else if (status == TotalInternalReflection) {
//                    runAction->IncrementTIRPhotons();
//                    G4cout << "[TIR] Photon TIR at " << track->GetPosition() << G4endl;
//                }
//            }
        }
  }
}
}
