# as of dec 1 2025, this is the functional code

from lecroyscope import Trace
import matplotlib.pyplot as plt
import numpy as np
#from Overlay_phot_dist_func import compare_pe_with_geant4

def calculate_pulse_amplitude(voltage, baseline_bins=30):
    """Calculate pulse amplitude with baseline subtraction"""
    baseline_voltage = voltage[:baseline_bins]
    baseline_mean = np.mean(baseline_voltage)
    baseline_std = np.std(baseline_voltage)
    peak_voltage = np.min(voltage)
    amplitude = peak_voltage - baseline_mean
    snr = abs(amplitude) / baseline_std if baseline_std > 0 else 0
    return amplitude, baseline_mean, baseline_std, snr

# Initialize counters
four_fold_events = 0      # Events with signal in Channel 3 (4-fold)
five_fold_events = 0      # Events with signal in Channel 4 (5-fold)
cherenkov_amplitudes = [] # Cherenkov signal amplitudes (in photoelectrons)

# Define multiple PE thresholds to test
pe_thresholds = [1.0, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0]
five_fold_counts = {pe: 0 for pe in pe_thresholds}

# Thresholds
FOUR_FOLD_THRESHOLD = 0.3  # threshold for channel 3
FIVE_FOLD_THRESHOLD = 0.002 # threshold for channel 4, Redundant as the value is below what we define for 1 PE equivalent

# Convert 1 p.e. to volts (1 p.e. = 3.002mV)
ONE_PHOTOELECTRON_V = 0.003002  # @1050V bais

for i in range(2, 2914):
    # FILE PATH SETUP
    base_path = "/eos/user/a/arnaik/Cherenkov_data_dec2025/20251204_11BAR/"
    file1 = base_path + "C1--cherenkov--"
    file2 = base_path + "C2--cherenkov--" 
    file3 = base_path + "C3--cherenkov--"  # Channel 3: 4-fold coincidence (scintillators)
    file4 = base_path + "C4--cherenkov--"  # Channel 4: 5-fold coincidence (scintillators + Cherenkov)

    # Your existing file numbering logic
    if (i > 0) and (i < 10):
        suffix = "0000" + str(i) + ".trc"
    elif (i > 9) and (i < 100):
        suffix = "000" + str(i) + ".trc"
    elif (i > 99) and (i < 1000):
        suffix = "00" + str(i) + ".trc"
    elif (i > 999) and (i < 10000):
        suffix = "0" + str(i) + ".trc"
    else:
        suffix = str(i) + ".trc"
    
    # Complete file paths
    file1 += suffix
    file2 += suffix
    file3 += suffix
    file4 += suffix
    
    # Load traces
    try:
        trace1 = Trace(file1)
        trace2 = Trace(file2) 
        trace3 = Trace(file3)  # Channel 3: 4-fold coincidence (scintillators)
        trace4 = Trace(file4)  # Channel 4: 5-fold coincidence (scintillators + Cherenkov)

        # time values
        time1 = trace1.time  # trace.x is an alias for trace.time
        time2 = trace2.time
        time3 = trace3.time
        time4 = trace4.time
        
        # Get voltage arrays
        v1 = np.array(trace1.voltage)
        v2 = np.array(trace2.voltage)
        v3 = np.array(trace3.voltage)
        v4 = np.array(trace4.voltage)

        # Calculate amplitudes with baseline subtraction
        amp3, base3, std3, snr3 = calculate_pulse_amplitude(v3)  # 4-fold channel
        amp4, base4, std4, snr4 = calculate_pulse_amplitude(v4)  # 5-fold channel

        # Convert to positive amplitudes
        amp3_pos = -amp3  # 4-fold signal amplitude
        amp4_pos = -amp4  # 5-fold signal amplitude

        # Check for 4-fold events (Channel 3 above threshold)
        if amp3_pos > FOUR_FOLD_THRESHOLD:
            four_fold_events += 1
            
            # Convert to photoelectrons
            cherenkov_pe = amp4_pos / ONE_PHOTOELECTRON_V
            
            # Store all Cherenkov amplitudes
            cherenkov_amplitudes.append(cherenkov_pe)
            
            # Check for 5-fold events (Channel 4 above threshold)
            if amp4_pos > FIVE_FOLD_THRESHOLD:
                five_fold_events += 1
                
                # Check for each PE threshold
                for pe_threshold in pe_thresholds:
                    if cherenkov_pe >= pe_threshold:
                        five_fold_counts[pe_threshold] += 1
                
                print(f"Event {i}: 5-fold | {cherenkov_pe:.1f} PE")
            else:
                print(f"Event {i}: 4-fold only | {cherenkov_pe:.1f} PE")
    
    except FileNotFoundError:
        print(f"Warning: Files for event {i} not found, skipping...")
        continue
    except Exception as e:
        print(f"Error processing event {i}: {e}")
        continue

