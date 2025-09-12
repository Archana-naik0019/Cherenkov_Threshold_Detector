#include "RefractiveIndexModel.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <vector>
#include <algorithm>

// Reference temperature for the table
static const double table_T0 = 293.15 * kelvin;

// Raw table: (Pressure in atm, n value)
// You can extend this table if more points are given
static const std::vector<std::pair<double,double>> refrIndexTable = {
    {0.25, 1.000068528},
    {0.50, 1.00013707},
    {0.75, 1.000205626},
    {1.00, 1.000274196},
    {1.25, 1.000342781},
    {1.50, 1.000411379},
    {1.75, 1.000479992},
    {2.00, 1.000548618},
    {2.25, 1.000617259},
    {2.50, 1.000685914},
    {2.75, 1.000754583},
    {3.00, 1.000823266},
    {3.25, 1.000891964},
    {3.50, 1.000960675},
    {3.75, 1.001029400},
    {4.00, 1.001098140},
    {4.25, 1.001166894},
    {4.50, 1.001235662},
    {4.75, 1.001304444},
    {5.00, 1.001373240}
};

// Linear interpolation helper
static double interpolate(double x, const std::vector<std::pair<double,double>>& table) {
    if (x <= table.front().first) return table.front().second;
    if (x >= table.back().first) {
        // simple linear extrapolation beyond last point
        double x0 = table[table.size()-2].first;
        double y0 = table[table.size()-2].second;
        double x1 = table.back().first;
        double y1 = table.back().second;
        return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
    }

    for (size_t i=1; i<table.size(); ++i) {
        if (x < table[i].first) {
            double x0 = table[i-1].first;
            double y0 = table[i-1].second;
            double x1 = table[i].first;
            double y1 = table[i].second;
            return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
        }
    }
    return table.back().second; // fallback
}

G4double RefractiveIndex_FromPressureAndTemp(G4double pressure, G4double temperature) {
    // convert to atm
    double pressure_atm = pressure / atmosphere;

    // interpolate n at T0
    double n_T0 = interpolate(pressure_atm, refrIndexTable);

    // adjust with T0/T scaling for number density
    double delta_n_T0 = n_T0 - 1.0;
    double delta_n = delta_n_T0 * (table_T0 / temperature);

    return 1.0 + delta_n;
}

