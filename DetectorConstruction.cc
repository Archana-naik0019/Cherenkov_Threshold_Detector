//line 125 to alter pressure
//#include "QuantumEfficiencyLoader.hh"

#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

#include "BookScintillatorArray.hh"
#include "ConicalReflector.hh"

#include "RefractiveIndexModel.hh"

#include "CLHEP/Units/PhysicalConstants.h"


//Using Pressure to refractive index relation with some corrections from the engineering handbook ():::::::::::::::::::::::::::::::::::::

//Instead of a linear relation, trying to get 'n' and 'P' relation from the ideal gas law ++++++++++++++++++++++++++++++++++++++++++++++++
G4Material* CreatePressurizedNitrogenWithOpticalProperties(G4double pressure,
                                                           G4double temperature = 293.15 * kelvin) {
    auto* nist = G4NistManager::Instance();

    // Reference conditions
    const G4double baseRI = 1.000298;                // at P0 = 1 atm, T0 = 293.15 K
    const G4double T0 = 293.15 * kelvin;
    const G4double P0 = atmosphere;

    // Refractive index scaling(updated)
    //G4double scaledRI = 1.0 + (baseRI - 1.0) * (pressure / P0) * (T0 / temperature); 
    G4double scaledRI = RefractiveIndex_FromPressureAndTemp(pressure, temperature); // Refractive index from interpolation table (defined in RefractiveIndexModel.cc):::


    // not necessary in our case: since N₂ is transparent in visible range)
    const G4double baseAbs = 1000 * m;
    G4double scaledAbs = baseAbs * (P0 / pressure);  // Inverse proportionality to number density

    // Photon energy range (for optical region)
    const G4int nEntries = 2;
    G4double photonEnergy[nEntries] = {
        1.239841939 * eV / 0.9,   // ~1.377 eV (900 nm)
        1.239841939 * eV / 0.1    // ~6.2 eV (200 nm)
    };
    G4double rIndexArray[nEntries];
    G4double absLengthArray[nEntries];

    for (int i = 0; i < nEntries; ++i) {
        rIndexArray[i] = scaledRI;
        absLengthArray[i] = scaledAbs;
    }

    // Create material with appropriate number density
    G4double molarMass = 28.0134 * g / mole;  // For N2
    G4double numberDensity = (pressure) / (CLHEP::k_Boltzmann * temperature);
    G4double density = numberDensity * molarMass / CLHEP::Avogadro;

    G4Material* nitrogen = new G4Material("PressurizedNitrogen", density, 1, kStateGas, temperature, pressure);
    nitrogen->AddElement(nist->FindOrBuildElement("N"), 2);

    auto* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX", photonEnergy, rIndexArray, nEntries);
    mpt->AddProperty("ABSLENGTH", photonEnergy, absLengthArray, nEntries);
    nitrogen->SetMaterialPropertiesTable(mpt);

    return nitrogen;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//G4Material* CreatePressurizedNitrogenWithOpticalProperties(G4double pressure) {
//    auto* nist = G4NistManager::Instance();
//    G4double baseRI = 1.000298;
//    G4double baseAbs = 100 * m;

//    G4double scaledRI = 1.0 + (baseRI - 1.0) * (pressure / atmosphere);		//needs to be worked out from ideal gas law -> tweaks from Engineering handbook
//    G4double scaledAbs = baseAbs * (atmosphere / pressure);			//absorption length is not significant to be considered in our case

//    const G4int nEntries = 2;
//    G4double photonEnergy[nEntries] = {
//        1.239841939 * eV / 0.9,
//        1.239841939 * eV / 0.2
//    };
//    G4double rIndexArray[nEntries];
//    G4double absLengthArray[nEntries];
//    for (int i = 0; i < nEntries; ++i) {
//        rIndexArray[i] = scaledRI;
//        absLengthArray[i] = scaledAbs;
//    }

    ///G4Material* nitrogen = nist->FindOrBuildMaterial("G4_lN2"); //can be replace by G4_lN2 for using "liquid Nitrogen"
    ///G4Material* pressurizedNitrogen = new G4Material("PressurizedN2", pressure * nitrogen->GetDensity() / atmosphere, nitrogen, kStateGas, 293.15*kelvin, pressure);


//      G4Element* elN = nist->FindOrBuildElement("N");  // atomic nitrogen
//      G4double referenceDensity = 0.0012506 * g/cm3;    // at 1 atm, 0°C
//      G4double density = pressure * referenceDensity / atmosphere;

//      G4Material* pressurizedNitrogen = new G4Material("PressurizedN2", density, 1, kStateGas, 293.15 * kelvin, pressure);
//	pressurizedNitrogen->AddElement(elN, 2); // molecular nitrogen N₂



//    auto* mpt = new G4MaterialPropertiesTable();
//    mpt->AddProperty("RINDEX", photonEnergy, rIndexArray, nEntries);
//    mpt->AddProperty("ABSLENGTH", photonEnergy, absLengthArray, nEntries);
//    pressurizedNitrogen->SetMaterialPropertiesTable(mpt);

//    return pressurizedNitrogen;
//}
//======================================================================================================================================

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto* nist = G4NistManager::Instance();

    auto* air = nist->FindOrBuildMaterial("G4_AIR");
    auto* steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    auto* quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    auto* silicon = nist->FindOrBuildMaterial("G4_Si"); //for pmt(changed from quartz)
    auto* mylar = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    auto* gas = CreatePressurizedNitrogenWithOpticalProperties(9.0 * atmosphere);
        
    
    // Adding optical properties to Air (world material)
    const G4int numEntriesAir = 2;
    G4double photonEnergyAir[numEntriesAir] = { 1.239841939 * eV / 0.9,  1.239841939 * eV / 0.1 };
    G4double airRefractiveIndex[numEntriesAir] = {1.00, 1.00}; 

    auto* airMPT = new G4MaterialPropertiesTable();
    airMPT->AddProperty("RINDEX", photonEnergyAir, airRefractiveIndex, numEntriesAir);

    // Attaching to world material
    //auto* worldMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    air->SetMaterialPropertiesTable(airMPT);



    // Quartz optical properties
    const G4int n = 2;
    G4double photonE[n] = {1.239841939 * eV / 0.9, 1.239841939 * eV / 0.1};
    G4double rQuartz[n] = {1.46, 1.46};
    G4double absQuartz[n] = {6.21 * cm, 6.21 * cm};
    auto* quartzMPT = new G4MaterialPropertiesTable();
    quartzMPT->AddProperty("RINDEX", photonE, rQuartz, n);
    quartzMPT->AddProperty("ABSLENGTH", photonE, absQuartz, n);
    quartz->SetMaterialPropertiesTable(quartzMPT);
    
    //Si abslength defined for qe addition
    //G4double abssilicon[n] = {1 * um, 1 * um};
    //auto* siliconMPT = new G4MaterialPropertiesTable();
    //siliconMPT->AddProperty("ABSLENGTH", photonE, abssilicon, n);
    //silicon->SetMaterialPropertiesTable(siliconMPT);			//this section is added for qe addition in the setup

    G4double tubeLength = 180 * cm;
    G4double tubeRadius = 10 * cm;
    G4double tubeThickness = 0.5 * cm;
    G4double mylarThickness = 50 * um;
    G4double quartzThickness = 2 * cm;
    G4double pmtThickness = 1 * cm;
    G4double margin = 0.1 * mm;
    G4double airGapThickness = 0.1 * mm; // 100 μm gap where SYLGARD is meant to be used


    G4double worldSize = 2.5 * m;
    auto* solidWorld = new G4Box("World", worldSize, worldSize, worldSize);
    auto* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    auto* physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

//steel (probable error source) 
   auto* solidTube = new G4Tubs("SteelTube",
                             tubeRadius - tubeThickness, // Inner radius
                             tubeRadius,                 // Outer radius
                             tubeLength / 2,
                             0, 360 * deg);
   auto* logicTube = new G4LogicalVolume(solidTube, steel, "SteelTube");
   new G4PVPlacement(nullptr, {}, logicTube, "SteelTube", logicWorld, false, 0);

//mylar
   auto* solidMylar = new G4Tubs("Mylar",
                              tubeRadius - tubeThickness - mylarThickness,
                              tubeRadius - tubeThickness,
                              tubeLength / 2,
                              0, 360 * deg);
   auto* logicMylar = new G4LogicalVolume(solidMylar, mylar, "MylarLV");
   new G4PVPlacement(nullptr, {}, logicMylar, "MylarPV", logicWorld, false, 0);

   // Adding reflectivity
   auto* mylarSurface = new G4OpticalSurface("MylarSurface");
   mylarSurface->SetType(dielectric_metal);
   mylarSurface->SetModel(unified);
   mylarSurface->SetFinish(polished);

   G4double reflectivity[n] = {0.99, 0.99};
   auto* reflectMPT = new G4MaterialPropertiesTable();
   reflectMPT->AddProperty("REFLECTIVITY", photonE, reflectivity, n);
   mylarSurface->SetMaterialPropertiesTable(reflectMPT);
   new G4LogicalSkinSurface("MylarSkin", logicMylar, mylarSurface);


//gas volume(solid,logocal and placement/phy)

   auto* solidGas = new G4Tubs("GasVolume",
                            0,
                            tubeRadius - tubeThickness - mylarThickness - margin,
                            tubeLength / 2,
                            0, 360 * deg);
   auto* logicGas = new G4LogicalVolume(solidGas, gas, "GasLV");
   //new G4PVPlacement(nullptr, {}, logicGas, "GasPV", logicWorld, false, 0);
   G4VPhysicalVolume* physGas = new G4PVPlacement(nullptr, {}, logicGas, "GasPV", logicWorld, false, 0, true); //for ^^^^^^^^^^^^^^^


//to visualize
//logicTube->SetVisAttributes(new G4VisAttributes(G4Colour(0.6,0.6,0.6))); // Gray steel
//logicMylar->SetVisAttributes(new G4VisAttributes(G4Colour(0,1,0)));      // Green Mylar
//logicGas->SetVisAttributes(new G4VisAttributes(G4Colour(0,0,1, 0.3)));   // Transparent blue gas

auto* gasVis = new G4VisAttributes(G4Colour(0, 0, 1, 0.3));
gasVis->SetForceSolid(true);  // Ensures filled rendering with transparency
logicGas->SetVisAttributes(gasVis);



//cap
    auto* capSolid = new G4Tubs("Cap", 0, tubeRadius + tubeThickness, tubeThickness / 2, 0, 360 * deg);
    auto* capLogic = new G4LogicalVolume(capSolid, steel, "CapLogical");
    new G4PVPlacement(0, G4ThreeVector(0, 0, tubeLength / 2 + tubeThickness / 2), capLogic, "Cap", logicWorld, false, 0);


//quartz window
    auto* solidQuartz = new G4Tubs("Quartz", 0, tubeRadius - tubeThickness, quartzThickness / 2, 0, 360 * deg);
    auto* logicQuartz = new G4LogicalVolume(solidQuartz, quartz, "QuartzLV");
    G4ThreeVector quartzPos(0, 0, -tubeLength / 2 - quartzThickness / 2);
    new G4PVPlacement(nullptr, quartzPos, logicQuartz, "QuartzPV", logicWorld, false, 0);
    
    
// Air gap
    auto* solidGap = new G4Tubs("AirGap", 0, tubeRadius - tubeThickness, airGapThickness / 2, 0, 360 * deg);
    auto* logicGap = new G4LogicalVolume(solidGap, air, "GapLV");
    G4ThreeVector gapPos(0, 0, -tubeLength / 2 - quartzThickness - airGapThickness / 2);
    new G4PVPlacement(nullptr, gapPos, logicGap, "GapPV", logicWorld, false, 0);
    
    
//sensitive layer
    auto* solidPMT = new G4Tubs("PMT", 0, 2.4 * cm , pmtThickness / 2, 0, 360 * deg); //5.5 * cm  as 3rd arg
    auto* logicPMT = new G4LogicalVolume(solidPMT, silicon, "PMTLV");
    G4ThreeVector pmtPos(0, 0, -tubeLength / 2 - quartzThickness - airGapThickness - pmtThickness / 2);
    new G4PVPlacement(nullptr, pmtPos, logicPMT, "PMT", logicWorld, false, 0);
    
    // Create visual attributes
    auto* siliconVis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); // Red color (RGB)
    siliconVis->SetForceSolid(true); // make it solid; use SetForceWireframe(false) if needed