# Calculate ratios for all thresholds
efficiency_ratios = {}
if four_fold_events > 0:
    efficiency_ratio_no_cut = five_fold_events / four_fold_events
    for pe_threshold in pe_thresholds:
        efficiency_ratios[pe_threshold] = five_fold_counts[pe_threshold] / four_fold_events
else:
    efficiency_ratio_no_cut = 0
    for pe_threshold in pe_thresholds:
        efficiency_ratios[pe_threshold] = 0

print(f"\n" + "="*70)
print("COMPREHENSIVE RESULTS")
print("="*70)
print(f"TOTAL 4-FOLD EVENTS: {four_fold_events}")
print(f"5-fold events (no PE cut): {five_fold_events}")
print(f"\nEfficiency Ratios for Different PE Thresholds:")
print("-" * 65)
print(f"{'PE Threshold':<8} {'5-fold Events':<14} {'4-fold Events':<14} {'Efficiency':<12} {'% of 4-fold':<12}")
print("-" * 65)
print(f"{'No cut':<8} {five_fold_events:<14} {four_fold_events:<14} {efficiency_ratio_no_cut:.4f}     {efficiency_ratio_no_cut*100:>6.1f}%")

for pe_threshold in pe_thresholds:
    count_5fold = five_fold_counts[pe_threshold]
    count_4fold = four_fold_events
    efficiency = efficiency_ratios[pe_threshold]
    percentage = efficiency * 100
    print(f"{pe_threshold:>4.1f} PE  {count_5fold:<14} {count_4fold:<14} {efficiency:.4f}     {percentage:>6.1f}%")

print("-" * 65)

# Calculate what percentage of 5-fold events pass each cut
print(f"\nCut Efficiency (Percentage of 5-fold events that pass each cut):")
print("-" * 50)
print(f"{'PE Threshold':<8} {'Passed':<10} {'Total 5-fold':<12} {'% of 5-fold':<12}")
print("-" * 50)
for pe_threshold in pe_thresholds:
    count_passed = five_fold_counts[pe_threshold]
    total_5fold = five_fold_events
    if five_fold_events > 0:
        percentage_of_5fold = (count_passed / five_fold_events) * 100
    else:
        percentage_of_5fold = 0
    print(f"{pe_threshold:>4.1f} PE  {count_passed:<10} {total_5fold:<12} {percentage_of_5fold:>6.1f}%")

print("-" * 50)

# Calculate what percentage of 4-fold events pass each cut
print(f"\nPercentage of 4-fold events above each PE threshold:")
print("-" * 55)
print(f"{'PE Threshold':<8} {'Above Thresh':<12} {'Total 4-fold':<12} {'% of 4-fold':<12}")
print("-" * 55)
if cherenkov_amplitudes:
    arr_pe = np.array(cherenkov_amplitudes)
    for pe_threshold in pe_thresholds:
        above_threshold = np.sum(arr_pe >= pe_threshold)
        percentage_of_4fold = (above_threshold / len(arr_pe)) * 100
        print(f"{pe_threshold:>4.1f} PE  {above_threshold:<12} {len(arr_pe):<12} {percentage_of_4fold:>6.1f}%")
else:
    for pe_threshold in pe_thresholds:
        print(f"{pe_threshold:>4.1f} PE  {0:<12} {0:<12} {0:>6.1f}%")

print("-" * 55)

if cherenkov_amplitudes:
    arr_pe = np.array(cherenkov_amplitudes)
    
    print(f"\nCHERENKOV SIGNAL STATISTICS:")
    print(f"Total 4-fold events analyzed: {len(arr_pe)}")
    print(f"Mean amplitude: {np.mean(arr_pe):.1f} PE")
    print(f"Median amplitude: {np.median(arr_pe):.1f} PE")
    print(f"Standard deviation: {np.std(arr_pe):.1f} PE")
    print(f"Range: {np.min(arr_pe):.1f} to {np.max(arr_pe):.1f} PE")
    
    print(f"\nEvents above each PE threshold (from 4-fold events):")
    for pe_threshold in pe_thresholds:
        above_threshold = np.sum(arr_pe >= pe_threshold)
        percentage = (above_threshold / len(arr_pe)) * 100
        print(f"  ≥{pe_threshold:>3.1f} PE: {above_threshold:>4d}/{len(arr_pe)} ({percentage:>5.1f}%)")

