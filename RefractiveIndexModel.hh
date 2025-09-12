#ifndef REFRACTIVE_INDEX_MODEL_HH
#define REFRACTIVE_INDEX_MODEL_HH

#include "globals.hh"  
#include "G4SystemOfUnits.hh"

// Function to compute refractive index given P, T
G4double RefractiveIndex_FromPressureAndTemp(G4double pressure, G4double temperature = 293.15*kelvin);

#endif

