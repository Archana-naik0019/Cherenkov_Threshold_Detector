import uproot
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter1d

# =========================
# USER CONTROLS
# =========================
atm_start = 8
atm_end   = 16
atm_step  = 2

yield_cut = 1.5
smooth_sigma = 3

# Pressure-dependent energy thresholds (EDIT THESE)
energy_cuts = {
    2:  3.12,
    3:  2.6,
    4:  2.2,
    5:  1.96,
    6:  1.8,
    7:  1.7,
    8:  1.6,
    9:  1.45,
    10: 1.35,
    11: 1.3,
    12: 1.25,
    13: 1.2,
    14: 1.15,
    15: 1.1,
    16: 1.08
}

# =========================
# PLOT SETUP
# =========================
plt.figure(figsize=(9,6))

# =========================
# LOOP OVER FILES
# =========================
for atm in range(atm_start, atm_end + 1, atm_step):

    file_path = f"vertexRelocated_{atm}atm.root"
    print(f"Processing {file_path}")

    try:
        with uproot.open(file_path) as f:
            hist = f["Energy_vs_Yield"]

            x_edges = hist.axis(0).edges()
            y_edges = hist.axis(1).edges()
            contents = hist.values()

            x_centers = 0.5 * (x_edges[:-1] + x_edges[1:])
            y_centers = 0.5 * (y_edges[:-1] + y_edges[1:])

            # =========================
            # Avg yield vs energy
            # =========================
            avg_yield = np.zeros(len(x_centers))
            for i in range(len(x_centers)):
                col = contents.T[:, i]
                mask = (y_centers >= yield_cut) & (col > 0)
                if np.sum(col[mask]) > 0:
                    avg_yield[i] = np.average(y_centers[mask], weights=col[mask])
                else:
                    avg_yield[i] = np.nan

            # =========================
            # Pressure-dependent energy cut
            # =========================
            e_cut = energy_cuts.get(atm, 1.0)   # fallback default
            e_mask = x_centers >= e_cut

            x_use = x_centers[e_mask]
            y_use = avg_yield[e_mask]

            # =========================
            # Smooth curve
            # =========================
            y_smooth = gaussian_filter1d(y_use, sigma=smooth_sigma)

            # =========================
            # Plot
            # =========================
            plt.plot(x_use, y_smooth, linewidth=2, label=f"{atm} atm")
            #plt.plot(x_use, y_smooth, linewidth=2, label=f"{atm} atm  (E>{e_cut:.2f})")

    except Exception as e:
        print(f"Skipping {file_path}: {e}")

# =========================
# FINAL PLOT STYLING
# =========================
plt.xlabel("Energy (GeV)", fontsize=12)
plt.ylabel("Average Cherenkov Yield (per mm)", fontsize=12)
plt.title("Cherenkov Yield vs Energy at Different Pressures", fontsize=14)

plt.xlim(0, 8)
plt.ylim(0, 50)
plt.legend(title="Pressure")
plt.grid(alpha=0.3)
plt.tight_layout()
plt.show()
