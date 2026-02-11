import uproot
import numpy as np

# ==========================
# USER CONTROLS
# ==========================
file_prefix = "cumulative_"
file_suffix = ".root"

start_i = 2
end_i   = 16
step_i  = 1   # change to 2,3,... if you want to skip files

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
# FUNCTION
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
# MAIN LOOP
# ==========================
for i in range(start_i, end_i + 1, step_i):

    filename = f"{file_prefix}{i}{file_suffix}"
    print(f"\n===== File: {filename} =====")

    try:
        with uproot.open(filename) as f:
            h_det = f[hist_detected]
            h_all = f[hist_all]

            for (emin, emax) in energy_bins:
                n_det = entries_in_range(h_det, emin, emax)
                n_all = entries_in_range(h_all, emin, emax)

                if n_all > 0:
                    ratio = n_det / n_all
                    ratio_str = f"{ratio:.4f}"
                else:
                    ratio_str = "NA"

                emax_str = f"{emax:.3f}" if emax is not None else "max"

                print(
                    f"E ∈ [{emin:.3f}, {emax_str}]  "
                    f"N_det = {int(n_det):6d}  "
                    f"N_all = {int(n_all):6d}  "
                    f"Ratio = {ratio_str}"
                )

    except FileNotFoundError:
        print("File not found — skipped")
    except KeyError as e:
        print(f"Missing histogram {e} — skipped")

