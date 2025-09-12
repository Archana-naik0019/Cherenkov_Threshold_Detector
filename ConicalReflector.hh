#ifndef ConicalReflector_h
#define ConicalReflector_h 1

#include "G4ThreeVector.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4PVPlacement;

class ConicalReflector {
public:
    // motherLogic: logical volume to place the cone into (we'll place into gas logic)
    // gasPhys: physical volume of the gas (needed for border surface)
    // position: placement position (in motherLogic coordinates)
    ConicalReflector(G4LogicalVolume* motherLogic,
                     G4VPhysicalVolume* gasPhys,
                     const G4ThreeVector& position);
    ~ConicalReflector() = default;
};

#endif

