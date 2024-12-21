#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Usage instructions:
//    1. "make"
//    2. "wave_transport.exe && gnuplot create_gif.gp"


// Define constants
#define NX 1000 // 1000 grid cells
#define DX 1.0  // cell width of 1 meter
#define DT 0.05 // time step via C_max = 0.5
#define NT 200  // number of time steps
#define V 10.0  // initial velocity


/*  save_frame
    ----------
    Purpose: 
        Stores the calculated data at a given time step to a numbered frame in th GIF.
    
    Parmeters:
        - *rho : double, density array
        - *nx  : int, number of data points in rho
        - n    : int, frame number
*/
void save_frame(double* rho, int nx, int n) {
    char filename[20];
    sprintf(filename, "frame_%03d.dat", n);
    FILE* fp = fopen(filename, "w");
    for (int i = 0; i < nx; i++) {
        fprintf(fp, "%f %f\n", i * DX, rho[i]);
    }
    fclose(fp);
}


/*  main
    ----
    Purpose: 
        Updates the density array over a specified range of time and distance.
        For each time step, save the resulting data to a .dat file which 
        represents a single frame of the final GIF.
*/
int main() {
    double* rho = (double*)calloc(NX, sizeof(double));
    double* flux = (double*)malloc(NX * sizeof(double));
    double* momentum = (double*)malloc(NX * sizeof(double));
    double* momentum_flux = (double*)malloc(NX * sizeof(double));

    // Initial conditions
    for (int i = 50; i < 149; i++) {
        rho[i] = 25.0;
    }

    // Main loop
    for (int n = 0; n < NT; n++) {
        
        // Step 1: Continuity equation (half time step)
        for (int i = 0; i < NX; i++) {
            flux[i] = rho[i] * V;
        }
        for (int i = 1; i < NX; i++) {
            rho[i] -= 0.5 * DT / DX * (flux[i] - flux[i-1]);
        }

        // Step 2: Momentum equation (full time step)
        for (int i = 0; i < NX; i++) {
            momentum[i] = rho[i] * V;
            momentum_flux[i] = momentum[i] * V;
        }
        for (int i = 1; i < NX; i++) {
            momentum[i] -= DT / DX * (momentum_flux[i] - momentum_flux[i-1]);
        }

        // Step 3: Continuity equation (half time step)
        for (int i = 1; i < NX; i++) {
            rho[i] -= 0.5 * DT / DX * (momentum[i] - momentum[i-1]);
        }

        // Apply density threshold
        for (int i = 0; i < NX; i++) {
            if (rho[i] <= 1e-6) {
                rho[i] = 0.0;
            }
        }

        // Save the current state
        save_frame(rho, NX, n);
    }

    // Clean up
    free(rho);
    free(flux);
    free(momentum);
    free(momentum_flux);

    return 0;
}
