# ERT-assessments
Assessment problems for ERT technical interview. This repo has separate folders for each ERT assessment. Each folder contains the code required to complete each assessment, along with the associated documentation. The original assessment problem statement has be uploaded alongside this README. Below are brief descriptions of each assessment.

IRI EDP: 
Creates a C-based modeling and simulation program that drives IRI model Fortran code. The code captures and generates vertical EDP (Electron Density Profile) for a given time and location of interest

Coordinate Transformation: 
Implements coordinate conversions from radar coordinates (bearing, range) to GIS coordinates (lat, lon) and vice versa in C.

C-code optimization:
Optimizes the provided C-code. This folder contains both the original and optimized C-code.

Data Collection:
Creates an app that collects data from the NOAA Space Weather Prediction Center (SWPC). The data is the Real-Time Solar Wind (RTSW) from the NOAA/DSCOVR satellite. The source is available at: https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind-1-hour.json

(Optional) Interpolation: 
Interpolates the given location and values to a structured grid of longitude and latitude locations.

(Optional) Modeling fluid dynamics (advanced and hard): 
Transport a square wave across a 1-D structured grid. Use operator splitting for the continuity and momentum equations with no forcing terms.
