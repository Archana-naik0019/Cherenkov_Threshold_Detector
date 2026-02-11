import uproot
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# ============================================================
# USER INPUTS
# ============================================================

root_file = "vertexRelocated_2atm.root"          # ROOT file
hist_name = "MuonEnergyWithDetectedCherenkov_4-fold_1PE"      # Energy histogram name
#refractive_index = 1.0003         # medium refractive index..needs to be made variable wrt pressure!
energy_unit = "GeV"               # "GeV" or "MeV"

# Muon mass
M_MUON_MEV = 105.658  # MeV

# ============================================================
# LOAD HISTOGRAM
# ============================================================

file = uproot.open(root_file)
hist = file[hist_name]

# bin centers and contents
bin_edges = hist.axis().edges()
bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])
counts = hist.values()

# expand energies according to bin counts
energies = np.repeat(bin_centers, counts.astype(int))

# convert energy to MeV if needed
if energy_unit.lower() == "gev":
    energies *= 1000.0  # GeV → MeV

# ============================================================
# COMPUTE CHERENKOV ANGLES
# ============================================================

beta = np.sqrt(1.0 - (M_MUON_MEV / energies) ** 2)

# Cherenkov condition
mask = refractive_index * beta > 1.0
beta = beta[mask]

cos_theta = 1.0 / (refractive_index * beta)
theta = np.arccos(cos_theta)        # radians
theta_deg = np.degrees(theta)       # degrees

# ============================================================
# STATISTICS
# ============================================================

mean_angle = np.mean(theta_deg)

# Mode via histogram peak
hist_vals, hist_bins = np.histogram(theta_deg, bins=100)
mode_angle = hist_bins[np.argmax(hist_vals)]

print("====================================")
print(f"Mean Cherenkov angle : {mean_angle:.3f} deg")
print(f"Mode Cherenkov angle : {mode_angle:.3f} deg")
print(f"Total photons/events: {len(theta_deg)}")
print("====================================")

# ============================================================
# PLOT
# ============================================================

plt.figure(figsize=(8, 6))
plt.hist(theta_deg, bins=100, histtype="step", linewidth=2)
plt.xlabel("Cherenkov Opening Angle (degrees)")
plt.ylabel("Counts")
plt.title("Cherenkov Opening Angle Distribution_2atm")
plt.grid(True)
plt.tight_layout()
plt.show()