// Apply to PMT logical volume
    logicPMT->SetVisAttributes(siliconVis);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////attempt to add QE
    // Create optical surface
/*	auto* pmtSurface = new G4OpticalSurface("PMTSurface");
	pmtSurface->SetType(dielectric_metal);
	pmtSurface->SetModel(unified);
	pmtSurface->SetFinish(polished);
	
	// Load QE from external CSV
	auto* qeMPT = LoadQuantumEfficiencyFromCSV("QE_Curves_Q_1nm.csv");
	
	if (qeMPT) {
  		pmtSurface->SetMaterialPropertiesTable(qeMPT);
	} else {
    		G4cerr << "Failed to attach QE MPT to PMT surface" << G4endl;
		}

	// Define photon energy array
	const G4int nums = 2; 
	G4double photonEnergy[nums] = {1.239841939 * eV / 0.4, 1.239841939 * eV / 0.3}; // ~310 nm to ~410 nm
	
	// Example QE values (adjust as per real curve)
	//G4double efficiency[nums] = {0.2, 0.3}; // 20% at ~310 nm, 30% at ~410 nm

	// Create MPT
	auto* pmtMPT = new G4MaterialPropertiesTable();
	pmtMPT->AddProperty("EFFICIENCY", photonEnergy, efficiency, nums);

	// Attach to optical surface
	pmtSurface->SetMaterialPropertiesTable(pmtMPT);

	// Attach surface to PMT logical volume
	new G4LogicalSkinSurface("PMTSkinSurface", logicPMT, pmtSurface);
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////

//scintillators addition in setup
     G4ThreeVector cylinderCenter = G4ThreeVector(0, 0, 0);
     G4double cylinderRadius = 10.0*cm;   
     G4double cylinderHeight = 180.0*cm; 

     BookScintillatorArray* bookArray = new BookScintillatorArray(logicWorld, cylinderCenter, cylinderRadius, cylinderHeight);
     bookArray->Construct();
    //auto* scintMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    //BookScintillatorArray* bookArray = new BookScintillatorArray(logicWorld, scintMat);
    
    
//conical reflector ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     G4double coneHeight = 70.0 * mm;//70.0 * mm;
     G4double defaultOffsetFromGasStart = 10.0 * mm; // adjustable if you want flush placement
     G4double zConeInWorld = -tubeLength/2 + coneHeight/2; //+ defaultOffsetFromGasStart;
     G4ThreeVector conePos(0, 0, zConeInWorld);
     new ConicalReflector(logicGas, physGas, conePos);
////////////////////////////////////////////////////////////////////^^^^^^^^^^^^^^^^^
    return physWorld;
}

void DetectorConstruction::ConstructSDandField() {

    auto* sdManager = G4SDManager::GetSDMpointer();
    
    //for PMT
    auto* sd = new SensitiveDetector("PMT_SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    //sdManager->AddNewDetector(pmtSD);			//used when we are using 2 different sd volumes(pmt and scint)
    //SetSensitiveDetector("PMTLV", pmtSD);
    
    
    //SetSensitiveDetector("PMTLV", sd);
    //for scint
    //auto* scintSD = new SensitiveDetector("Scint_SD");
    //sdManager->AddNewDetector(scintSD);
    //SetSensitiveDetector(bookArray->GetScintillatorLogicalVolume(), scintSD);
}


//now we need to get an Idea of the focussing conical arrangement
