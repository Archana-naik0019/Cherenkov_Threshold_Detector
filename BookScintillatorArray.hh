#ifndef BOOKSCINTILLATORARRAY_HH
#define BOOKSCINTILLATORARRAY_HH

#include "G4ThreeVector.hh"
#include "globals.hh"

class G4LogicalVolume;

class BookScintillatorArray {
public:
    BookScintillatorArray(G4LogicalVolume* worldLogic, G4ThreeVector cylinderCenter, G4double cylinderRadius, G4double cylinderHeight);
    void Construct();

    //adding sensitivity
     G4LogicalVolume* GetScintillatorLogicalVolume() const { return fScintillatorLogic; }

private:
    G4LogicalVolume* fWorldLogic;
    G4ThreeVector fCylinderCenter;
    G4double fCylinderRadius;
    G4double fCylinderHeight;
    
     G4LogicalVolume* fScintillatorLogic = nullptr; //sensitivity
};

#endif

