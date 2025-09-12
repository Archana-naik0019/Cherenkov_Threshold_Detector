
#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"
#include <array>

class EventAction : public G4UserEventAction {
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void IncrementEventGeneratedPhotons() { eventGeneratedPhotons++; }
    void IncrementEventDetectedPhotons() { eventDetectedPhotons++; }
    void IncrementEventPhotonsAtPMT() { eventPhotonsAtPMT++; }
    

    void IncrementPrimGeneratedPhotons() { eventPrimGeneratedPhotons++; }
    void IncrementSecGeneratedPhotons() { eventSecGeneratedPhotons++; }

    
    void AddMuonTrackLength(G4double length) { fMuonTrackLength += length; }
    void IncrementGeneratedCherenkovPhotons() { ++fGeneratedCherenkovPhotons; }

    void ResetEventCounters() {
        eventGeneratedPhotons = 0;
        eventDetectedPhotons = 0;
        eventPhotonsAtPMT = 0;
        
        eventPrimGeneratedPhotons = 0;
        eventSecGeneratedPhotons = 0;

        
        fMuonTrackLength = 0.0;
        fGeneratedCherenkovPhotons = 0;
    }
    
    G4double muonEnergyThisEvent = 0.;
    void SetMuonEnergy(G4double e) { muonEnergyThisEvent = e; }
    G4double GetMuonEnergy() const { return muonEnergyThisEvent; }

    G4int GetGeneratedPhotonsThisEvent() const { return eventGeneratedPhotons; }//added later

    G4int GetDetectedPhotonsThisEvent() const { return eventDetectedPhotons; }
    
    //--------------------4-fold vis scint----------------------------------------
    void SetBookletHit(G4int bookletIndex) { fBookletHits[bookletIndex] = true; }
    G4bool AllBookletsHit() const {
        for (G4bool hit : fBookletHits) if (!hit) return false;
        return true;
    }
    void ResetBookletHits() { std::fill(fBookletHits.begin(), fBookletHits.end(), false); }
    //-----------------------------------------------------------------------------
    
    void SetThetaPhi(G4double theta_mom, G4double phi_mom) {
        fTheta = theta_mom;
        fPhi   = phi_mom;
    }

private:
    G4int eventGeneratedPhotons;
    G4int eventDetectedPhotons;
    G4int eventPhotonsAtPMT;
    
    G4int eventPrimGeneratedPhotons = 0;  // NEW
    G4int eventSecGeneratedPhotons = 0;   // NEW
    
    G4double fMuonTrackLength = 0.0; //for yield
    G4int fGeneratedCherenkovPhotons = 0; //for yield
    
    std::array<G4bool, 4> fBookletHits; // = {false, false, false, false}; //mapping for 4-fold
    
    G4double fTheta;
    G4double fPhi;

};

#endif
