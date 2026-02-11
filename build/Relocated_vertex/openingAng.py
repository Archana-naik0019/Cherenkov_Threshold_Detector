import uproot
import numpy as np
import matplotlib.pyplot as plt

# ============================================================
# REFRACTIVE INDEX MODEL (MATCHES YOUR GEANT4 CODE)
# ============================================================

TABLE_T0 = 293.15  # K

refr_index_table = [
    (0.25, 1.000068528),
    (0.50, 1.00013707),
    (0.75, 1.000205626),
    (1.00, 1.000274196),
    (1.25, 1.000342781),
    (1.50, 1.000411379),
    (1.75, 1.000479992),
    (2.00, 1.000548618),
    (2.25, 1.000617259),
    (2.50, 1.000685914),
    (2.75, 1.000754583),
    (3.00, 1.000823266),
    (3.25, 1.000891964),
    (3.50, 1.000960675),
    (3.75, 1.001029400),
    (4.00, 1.001098140),
    (4.25, 1.001166894),
    (4.50, 1.001235662),
    (4.75, 1.001304444),
    (5.00, 1.001373240),
]

def interpolate(x, table):
    if x <= table[0][0]:
        return table[0][1]
    if x >= table[-1][0]:
        x0, y0 = table[-2]
        x1, y1 = table[-1]
        return y0 + (x - x0) * (y1 - y0) / (x1 - x0)

    for i in range(1, len(table)):
        if x < table[i][0]:
            x0, y0 = table[i-1]
            x1, y1 = table[i]
            return y0 + (x - x0) * (y1 - y0) / (x1 - x0)

    return table[-1][1]

def refractive_index_from_pressure_and_temp(p_atm, T):
    n_T0 = interpolate(p_atm, refr_index_table)
    delta_n = (n_T0 - 1.0) * (TABLE_T0 / T)
    return 1.0 + delta_n


# ============================================================
# USER INPUTS (ONLY CHANGE THESE)
# ============================================================

hist_name   = "MuonEnergyWithDetectedCherenkov_4-fold_1PE"
energy_unit = "GeV"
temperature = 293.15     # K
pressure_range = range(2, 17)  # 2 → 16 atm

# Muon mass
M_MUON_MEV = 105.658

# ============================================================
# PLOTTING
# ============================================================

plt.figure(figsize=(9, 7))

for P in pressure_range:

    root_file = f"vertexRelocated_{P}atm.root"

    file = uproot.open(root_file)
    hist = file[hist_name]

    edges = hist.axis().edges()
    centers = 0.5 * (edges[:-1] + edges[1:])
    counts = hist.values()

    energies = np.repeat(centers, counts.astype(int))
    if energy_unit.lower() == "gev":
        energies *= 1000.0

    beta = np.sqrt(1.0 - (M_MUON_MEV / energies) ** 2)

    n = refractive_index_from_pressure_and_temp(P, temperature)

    mask = n * beta > 1.0
    beta = beta[mask]

    if len(beta) == 0:
        print(f"P = {P} atm → below Cherenkov threshold")
        continue

    theta_deg = np.degrees(np.arccos(1.0 / (n * beta)))

    mean_angle = np.mean(theta_deg)

    plt.hist(
        theta_deg,
        bins=100,
        histtype="step",
        linewidth=1.8,
        label=f"{P} atm (⟨θ⟩={mean_angle:.2f}°)"
    )

    print(f"P={P:2d} atm | n={n:.7f} | Mean θc={mean_angle:.3f}°")

# ============================================================
# FINAL PLOT DECORATION
# ============================================================

plt.xlabel("Cherenkov Opening Angle (degrees)")
plt.ylabel("Counts")
plt.title("Cherenkov Opening Angle Distribution for Different Pressures")
plt.legend(fontsize=9)
plt.grid(True)
plt.tight_layout()
plt.show()

