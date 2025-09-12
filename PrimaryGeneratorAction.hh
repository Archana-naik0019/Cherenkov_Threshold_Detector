#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "G4GeneralParticleSource.hh"

#include "TFile.h"
#include "TTree.h"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event* anEvent);
    
    G4double GetMuonEnergy() const { return fMuonEnergyGeV; }

private:
    G4double fMuonEnergyGeV;
    
    G4ParticleGun* fParticleGun;

    G4GeneralParticleSource* fGPS;
    G4bool useGPS; // switching between gun and GPS
    
     // for ROOT instructions//'''''''''''
    TFile* file;
    TTree* tree;
    Long64_t nEntries;
    Long64_t currentEntry;

    // Branch variables
    double px, py, pz, momentum, theta, phi;
    double id;

};

#endif
