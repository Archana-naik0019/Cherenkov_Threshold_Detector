import uproot
import numpy as np
import matplotlib.pyplot as plt

# ============================================================
# INPUTS
# ============================================================

file_old = "olderVertexPos_2atm.root"
file_new = "vertexRelocated_2atm.root"

hist_name = "theta_mom_4-fold"

# ============================================================
# LOAD HISTOGRAMS
# ============================================================

f_old = uproot.open(file_old)
f_new = uproot.open(file_new)

h_old = f_old[hist_name]
h_new = f_new[hist_name]

edges = h_old.axis().edges()
centers = 0.5 * (edges[:-1] + edges[1:])

counts_old = h_old.values()
counts_new = h_new.values()

centers_t = 180.0 - centers
order = np.argsort(centers_t)

centers_t = centers_t[order]
counts_old = counts_old[order]
counts_new = counts_new[order]

# ============================================================
# STATISTICS FUNCTION
# ============================================================

def hist_stats(centers, counts):
    total = np.sum(counts)

    mean = np.sum(centers * counts) / total
    variance = np.sum(counts * (centers - mean) ** 2) / total
    sigma = np.sqrt(variance)
    mode = centers[np.argmax(counts)]

    return mean, mode, sigma, int(total)

mean_o, mode_o, sigma_o, N_o = hist_stats(centers, counts_old)
mean_n, mode_n, sigma_n, N_n = hist_stats(centers, counts_new)

# ============================================================
# OVERLAY PLOT
# ============================================================

plt.figure(figsize=(8, 6))

plt.step(
    centers,
    counts_old,
    where="mid",
    linewidth=2,
    label=(
        "Older vertex\n"
        f"μ = {mean_o:.2f}°, "
        f"mode = {mode_o:.2f}°, "
        f"σ = {sigma_o:.2f}°, "
        f"N = {N_o}"
    )
)

plt.step(
    centers,
    counts_new,
    where="mid",
    linewidth=2,
    label=(
        "Relocated vertex\n"
        f"μ = {mean_n:.2f}°, "
        f"mode = {mode_n:.2f}°, "
        f"σ = {sigma_n:.2f}°, "
        f"Entries = {N_n}"
    )
)

plt.xlabel("Cherenkov Opening Angle (degrees)")
plt.ylabel("Counts")
plt.title("Overlay: Theta_4-fold (2 atm)")
plt.legend(fontsize=9)
plt.grid(True)
plt.tight_layout()
plt.xlim(right=90)
plt.xlim(left=0)


plt.show()

