#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4MuonMinus.hh"
#include "G4SystemOfUnits.hh"
//#include "EventAction.hh"
#include "G4RandomTools.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);

    // Muon definition
    fParticleGun->SetParticleDefinition(G4MuonMinus::MuonMinusDefinition());

    // Oblique direction: ~7° from vertical
    G4double theta = 20 * deg;
    G4ThreeVector direction(std::sin(theta), 0., -std::cos(theta));
    fParticleGun->SetParticleMomentumDirection(direction);

    // Start at bottom left of gas cylinder
    fParticleGun->SetParticlePosition(G4ThreeVector(-30 * cm, 0 , 100.0 * cm));

    // Energy
    //fParticleGun->SetParticleEnergy(5.0 * GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    
    //flat gun for muon energy range 0.5 to 2.5 GeV
    G4double energy_GeV = 0.5 + (G4UniformRand() * (2.5 - 0.5));
    fMuonEnergyGeV = energy_GeV; // Store energy
    fParticleGun->SetParticleEnergy(energy_GeV * GeV);

    G4cout << "Generated muon with energy: " << energy_GeV << " GeV" << G4endl;
    
    fParticleGun->GeneratePrimaryVertex(event);
}
