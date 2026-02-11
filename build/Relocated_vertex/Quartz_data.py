import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import warnings
warnings.filterwarnings('ignore')

def interpolate_extrapolate_csv(input_file, output_file, x_col, y_col, new_x_range, extrapolation_type='linear'):
    """
    Interpolates and extrapolates data from a CSV file
    
    Parameters:
    -----------
    input_file : str
        Path to input CSV file
    output_file : str
        Path to output CSV file
    x_col : str
        Name of x-column in CSV
    y_col : str
        Name of y-column in CSV
    new_x_range : tuple
        (start, stop, num_points) for new x-values
    extrapolation_type : str
        Type of extrapolation: 'linear', 'quadratic', or 'cubic'
    """
    
    # Read the CSV file
    df = pd.read_csv(input_file)
    
    # Extract x and y data
    x = df[x_col].values
    y = df[y_col].values
    
    # Sort by x to ensure proper interpolation
    sort_idx = np.argsort(x)
    x_sorted = x[sort_idx]
    y_sorted = y[sort_idx]
    
    # Remove duplicates in x for interpolation
    x_unique, unique_indices = np.unique(x_sorted, return_index=True)
    y_unique = y_sorted[unique_indices]
    
    # Choose interpolation method based on data points
    if len(x_unique) >= 4 and extrapolation_type == 'cubic':
        kind = 'cubic'
    elif len(x_unique) >= 3 and extrapolation_type == 'quadratic':
        kind = 'quadratic'
    else:
        kind = 'linear'
    
    # Create interpolation function
    interp_func = interpolate.interp1d(x_unique, y_unique, kind=kind, 
                                       fill_value='extrapolate', 
                                       bounds_error=False)
    
    # Generate new x values
    new_x = np.linspace(new_x_range[0], new_x_range[1], new_x_range[2])
    
    # Get interpolated/extrapolated y values
    new_y = interp_func(new_x)
    
    # Create output DataFrame
    output_df = pd.DataFrame({
        x_col: new_x,
        y_col: new_y
    })
    
    # Save to CSV
    output_df.to_csv(output_file, index=False)
    
    # Plot the data
    plot_data(df, output_df, x_col, y_col, input_file, output_file)
    
    return df, output_df

def plot_data(original_df, new_df, x_col, y_col, input_filename, output_filename):
    """
    Plot original and interpolated/extrapolated data
    """
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    
    # Plot 1: Original data
    axes[0, 0].scatter(original_df[x_col], original_df[y_col], color='blue', s=50, label='Original data')
    axes[0, 0].plot(original_df[x_col], original_df[y_col], 'b-', alpha=0.5, label='Original trend')
    axes[0, 0].set_xlabel(x_col, fontsize=12)
    axes[0, 0].set_ylabel(y_col, fontsize=12)
    axes[0, 0].set_title(f'Original Data: {input_filename}', fontsize=14, fontweight='bold')
    axes[0, 0].grid(True, alpha=0.3)
    axes[0, 0].legend()
    
    # Plot 2: Interpolated/Extrapolated data
    axes[0, 1].scatter(new_df[x_col], new_df[y_col], color='red', s=30, label='New data', alpha=0.6)
    axes[0, 1].plot(new_df[x_col], new_df[y_col], 'r-', alpha=0.7, label='Interpolated/Extrapolated')
    axes[0, 1].set_xlabel(x_col, fontsize=12)
    axes[0, 1].set_ylabel(y_col, fontsize=12)
    axes[0, 1].set_title(f'Interpolated/Extrapolated Data: {output_filename}', fontsize=14, fontweight='bold')
    axes[0, 1].grid(True, alpha=0.3)
    axes[0, 1].legend()
    
    # Plot 3: Combined plot
    axes[1, 0].scatter(original_df[x_col], original_df[y_col], color='blue', s=50, label='Original data', zorder=3)
    axes[1, 0].plot(original_df[x_col], original_df[y_col], 'b-', alpha=0.5, label='Original trend')
    axes[1, 0].scatter(new_df[x_col], new_df[y_col], color='red', s=20, label='New data', alpha=0.6, zorder=2)
    axes[1, 0].plot(new_df[x_col], new_df[y_col], 'r-', alpha=0.7, label='Interpolated/Extrapolated')
    axes[1, 0].set_xlabel(x_col, fontsize=12)
    axes[1, 0].set_ylabel(y_col, fontsize=12)
    axes[1, 0].set_title('Original vs Interpolated/Extrapolated Data', fontsize=14, fontweight='bold')
    axes[1, 0].grid(True, alpha=0.3)
    axes[1, 0].legend()
    
    # Plot 4: Data point comparison
    axes[1, 1].bar(['Original', 'New'], [len(original_df), len(new_df)], 
                   color=['blue', 'red'], alpha=0.7)
    axes[1, 1].set_ylabel('Number of Data Points', fontsize=12)
    axes[1, 1].set_title('Data Points Comparison', fontsize=14, fontweight='bold')
    axes[1, 1].grid(True, alpha=0.3, axis='y')
    
    # Add count labels on bars
    for i, v in enumerate([len(original_df), len(new_df)]):
        axes[1, 1].text(i, v + max(len(original_df), len(new_df))/50, str(v), 
                       ha='center', fontweight='bold')
    
    plt.tight_layout()
    plt.show()

