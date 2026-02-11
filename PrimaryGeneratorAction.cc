#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4MuonMinus.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "EventAction.hh"
#include "G4RandomTools.hh"
#include "SteppingAction.hh"		//additional condition on muons(scint pass)

#include "TMath.h"


PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);
    
     // Open CORSIKA ROOT file//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    file = new TFile("flux_sibyll_fluka_jim_raj.root");
    tree = (TTree*)file->Get("ttree");

    tree->SetBranchAddress("px", &px);
    tree->SetBranchAddress("py", &py);
    tree->SetBranchAddress("pz", &pz);
    tree->SetBranchAddress("momentum", &momentum);
    tree->SetBranchAddress("theta", &theta);
    tree->SetBranchAddress("phi", &phi);
    tree->SetBranchAddress("id", &id);

    nEntries = tree->GetEntries();
    currentEntry = 0;

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

    // Muon definition
    //fParticleGun->SetParticleDefinition(G4MuonMinus::MuonMinusDefinition());    

    // Energy
    //fParticleGun->SetParticleEnergy(5.0 * GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
    //if(file) file->Close(); //'''''''''''''''''might be causing crash
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::(prev attempt):::::::::::::::::::::::::::    
    //flat gun for muon energy range 0.5 to 2.5 GeV
//    G4double energy_GeV = 0.25 + (G4UniformRand() * (0.5 - 0.5));
//    fMuonEnergyGeV = energy_GeV; // Store energy
//    fParticleGun->SetParticleEnergy(energy_GeV * GeV);
    
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    // Randomize theta and phi within ranges
//    G4double theta_min = 16.5 * deg;   // lower bound
//    G4double theta_max = 17 * deg;   // upper bound
//    G4double phi_min   = 174 * deg;  // lower bound
//    G4double phi_max   = 175 * deg;  // upper bound

//    G4double theta = theta_min + (theta_max - theta_min) * G4UniformRand();
//    G4double phi   = phi_min   + (phi_max   - phi_min)   * G4UniformRand();

//    G4ThreeVector dir(std::sin(theta) * std::cos(phi),
//                      std::sin(theta) * std::sin(phi),
//                      -std::cos(theta));
//    dir = dir.unit(); // normalize
//    fParticleGun->SetParticleMomentumDirection(dir);

    // Position (fixed for now)
//    fParticleGun->SetParticlePosition(G4ThreeVector(40.0 * cm, 0 * cm , 150.0 * cm));

    // Debug print
//    G4cout << "Generated muon with E = " << energy_GeV << " GeV, "
//           << "theta = " << theta/deg << " deg, "
//           << "phi = "   << phi/deg   << " deg" << G4endl;

    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//    G4cout << "Generated muon with energy: " << energy_GeV << " GeV" << G4endl;
    
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//====================================redef with corsika input========================''''

G4double Emin = 1.045 * GeV;  //ooooooo00000000ooooooo
//G4double Emax = 3.115 * GeV;  //ooooooo0000000ooooooo
bool found = false; //ooooooo000000ooooo
while (!found) {	//ooooooooo0000000ooooo


 if(currentEntry >= nEntries) currentEntry = 0;
    tree->GetEntry(currentEntry);
    currentEntry++;

    G4ThreeVector mom(px*GeV, py*GeV, -pz*GeV);	//beam axis convention might be different
    G4double p = mom.mag();
    
    G4double theta_mom = mom.theta();  // in rad
    G4double phi_mom   = mom.phi();    // in rad
    
    //G4double muonMass = 105.66 * MeV; //rest mass
    G4double muonMass = G4MuonMinus::MuonMinusDefinition()->GetPDGMass();

    double c = TMath::C(); //m/s
    G4double totalE = std::sqrt(p*p + muonMass*muonMass);
    
    G4double KE = totalE - muonMass;       // kinetic energy [MeV]
    
    fMuonEnergyGeV = KE / GeV;
    
    // Keep looping until KE is within desired range
    if (fMuonEnergyGeV >= Emin/GeV) {
//    if (fMuonEnergyGeV >= Emin/GeV && fMuonEnergyGeV <= Emax/GeV) {	//ooooo00000oooo
        found = true;	//oooooooo000000ooooooo
    
}   

    // Assign mu+/mu- based on ID
    
    int pid = static_cast<int>(std::round(id));
    
//*/    G4cout << "DEBUG: raw id = " << id 
//*/       << " rounded pid = " << pid << G4endl;

    
    if(pid == 5) {
    fParticleGun->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle("mu+"));
  } else if(pid == 6) {
    fParticleGun->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
  } else {
    G4cerr << "Warning: unexpected particle ID = " << id 
           << " (not 5 or 6)" << G4endl;
  }

    // Set momentum
    fParticleGun->SetParticleMomentumDirection(mom.unit());
    fParticleGun->SetParticleMomentum(p);

    // Define vertex: plane above detector: this is the mid junction of the intersection of 2 scitillators of the top-most booklet
//    double x = 23*cm; // e.g. ±0.5 m
//    double y = 0*cm;
//    double z = 100*cm;
    // Define vertex: plane above detector: this is the ~mid-point of one of the 2 scitillators of the top-most booklet(the one towards +y and -x wrt previous gun position)
      double x = 16*cm; // e.g. ±0.5 m
      double y = 7*cm;
      double z = 100*cm;


    
    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

    // Debug print
//*/    G4cout << "Generated muon with E = " << fMuonEnergyGeV << " GeV, "
//*/           << "theta = " << theta_mom/deg << " deg, "
//*/           << "phi = "   << phi_mom/deg   << " deg" << G4endl;
           
    //~~~~~~~~~~~~~~~~~~~~~~~
    auto eventAction = (EventAction*) 
      G4RunManager::GetRunManager()->GetUserEventAction();

    if(eventAction) {
      eventAction->SetThetaPhi(theta_mom, phi_mom);
    }
}

//====================================================================================''''
    
    fParticleGun->GeneratePrimaryVertex(event);
} //must match with bracs on line 46
