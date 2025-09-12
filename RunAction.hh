#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void IncrementReflectedPhotons();
    void IncrementTIRPhotons();
    void IncrementGeneratedPhotons();
    void IncrementDetectedPhotons();
    void IncrementPhotonsAtPMT();
    
    void IncrementPrimGeneratedPhotons();
    void IncrementSecGeneratedPhotons();

    void ConstructHistograms();
    void RecordAmplificationFactor(G4double factor);

    G4int eventID = 0;

private:
    G4int reflectedPhotons = 0;
    G4int tirPhotons = 0;
    G4int totalGeneratedPhotons = 0;
    G4int totalPhotonsAtPMT = 0;
    G4int totalDetectedPhotons = 0;

    std::vector<G4double> amplificationFactors;
};

#endif
