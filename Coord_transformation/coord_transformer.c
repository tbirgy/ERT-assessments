#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846


/*  GIS2Radar
    ---------
    Purpose: 
        Converts input coordinates of inital and final locations from their latitude/longitude
        values to a range and bearing from the initial location to the final location.
    
    Parmeters:
        - *range    : double, range in km from intial to final location
        - *bearing  : double, bearing in degrees from intial to final location
        - glonInit  : double, initial longitude
        - glatInit  : double, initial latitude
        - glonFinal : double, final longitude
        - glatFinal : double, final latitude
    
    Returns:
        Updates the input variables *range and *bearing with the calculated values.
*/
int GIS2Radar(double *range, double *bearing, double glonInit, double glatInit, double glonFinal, double glatFinal) {
    
    // Convert degrees to radians
    double lat1 = glatInit * PI / 180.0;
    double lon1 = glonInit * PI / 180.0;
    double lat2 = glatFinal * PI / 180.0;
    double lon2 = glonFinal * PI / 180.0;

    // Use Haversine formula
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = sin(dlat/2) * sin(dlat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    // Define Earth's radius (km) and calculate range
    double R = 6371.0;
    *range = R * c;

    // Calculate bearing
    double y = sin(dlon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon);
    *bearing = atan2(y, x) * 180.0 / PI;
    if (*bearing < 0) {
        *bearing += 360.0;
    }

    return 0;
}


/*  RtoG
    ----
    Purpose: 
        Converts input coordinates given as a range and bearing from the initial location to a final 
        location to their their latitude/longitude values.
    
    Parmeters:
        - range      : double, range in km from intial to final location
        - bearing    : double, bearing in degrees from intial to final location
        - glonInit   : double, initial longitude
        - glatInit   : double, initial latitude
        - *glonFinal : double, final longitude
        - *glatFinal : double, final latitude
    
    Returns:
        Updates the input variables *glonFinal and *glatFinal with the calculated values.
*/
int RtoG(double range, double bearing, double glonInit, double glatInit, double *glonFinal, double *glatFinal) {
    // Convert degrees to radians
    double lat1 = glatInit * PI / 180.0;
    double lon1 = glonInit * PI / 180.0;
    double brng = bearing * PI / 180.0;

    // Define Earth's radius (km) and calculate final lat/long
    double R = 6371.0;
    double lat2 = asin(sin(lat1) * cos(range/R) + cos(lat1) * sin(range/R) * cos(brng));
    double lon2 = lon1 + atan2(sin(brng) * sin(range/R) * cos(lat1), cos(range/R) - sin(lat1) * sin(lat2));

    // Convert back to degrees
    *glatFinal = lat2 * 180.0 / PI;
    *glonFinal = lon2 * 180.0 / PI;

    return 0;
}


/*  main program
    ------------
    Purpose:
        Convert from GIS and Radar coordinates and vice versa. Currently the inital location is Wallops
        island, and the final location is Puerto Rico. These are hardcoded but the program can be 
        modified to include these as command line inputs.
    
    Example usage:
        - Build    : "gcc coord_transformer.c -o coord_trans"
        - RtoG     :  "coord_trans.exe -r"
        - GIS2Radar:  "coord_trans.exe -g"
    
    Output:
        Displays the methodology used ("Radar to GIS" or "GIS to Radar") and the calculated data. Data is
        either range/bearing from inital to final location or latitude/longitude of final location.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [-r|-g]\n", argv[0]);
        return 1;
    }

    double glonInit = -75.0;  // Wallops Island longitude
    double glatInit = 37.0;   // Wallops Island latitude
    double glonFinal = -66.0; // Puerto Rico longitude
    double glatFinal = 18.0;  // Puerto Rico latitude

    if (strcmp(argv[1], "-g") == 0) {
        double range, bearing;
        int result = GIS2Radar(&range, &bearing, glonInit, glatInit, glonFinal, glatFinal);
        if (result == 0) {
            printf("GIS to Radar:\n");
            printf("Range: %.2f km\n", range);
            printf("Bearing: %.2f degrees\n", bearing);
        } else {
            printf("Error in GIS2Radar calculation\n");
        }
    } else if (strcmp(argv[1], "-r") == 0) {
        double range, bearing;
        // First, calculate range and bearing
        GIS2Radar(&range, &bearing, glonInit, glatInit, glonFinal, glatFinal);
        
        // Then use these to calculate back to GIS coordinates
        double calculatedLon, calculatedLat;
        int result = RtoG(range, bearing, glonInit, glatInit, &calculatedLon, &calculatedLat);
        if (result == 0) {
            printf("Radar to GIS:\n");
            printf("Final Longitude: %.6f\n", calculatedLon);
            printf("Final Latitude: %.6f\n", calculatedLat);
        } else {
            printf("Error in RtoG calculation\n");
        }
    } else {
        printf("Invalid command. Use -r or -g.\n");
        return 1;
    }

    return 0;
}