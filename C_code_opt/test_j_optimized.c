#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "log_table.h"


// Example build script:
// gcc -lm test_j.c -o test_j
// gcc -lm test_j_optimized.c -o tst -pg -no-pie

// Using const double rather than define for type safety,
// precompute static values, and remove unnecessary variables
const double fptildemin = 0.13247393628617882098;
const double logfptmin = -2.02136936017939559330;
const double aX = 0.67501400094624334834;
const double gX = 0.87557210903945397801;
const double ggDivFracPi  = 0.06170962072114729518;
const double aC = 0.0317;
const double gC = 5.87;
const double saC = 0.0547;
const double sbC = 0.0783;
const double sbX = 0.16;


/*  logInterp
    ---------
      This function makes use of the fact that the program only ever searches in the 
      range ln(0.1) to ln(10). It takes in the log argument and interpolates the value
      based on the known table entries.
    
    Parmeters:
      - x : double, the desired x location to be used in ln(x)

    Returns:
      - Interpolated value of ln(x) from the log lookup table.
*/
double logInterp(double x) {
  double x0 = floor(x * 10.0) / 10.0;
  int idx = (int) (x0*10. - 1.);
  double y0 = log_table[idx]; 
  return y0  + (x-x0) * (log_table[idx+1] - y0) * 10.;
}


/*  small_pow
    ---------
      This function makes use of a Taylor expansion for the variables which are raised
      to small powers. Since the input range is known, we can use this to get a faster
      approximation of the true value.
    
    Parmeters:
      - a : double, the base of a^b
      - b : double, the power of a^b

    Returns:
      - Taylor series approximation of a^b
*/
double small_pow(double a, double b) {
  double term = b*logInterp(a);
  double res = 1 + term + 0.5*term*term + term*term*term/6.;
  return res + term*term*term*term/24.;
}


double function_j(double f, double fp, double fptilde, double fracF, double exp1, bool fLessFp) {

  // Use builtin MAX as it's faster than user-defined function
  double fpt = __builtin_fmax(fptilde, fptildemin); 

  double basePow = small_pow(fpt, sbX);      // Calculate once as it's used later on
  double alpha   = aC  * small_pow(fpt, aX); // Approximate value of alpha via Taylor expansion
  double gamma   = gC  * small_pow(fpt, gX); // Approximate value of gamma via Taylor expansion
  double sigma  = fLessFp ? (saC*basePow*basePow) : (sbC * basePow); // Only need to calculate one term per loop.

  double gauss = (f-fp)/(sigma*fp);  // Term looks like the power of a Gaussian distribution
  double exp2arg = -0.5*gauss*gauss;
  
  // Combine multiplications and return S
  double S = alpha * ggDivFracPi * fracF * exp1 * pow(gamma, exp2l(exp2arg));

  return S;
}


main() {

  // Define variables
  double S, f, fp, fptilde, fracF, fDivFp, exp1;
  bool fLessFp;

  for (f = -5.; f <= 5.; f += 0.01) {

    // Only need to calculate once for f-loop
    fracF = 1 / (f * f * f * f * f); 
    
    for (fp = 0.; fp <= 10.; fp += 0.01) {

      // Pull the exp1 vaiable out of the inner-most loop and approximate its values
      // given it asymptotes to 1.
      fLessFp = (f <= fp);
      fDivFp = f/fp;
      exp1 = (abs(fDivFp) > 20) ? 1 : exp2l(-1.25 / (fDivFp * fDivFp * fDivFp * fDivFp));

      for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {

        // Calculate S
        S = function_j(f, fp, fptilde, fracF, exp1, fLessFp);
      }
   }
  }
}
