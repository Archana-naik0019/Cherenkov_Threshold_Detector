#ifndef BOOKSCINTILLATORARRAY_HH
#define BOOKSCINTILLATORARRAY_HH

#include "G4ThreeVector.hh"
#include "globals.hh"

class G4LogicalVolume;

class BookScintillatorArray {
public:
    BookScintillatorArray(G4LogicalVolume* worldLogic, G4ThreeVector cylinderCenter, G4double cylinderRadius, G4double cylinderHeight);
    void Construct();

private:
    G4LogicalVolume* fWorldLogic;
    G4ThreeVector fCylinderCenter;
    G4double fCylinderRadius;
    G4double fCylinderHeight;
};

#endif