#========================================================
        
    # Creating plots
    plt.figure(figsize=(20, 6))    
    
    # Plot 1: Cherenkov signal distribution with all thresholds
    plt.subplot(1, 3, 1)
    n, bins, patches = plt.hist(arr_pe, bins=40, alpha=0.7, edgecolor='black')
    
    # Add vertical lines for all thresholds
    colors = plt.cm.viridis(np.linspace(0, 1, len(pe_thresholds)))
    for i, pe_threshold in enumerate(pe_thresholds):
        plt.axvline(pe_threshold, color=colors[i], linestyle='--', 
                   alpha=0.8, label=f'{pe_threshold} PE')
    
    plt.xlabel('Cherenkov Signal [photoelectrons]')
    plt.ylabel('Number of Events')
    plt.title(f'Cherenkov Signal Distribution\n(Total 4-fold events: {len(arr_pe)})')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.3)
    
    # Plot 2: Efficiency vs PE threshold
    plt.subplot(1, 3, 2)
    thresholds = ['No cut'] + [f'{pe} PE' for pe in pe_thresholds]
    efficiencies = [efficiency_ratio_no_cut*100] + [efficiency_ratios[pe]*100 for pe in pe_thresholds]
    
    plt.plot(thresholds, efficiencies, 'o-', linewidth=2, markersize=8)
    plt.xlabel('PE Threshold')
    plt.ylabel('Efficiency (%)')
    plt.title(f'Efficiency vs PE Threshold\n(% of {four_fold_events} 4-fold events)')
    plt.xticks(rotation=45)
    plt.grid(True, alpha=0.3)
    
    # Add value labels on points
    for i, (th, eff) in enumerate(zip(thresholds, efficiencies)):
        plt.annotate(f'{eff:.1f}%', (i, eff), textcoords="offset points", 
                    xytext=(0,10), ha='center')
    
    # Plot 3: Number of events passing each cut
    plt.subplot(1, 3, 3)
    events_passing = [five_fold_events] + [five_fold_counts[pe] for pe in pe_thresholds]
    
    plt.plot(thresholds, events_passing, 's-', color='red', linewidth=2, markersize=8)
    plt.xlabel('PE Threshold')
    plt.ylabel('Number of 5-fold Events')
    plt.title(f'Events Passing Each Cut\n(Total 4-fold: {four_fold_events})')
    plt.xticks(rotation=45)
    plt.grid(True, alpha=0.3)
    
    # Add value labels on points
    for i, (th, count) in enumerate(zip(thresholds, events_passing)):
        plt.annotate(f'{count}', (i, count), textcoords="offset points", 
                    xytext=(0,10), ha='center')
    
    plt.tight_layout()
#    plt.show()
    #plt.savefig("20bin_11BAR.pdf", bbox_inches='tight')
    plt.savefig("20bin_11BAR.png", dpi=300, bbox_inches='tight')

#============================================================
   
    
    # Print summary table
print(f"\n" + "="*70)
print("SUMMARY TABLE (for easy copying)")
print("="*70)
print("PE_Threshold,4_fold_Events,5_fold_Events,Efficiency_Percent")
print(f"No_cut,{four_fold_events},{five_fold_events},{efficiency_ratio_no_cut*100:.2f}")
for pe_threshold in pe_thresholds:
    print(f"{pe_threshold},{four_fold_events},{five_fold_counts[pe_threshold]},{efficiency_ratios[pe_threshold]*100:.2f}")

# Final summary
print(f"\n" + "="*70)
print("FINAL SUMMARY")
print("="*70)
print(f"Total 4-fold events: {four_fold_events}")
print(f"Total 5-fold events (no cut): {five_fold_events}")
print(f"Overall efficiency (no cut): {efficiency_ratio_no_cut*100:.1f}%")
print(f"Recommended PE threshold based on your analysis: 4.0 PE")
print(f"Efficiency at 4.0 PE: {efficiency_ratios[4.0]*100:.1f}%")
print("="*70)

'''
# calling fuction for the overlay of data sim photon dist
compare_pe_with_geant4(
    exp_pe = arr_pe,
    geant4_root_file = "Corrected_bin0_photDist/CorrectedOutput_15.root",
    hist_name = "hDetected_corrected_15",
    output_name = "unnormalized_6dec_15bar_overlay_python_vs_geant4_corrected.png",
    normalize = False
)
'''
