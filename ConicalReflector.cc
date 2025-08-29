#include "ConicalReflector.hh"

#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"


ConicalReflector::ConicalReflector(G4LogicalVolume* motherLogic,
                                   G4VPhysicalVolume* gasPhys,
                                   const G4ThreeVector& position)
{
    // --- Materials
    auto nist = G4NistManager::Instance();
    G4Material* steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    // --- Dimensions from your drawing (all in mm -> convert to *mm)
    // Top (outer) diameter = 210 mm -> outer top radius = 105 mm
    // Top inner opening diameter = 180 mm -> inner top radius = 90 mm
    // Bottom hole diameter = 50 mm -> inner bottom radius = 25 mm
    // Height = 70 mm -> half-height = 35 mm
    // Bottom flat ring thickness = 4 mm

    const G4double outerTopR = 105.0 * mm;
    const G4double innerTopR = 90.0  * mm;
    const G4double innerBotR = 25.0  * mm; //prev 25
    const G4double bottomRingThickness = 4.0 * mm;
    const G4double height = 70.0 * mm;
    const G4double halfHeight = height * 0.5;
    
    const G4double eps = 0.1*mm;

    // We'll create:
    // 1) Outer solid (solid cone/slanted outer): using G4Cons with outer radii varying
    // 2) Inner cone void (hollow) using G4Cons with inner radii varying
    // 3) Subtract inner cone from outer to make a thin-walled funnel
    // 4) Add a bottom ring (4 mm thick) using G4Tubs unioned with the shell, leaving central hole

    // Outer solid: from bottom outer radius = innerBotR + bottomRingThickness (so outer bottom radius)
    const G4double outerBotR = innerBotR + bottomRingThickness; // 25 + 4 = 29 mm

    G4Tubs* outerCyl = new G4Tubs("Reflector_outerCyl",
                              0.0*mm,
                              outerTopR,     // radius = 105 mm
                              halfHeight,    // 70/2 = 35 mm
                              0.*deg, 360.*deg);

    // Inner void (hollow cone): bottom radius = innerBotR, top radius = innerTopR
    G4Cons* innerCons = new G4Cons("Reflector_innerCons",
                                   0.0*mm, innerBotR,   // rmin1, rmax1 (bottom)
                                   0.0*mm, innerTopR,   // rmin2, rmax2 (top)
                                   halfHeight + eps, 0.*deg, 360.*deg);

    G4SubtractionSolid* reflectorSolid =
    new G4SubtractionSolid("Reflector_solid", outerCyl, innerCons);

  // --- Logical volume (stainless steel)
G4LogicalVolume* logicReflector =
    new G4LogicalVolume(reflectorSolid, steel, "ReflectorLV");

// --- Place reflector in detector
G4VPhysicalVolume* physReflector = new G4PVPlacement(
    nullptr, position, logicReflector, "ReflectorPV", motherLogic, false, 0, true);

    // --- Optical surface: reflectivity = 0.9 across photonE range given by user
    const G4int nEntries = 2;
    G4double photonE[nEntries] = {
        1.239841939 * eV / 0.9,
        1.239841939 * eV / 0.1
    };
    G4double reflectivity[nEntries] = { 0.9, 0.9 };
    //G4double efficiency[nEntries]   = { 0.0, 0.0 };

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("REFLECTIVITY", photonE, reflectivity, nEntries);
    //mpt->AddProperty("EFFICIENCY",   photonE, efficiency,   nEntries);

    G4OpticalSurface* optSurface = new G4OpticalSurface("ReflectorInnerOptSurface");
    optSurface->SetType(dielectric_metal);
    optSurface->SetFinish(polished);
    optSurface->SetModel(unified);
    optSurface->SetMaterialPropertiesTable(mpt);

    // --- Apply border surface only between the reflector physical volume and the gas physical volume ---
    // physReflector is the reflector PV; gasPhys must be the physical volume of the gas (GasPV)
    // NOTE: The order physReflector, gasPhys matters: border surface registered between the two PVs.
    new G4LogicalBorderSurface("Reflector_Gas_Border", gasPhys, physReflector, optSurface);

    // (visualization optional)
     auto vis = new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.3));
     vis->SetForceSolid(true);
     logicReflector->SetVisAttributes(vis);
}

