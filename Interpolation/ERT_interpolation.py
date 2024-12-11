#--------------
#--- Step 1 ---
#--------------
# Load libraries and define the table of known values at given 
# latitudes/longitudes
import numpy as np
import matplotlib.pyplot as plt

data = np.array([
    [121.39, 13.51, 1.494],
    [126.19, 12.02, 1.934],
    [130.27, 13.11, 2.148],
    [127.42, 10.09, 9.155],
    [126.14, 15.33, 2.221],
    [125.96, 14.00, 8.100],
    [123.15, 10.88, 2.039],
    [130.50, 11.18, 1.916],
    [129.08, 15.78, 3.729],
    [122.74, 15.82, 7.137]
])


#--------------
#--- Step 2 ---
#--------------
# Define two required functions to perform the interpolation. 

# getRange
# --------
# Calculates the range (km) bewteen two points defined by
# latitude and longitutde coordinates
def getRange(lat1, lon1, lat2, lon2):
    
    # Convert degrees to radians
    lat1 = lat1 * np.pi / 180.0;
    lon1 = lon1 * np.pi / 180.0;
    lat2 = lat2 * np.pi / 180.0;
    lon2 = lon2 * np.pi / 180.0;

    # Use Haversine formula
    dlon = lon2 - lon1;
    dlat = lat2 - lat1;
    a = np.sin(dlat/2) * np.sin(dlat/2) + np.cos(lat1) * np.cos(lat2) * np.sin(dlon/2) * np.sin(dlon/2);
    c = 2 * np.arctan2(np.sqrt(a), np.sqrt(1-a));
    
    # Define Earth's radius (km) and calculate range
    R = 6371.0;
    return(abs(R*c))

# distWeightedMean
# ----------------
# Performs a distance-weighted mean interpolation on a desired 
# grid (interpPoint) from the values at known coordinates (dataPoints)
def distWeightedMean(dataPoints, interpPoint):

    # Calculate array of distances between each grid point and data point
    distances = np.array([getRange(interpPoint[0], interpPoint[1], dp[0], dp[1]) for dp in dataPoints])

    # Calculate the weights using power parameter > 2 
    # to ensure summation convergence
    weights = 1 / (distances ** 2.1) 
    weighted_sum = np.sum(dataPoints[:, 2] * weights)
    weight_sum = np.sum(weights)
    
    return weighted_sum / weight_sum if weight_sum != 0 else 0


#--------------
#--- Step 3 ---
#--------------
# Create the grid  of desired latitude/longitude coordinates and perform interpolation 

# Define grid 
lat, lon = np.meshgrid(np.linspace(121.0, 131.0, 70), np.linspace(10.0,  16.0,  50))
interpGrid = np.column_stack((lat.ravel(), lon.ravel(), np.zeros_like(lat.ravel())))

# Calculate interpolation
for i in range(interpGrid.shape[0]):
    interpGrid[i, 2] = distWeightedMean(data, interpGrid[i, :2])


#--------------
#--- Step 4 ---
#--------------
# Plot the interpolated grid and overlay the original data

# Define plot
fig, ax = plt.subplots(figsize=(12, 8))

# Create a contour plot of the grid and overlay the original data
lon_unique = np.unique(interpGrid[:, 0])
lat_unique = np.unique(interpGrid[:, 1])
values     = interpGrid[:, 2].reshape(len(lat_unique), len(lon_unique))
contour = ax.contourf(lon_unique, lat_unique, values, 100, cmap='viridis')
scatter = ax.scatter(data[:, 0], data[:, 1], c=data[:, 2], cmap='viridis', marker='*', 
                     s=100, edgecolor='black', linewidth=1, label='Known data')

# Adjust plot style
plt.colorbar(contour).set_label(label='Interpolated Value',size=12)
ax.set_title('Interpolated Values \n(by T. Birgy)', fontsize='14')
ax.set_xlabel('Longitude', fontsize='12')
ax.set_ylabel('Latitude', fontsize='12')
plt.legend(framealpha=0.3)  
plt.show()