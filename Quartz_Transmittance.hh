#ifndef QUARTZ_TRANSMITTANCE_HH
#define QUARTZ_TRANSMITTANCE_HH

#include "G4MaterialPropertiesTable.hh"
#include "globals.hh"
#include <string>

// Load quartz transmittance CSV and convert to ABSLENGTH
// thickness_cm: physical thickness of quartz in cm
G4MaterialPropertiesTable*
LoadQuartzTransmittanceCSV(
    const std::string& filename,
    G4double thickness_cm,
    G4double lambdaMin_nm = 100.0,
    G4double lambdaMax_nm = 900.0,
    G4int    nPoints      = 200
);

#endif

