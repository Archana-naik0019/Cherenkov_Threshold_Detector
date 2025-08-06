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

#include <fstream>

#include "QEManager.hh"
static QEManager qeManager("../QE_Curves_Q_1nm.csv");


SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {

    static std::ofstream outfile("secondary_4_info.txt", std::ios::app); //"app" to append event-wise info in same file, "out" would be over-writing it
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
            
            //G4int trackID = sec->GetTrackID();
            
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
            outfile << "[Secondary] ParentID: " << parentID
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
    }

    
    G4Track* track = step->GetTrack();
    

    
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) return;

    auto* runAction = const_cast<RunAction*>(
        static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    auto* eventAction = const_cast<EventAction*>(
        static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));

    if (!runAction || !eventAction) return;

    // Count generated photon
    if (track->GetCurrentStepNumber() == 1) {
        runAction->IncrementGeneratedPhotons();
        eventAction->IncrementEventGeneratedPhotons();
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////working counting without opabs//////////
//G4StepPoint* preStepPoint = step->GetPreStepPoint();
//G4StepPoint* postStepPoint = step->GetPostStepPoint();

//if (preStepPoint && postStepPoint) {
//    G4VPhysicalVolume* preVolume = preStepPoint->GetPhysicalVolume();
//    G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();

//    if (postVolume && postVolume->GetName() == "PMT" &&
//        (!preVolume || preVolume->GetName() != "PMT")) {
        
//        runAction->IncrementDetectedPhotons();
//        eventAction->IncrementEventDetectedPhotons();

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

G4double energy = track->GetTotalEnergy(); // in MeV
G4double wavelength = 1240.0 / (energy* 1.0e6);     // in nm

G4double qe = qeManager.GetQE(wavelength);
G4double rnd = G4UniformRand();

if (rnd < qe) {
    runAction->IncrementDetectedPhotons();
    eventAction->IncrementEventDetectedPhotons();
    G4cout << "[Detected] Photon at " << wavelength << " nm, QE = " << qe << G4endl;
} else {
    G4cout << "[Missed] Photon at " << wavelength << " nm, QE = " << qe << G4endl;
}

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

//******************************************************************************the following section is for getting photons generated per unit length***************************************
	G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
	//G4Track* track = step->GetTrack();

	// Only consider muons in GasPV
	if (volumeName == "GasPV" && track->GetDefinition()->GetParticleName() == "mu-") {
	    fTotalMuonStepLengthInGas += step->GetStepLength();
	}

	// Count secondary Cherenkov photons created in GasPV
	//const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
	for (auto sec : *secondaries) {
	    if (sec->GetCreatorProcess() && sec->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
		if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "GasPV") {
		    fTotalCherenkovPhotonsInGas++;
		}
	    }
	}
	
	EventAction* evt = const_cast<EventAction*>(dynamic_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));
	//EventAction* evt = static_cast<EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
	evt->AddMuonStepLengthInGas(fTotalMuonStepLengthInGas);
	evt->AddCherenkovPhotonsInGas(fTotalCherenkovPhotonsInGas);

	//resetting local counters at end of each event
	fTotalMuonStepLengthInGas = 0.0;
        fTotalCherenkovPhotonsInGas = 0;

//************************************************************************************************************************************************************************************************


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
