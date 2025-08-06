#include "BookScintillatorArray.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

BookScintillatorArray::BookScintillatorArray(G4LogicalVolume* worldLogic, G4ThreeVector cylinderCenter, G4double cylinderRadius, G4double cylinderHeight)
    : fWorldLogic(worldLogic), fCylinderCenter(cylinderCenter), fCylinderRadius(cylinderRadius), fCylinderHeight(cylinderHeight)
{}


void BookScintillatorArray::Construct() {
    // Scintillator dimensions
    G4double scintX = 1.0 * cm;
    G4double scintY = 5.0 * cm;
    G4double scintZ = 5.0 * cm;

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box* scintSolid = new G4Box("Scintillator", scintX/2, scintY/2, scintZ/2);
    G4LogicalVolume* scintLogic = new G4LogicalVolume(scintSolid, scintMat, "ScintillatorLogic");

    // Distance from cylinder to paddle center
    G4double radialDistance = fCylinderRadius + scintZ / 2.0 + 5.0 * cm;
    G4double zOffset = fCylinderHeight / 4.0;

    // V Pair 1 (placed along +X axis, facing cylinder)
    {
        // Left paddle (leaning inwards, from above)
        {
            G4ThreeVector pos = fCylinderCenter + G4ThreeVector(+radialDistance, +scintY*0.8 / 2.0, +zOffset);
            G4RotationMatrix* rot = new G4RotationMatrix();
            rot->rotateZ(-45.0 * deg);  // lean inward
            new G4PVPlacement(rot, pos, scintLogic, "ScintillatorPhys", fWorldLogic, false, 0, true);
        }

        // Right paddle (leaning inwards, from below)
        {
            G4ThreeVector pos = fCylinderCenter + G4ThreeVector(+radialDistance, -scintY*0.8 / 2.0, +zOffset);
            G4RotationMatrix* rot = new G4RotationMatrix();
            rot->rotateZ(+45.0 * deg);  // lean inward
            new G4PVPlacement(rot, pos, scintLogic, "ScintillatorPhys", fWorldLogic, false, 1, true);
        }
    }

    // V Pair 2 (placed along -X axis, facing cylinder)
    {
        // Left paddle (leaning inwards, from above)
        {
            G4ThreeVector pos = fCylinderCenter + G4ThreeVector(-radialDistance, +scintY*0.8 / 2.0, -zOffset);
            G4RotationMatrix* rot = new G4RotationMatrix();
            rot->rotateZ(225.0 * deg);  // 180 - 45 -> inward
            new G4PVPlacement(rot, pos, scintLogic, "ScintillatorPhys", fWorldLogic, false, 2, true);
        }

        // Right paddle (leaning inwards, from below)
        {
            G4ThreeVector pos = fCylinderCenter + G4ThreeVector(-radialDistance, -scintY*0.8 / 2.0, -zOffset);
            G4RotationMatrix* rot = new G4RotationMatrix();
            rot->rotateZ(135.0 * deg);  // 180 + 45 -> inward
            new G4PVPlacement(rot, pos, scintLogic, "ScintillatorPhys", fWorldLogic, false, 3, true);
        }
    }
    

// -------------------- Book Pair 3 (angled +X side) ---------------------
{
    G4double angledOffset = scintY / 2.0; // Only declare this once per pair

    // Rotation +45
    G4RotationMatrix* rotA = new G4RotationMatrix();
    rotA->rotateZ(-45.0 * deg);
    G4ThreeVector deltaA = (*rotA) * G4ThreeVector(0, angledOffset, 0);

    // Rotation -45
    G4RotationMatrix* rotB = new G4RotationMatrix();
    rotB->rotateZ(+225.0 * deg);
    G4ThreeVector deltaB = (*rotB) * G4ThreeVector(0, angledOffset, 0);

    G4ThreeVector basePos = fCylinderCenter + G4ThreeVector(1.5 * radialDistance, 0, 1.5 * fCylinderHeight / 4);

    G4ThreeVector posA = basePos + deltaA;
    G4ThreeVector posB = basePos + deltaB;

    new G4PVPlacement(rotA, posA, scintLogic, "ScintillatorPhys", fWorldLogic, false, 100, true);
    new G4PVPlacement(rotB, posB, scintLogic, "ScintillatorPhys", fWorldLogic, false, 101, true);
}

// -------------------- Book Pair 4 (angled -X side) ---------------------
{
    G4double angledOffset = scintY / 2.0;

    G4RotationMatrix* rotA = new G4RotationMatrix();
    rotA->rotateZ(45.0 * deg);
    G4ThreeVector deltaA = (*rotA) * G4ThreeVector(0, angledOffset, 0);

    G4RotationMatrix* rotB = new G4RotationMatrix();
    rotB->rotateZ(135.0 * deg);
    G4ThreeVector deltaB = (*rotB) * G4ThreeVector(0, angledOffset, 0);

    G4ThreeVector basePos = fCylinderCenter + G4ThreeVector(-1.5 * radialDistance, 0, -1.5 * fCylinderHeight / 4);

    G4ThreeVector posA = basePos + deltaA;
    G4ThreeVector posB = basePos + deltaB;

    new G4PVPlacement(rotA, posA, scintLogic, "ScintillatorPhys", fWorldLogic, false, 102, true);
    new G4PVPlacement(rotB, posB, scintLogic, "ScintillatorPhys", fWorldLogic, false, 103, true);
}


}
