import numpy as np
import matplotlib.pyplot as plt

# =============================
# Pressure arrays
# =============================
P_old = np.array([2, 3, 4, 5, 6, 7, 8, 9, 10], dtype=float)
P_new = np.array([2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16], dtype=float)

# =============================
# OLD (spreadsheet data)
# =============================
eff_old = np.array([
    0.5136612022,
    0.5069767442,
    0.5128205128,
    0.5059760956,
    0.5,
    0.5,
    0.4946996466,
    0.4761904762,
    0.4697986577
])

N4_old = np.array([
    183, 215, 234, 251, 268,
    280, 283, 294, 298
])

# =============================
# NEW (full 2–16 atm)
# =============================
eff_new = np.array([
    0.6804733728,
    0.6633663366,
    0.6484018265,
    0.6506550218,
    0.6502057613,
    0.7288888889,
    0.6425855513,
    0.63099631,
    0.6308243728,
    0.6312056738,
    0.6293706294,
    0.6224489796,
    0.6208053691,
    0.6158940397,
    0.6098360656
])

N4_new = np.array([
    169, 202, 219, 229, 243,
    225, 263, 271, 279, 282,
    286, 294, 298, 302, 305
])

# =============================
# Binomial errors
# =============================
err_old = np.sqrt(eff_old * (1 - eff_old) / N4_old)
err_new = np.sqrt(eff_new * (1 - eff_new) / N4_new)

# =============================
# Mean efficiencies
# =============================
mean_old = np.mean(eff_old)
mean_new = np.mean(eff_new)

# =============================
# Plot
# =============================
plt.figure(figsize=(8, 6))

plt.errorbar(
    P_old, eff_old, yerr=err_old,
    fmt='o-', capsize=3,
    label=f'at 10 atm (2–10 atm), mean = {mean_old:.4f}'
)

plt.errorbar(
    P_new, eff_new, yerr=err_new,
    fmt='s-', capsize=3,
    label=f'at 16 atm (2–16 atm), mean = {mean_new:.4f}'
)

plt.xlabel("Pressure (atm)")
plt.ylabel("Light Collection Efficiency")
plt.title("Light Collection Efficiency (Binomial Errors)")
plt.grid(True, alpha=0.3)
plt.legend(fontsize=9)

plt.tight_layout()
plt.show()

