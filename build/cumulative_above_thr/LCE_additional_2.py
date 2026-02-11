import uproot
import numpy as np
import matplotlib.pyplot as plt

# ==========================
# FILE & HISTOGRAMS
# ==========================
filename = "vertexRelocated_LCE_16atm.root"

hist_detected = "MuonEnergyWithDetectedCherenkov_4-fold_4PE"
hist_all      = "MuonEnergyforAll_4-fold"

# ==========================
# ENERGY BINS (GeV)
# ==========================
'''
energy_bins = [
    (1.045, 1.075),
    (1.075, 1.126),
    (1.126, 1.167),
    (1.167, 1.215),
    (1.215, 1.285),
    (1.285, 1.336),
    (1.336, 1.425),
    (1.425, 1.515),
    (1.515, 1.625),
    (1.625, 1.786),
    (1.786, 1.935),
    (1.935, 2.176),
    (2.176, 2.536),
    (2.536, 3.115),
    (3.115, None)
]
'''

energy_bins = [
    (1.045, 1.285),
    #(1.075, 1.285),
    #(1.126, 1.285),
    #(1.167, 1.285),
    #(1.215, 1.285),
    (1.285, 1.336),
    (1.336, 1.425),
    (1.425, 1.515),
    (1.515, 1.625),
    (1.625, 1.786),
    (1.786, 1.935),
    (1.935, 2.176),
    (2.176, 2.536),
    (2.536, 3.115),
    (3.115, None)
]

# ==========================
# HELPERS
# ==========================
def entries_in_range(hist, emin, emax):
    values, edges = hist.to_numpy()
    centers = 0.5 * (edges[:-1] + edges[1:])

    if emax is None:
        mask = centers >= emin
    else:
        mask = (centers >= emin) & (centers < emax)

    return values[mask].sum()

# ==========================
# MAIN
# ==========================
x_vals = []
ratios = []

with uproot.open(filename) as f:
    h_det = f[hist_detected]
    h_all = f[hist_all]

    for emin, emax in energy_bins:
        n_det = entries_in_range(h_det, emin, emax)
        n_all = entries_in_range(h_all, emin, emax)

        if n_all == 0:
            continue   # skip empty bins

        ratio = n_det / n_all

        # bin center for plotting
        if emax is None:
            x_center = emin
        else:
            x_center = 0.5 * (emin + emax)

        x_vals.append(x_center)
        ratios.append(ratio)

# ==========================
# PLOT
# ==========================
plt.figure(figsize=(7,5))
plt.plot(x_vals, ratios, marker='o', linewidth=2)

plt.xlabel("Muon Energy (GeV)")
plt.ylabel("Detection Efficiency (≥4 PE)")
plt.title("Cherenkov Detection Efficiency vs Muon Energy\n16 atm")

plt.ylim(0, 1.05)
plt.grid(alpha=0.3)

plt.tight_layout()
plt.show()

