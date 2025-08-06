
#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void IncrementEventGeneratedPhotons() { eventGeneratedPhotons++; }
    void IncrementEventDetectedPhotons() { eventDetectedPhotons++; }

    void ResetEventCounters() {
        eventGeneratedPhotons = 0;
        eventDetectedPhotons = 0;
    }
    
    G4double muonEnergyThisEvent = 0.;
    void SetMuonEnergy(G4double e) { muonEnergyThisEvent = e; }
    G4double GetMuonEnergy() const { return muonEnergyThisEvent; }

    G4int GetGeneratedPhotonsThisEvent() const { return eventGeneratedPhotons; }//added later

    G4int GetDetectedPhotonsThisEvent() const { return eventDetectedPhotons; }
    
    //***********************************section to get photon yield*************
    G4double fTotalMuonStepLengthInGas = 0;
    G4int fTotalCherenkovPhotonsInGas = 0;

    void AddMuonStepLengthInGas(G4double);
    void AddCherenkovPhotonsInGas(G4int);

    G4double GetCherenkovPerUnitLengthInGas() const;
   //***************************************************************************

private:
    G4int eventGeneratedPhotons;
    G4int eventDetectedPhotons;

};

#endif
