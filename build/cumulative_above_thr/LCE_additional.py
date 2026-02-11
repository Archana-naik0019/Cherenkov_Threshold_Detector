import uproot
import numpy as np

# ==========================
# FILE & HISTOGRAMS
# ==========================
filename = "vertexRelocated_LCE_16atm.root"

hist_detected = "MuonEnergyWithDetectedCherenkov_4-fold_4PE"
hist_all      = "MuonEnergyforAll_4-fold"

# ==========================
# ENERGY BINS (GeV)
# ==========================
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
    (3.115, None)   # up to max
]

# ==========================
# HELPER
# ==========================
def entries_in_range(hist, emin, emax):
    values, edges = hist.to_numpy()
    centers = 0.5 * (edges[:-1] + edges[1:])

    if emax is None:
        mask = centers >= emin
    else:
        mask = (centers >= emin) & (centers < emax)

    return int(values[mask].sum())

# ==========================
# MAIN
# ==========================
with uproot.open(filename) as f:
    h_det = f[hist_detected]
    h_all = f[hist_all]

    print("\nEnergy range (GeV) | Detected | All | Ratio")
    print("-" * 55)

    for emin, emax in energy_bins:
        n_det = entries_in_range(h_det, emin, emax)
        n_all = entries_in_range(h_all, emin, emax)

        ratio = n_det / n_all if n_all > 0 else 0.0

        if emax is None:
            label = f"{emin:.3f} – max"
        else:
            label = f"{emin:.3f} – {emax:.3f}"

        print(f"{label:18s} | {n_det:8d} | {n_all:6d} | {ratio:.4f}")