def create_sample_csv(filename='sample_data.csv'):
    """Create a sample CSV file for testing"""
    sample_data = {
        'x': [1, 3, 5, 7, 9, 11, 13, 15],
        'y': [2.1, 3.8, 5.2, 7.1, 8.9, 10.5, 12.3, 14.2]
    }
    df = pd.DataFrame(sample_data)
    df.to_csv(filename, index=False)
    print(f"Sample CSV created: {filename}")
    return filename

def main():
    """
    Main function to demonstrate the interpolation/extrapolation
    """
    print("="*60)
    print("CSV Interpolation and Extrapolation Tool")
    print("="*60)
    
    # Create a sample CSV if no data is available
    print("\n1. Creating sample data...")
    input_csv = create_sample_csv('sample_data.csv')
    
    # Define parameters
    output_csv = 'interpolated_data.csv'
    x_column = 'x'
    y_column = 'y'
    
    # Define new range: (start, stop, num_points)
    # This will interpolate between existing points and extrapolate beyond
    new_range = (0, 20, 50)  # From 0 to 20 with 50 points
    
    # Extrapolation type: 'linear', 'quadratic', or 'cubic'
    extrap_type = 'cubic'
    
    print("\n2. Processing data...")
    print(f"   Input file: {input_csv}")
    print(f"   Output file: {output_csv}")
    print(f"   X column: {x_column}")
    print(f"   Y column: {y_column}")
    print(f"   New range: {new_range[0]} to {new_range[1]} with {new_range[2]} points")
    print(f"   Extrapolation type: {extrap_type}")
    
    print("\n3. Performing interpolation and extrapolation...")
    original_df, new_df = interpolate_extrapolate_csv(
        input_file=input_csv,
        output_file=output_csv,
        x_col=x_column,
        y_col=y_column,
        new_x_range=new_range,
        extrapolation_type=extrap_type
    )
    
    print(f"\n4. Results:")
    print(f"   Original data points: {len(original_df)}")
    print(f"   New data points: {len(new_df)}")
    print(f"\n   First few original data points:")
    print(original_df.head().to_string(index=False))
    print(f"\n   First few interpolated/extrapolated data points:")
    print(new_df.head().to_string(index=False))
    
    print(f"\n5. Plots displayed above.")
    print(f"\n6. Output saved to: {output_csv}")
    
    # Show summary statistics
    print("\n" + "="*60)
    print("SUMMARY STATISTICS")
    print("="*60)
    print("\nOriginal Data Statistics:")
    print(f"   X min: {original_df[x_column].min():.2f}, X max: {original_df[x_column].max():.2f}")
    print(f"   Y min: {original_df[y_column].min():.2f}, Y max: {original_df[y_column].max():.2f}")
    
    print("\nNew Data Statistics:")
    print(f"   X min: {new_df[x_column].min():.2f}, X max: {new_df[x_column].max():.2f}")
    print(f"   Y min: {new_df[y_column].min():.2f}, Y max: {new_df[y_column].max():.2f}")
    
    # Calculate extrapolation portions
    orig_min = original_df[x_column].min()
    orig_max = original_df[x_column].max()
    
    extrap_left = new_df[new_df[x_column] < orig_min]
    extrap_right = new_df[new_df[x_column] > orig_max]
    interp_region = new_df[(new_df[x_column] >= orig_min) & (new_df[x_column] <= orig_max)]
    
    print(f"\nExtrapolation Analysis:")
    print(f"   Left extrapolation points (x < {orig_min}): {len(extrap_left)}")
    print(f"   Interpolation points ({orig_min} ≤ x ≤ {orig_max}): {len(interp_region)}")
    print(f"   Right extrapolation points (x > {orig_max}): {len(extrap_right)}")

if __name__ == "__main__":
    main()
