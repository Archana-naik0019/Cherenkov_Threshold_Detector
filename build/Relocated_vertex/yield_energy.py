import uproot
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter1d

# --- Open ROOT file ---
file_path = "vertexRelocated_14atm.root"
with uproot.open(file_path) as f:
    hist = f["Energy_vs_Yield"]

    # --- Get bin edges and contents ---
    x_edges = hist.axis(0).edges()
    y_edges = hist.axis(1).edges()
    contents = hist.values()  # shape (nx, ny)

    # --- Bin centers ---
    x_centers = 0.5 * (x_edges[:-1] + x_edges[1:])
    y_centers = 0.5 * (y_edges[:-1] + y_edges[1:])

    # --- Compute average Yield per Energy bin, ignoring Yield < 1.5 ---
    avg_yield = np.zeros(len(x_centers))
    for i in range(len(x_centers)):
        col = contents.T[:, i]  # counts for this Energy bin
        mask = (y_centers >= 1.5) & (col > 0)  # only Yield >=1.5 and nonzero counts
        if np.sum(col[mask]) > 0:
            avg_yield[i] = np.average(y_centers[mask], weights=col[mask])
        else:
            avg_yield[i] = 0

# --- Meshgrid for plotting ---
X, Y = np.meshgrid(x_edges, y_edges)

# --- Plot 2D histogram ---
plt.figure(figsize=(8,6))
c = plt.pcolormesh(X, Y, contents.T, cmap="viridis", shading='auto', vmin=0, vmax=20)
cb = plt.colorbar(c)
cb.set_label("Counts", fontsize=12)

# --- Apply smoothing to the full data ---
smooth_avg = gaussian_filter1d(avg_yield, sigma=5)

# --- Overlay average trend line ---
energy_mask = x_centers >= 1.2

# Apply the mask to x and smooth_avg
x_centers_masked = x_centers[energy_mask]
smooth_avg_masked = smooth_avg[energy_mask]  # This is the key fix!

plt.plot(x_centers_masked, smooth_avg_masked, color='red', linewidth=2, 
         label='Smoothed Average Yield (E ≥ 1.1 GeV)')

# --- Labels, title, limits ---
plt.xlabel("Energy (GeV)", fontsize=12)
plt.ylabel("Yield (per mm muon path)", fontsize=12)
plt.title("Energy vs Yield", fontsize=14)
plt.ylim(0, 50)
plt.legend()
plt.tight_layout()
plt.show()
