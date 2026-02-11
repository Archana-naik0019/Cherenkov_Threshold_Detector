import uproot
import numpy as np
import matplotlib.pyplot as plt

# ==========================
# USER CONTROLS
# ==========================
file_prefix = "cumulative_"
file_suffix = ".root"

start_i = 4
end_i   = 16
step_i  = 4          # <-- set 2,3,... to skip files
# OR explicitly choose pressures:
# pressures = [2, 4, 6, 8, 10, 12, 14, 16]

hist_detected = "MuonEnergyWithDetectedCherenkov_4-fold_4PE"
hist_all      = "MuonEnergyforAll_4-fold"

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

# ==========================
# FUNCTIONS
# ==========================
def entries_in_range(hist, xmin, xmax):
    values, edges = hist.to_numpy()
    centers = 0.5 * (edges[:-1] + edges[1:])

    if xmax is None:
        mask = centers >= xmin
    else:
        mask = (centers >= xmin) & (centers < xmax)

    return values[mask].sum()

# ==========================
# MAIN
# ==========================
plt.figure(figsize=(9, 6))

pressures = range(start_i, end_i + 1, step_i)

for p in pressures:
    filename = f"{file_prefix}{p}{file_suffix}"

    try:
        with uproot.open(filename) as f:
            h_det = f[hist_detected]
            h_all = f[hist_all]

            x_vals = []
            y_vals = []

            for emin, emax in energy_bins:
                n_det = entries_in_range(h_det, emin, emax)
                n_all = entries_in_range(h_all, emin, emax)

                if n_all > 0:
                    ratio = n_det / n_all
                    if emax is None:
                        ecenter = emin + 0.1  # safe dummy for last bin
                    else:
                        ecenter = 0.5 * (emin + emax)

                    x_vals.append(ecenter)
                    y_vals.append(ratio)

            if len(x_vals) > 0:
                plt.plot(
                    x_vals,
                    y_vals,
                    marker='o',
                    linestyle='-',
                    label=f"{p} atm"
                )

    except Exception as e:
        print(f"Skipping {filename}: {e}")

# ==========================
# PLOT AESTHETICS
# ==========================
plt.xlabel("Muon Energy")
plt.ylabel("Detection Ratio (≥4 PE)")
plt.title("Cherenkov Detection Ratio vs Energy (All Pressures)")

plt.grid(alpha=0.3)
plt.legend(fontsize=9, ncol=2)
plt.tight_layout()

plt.show()

