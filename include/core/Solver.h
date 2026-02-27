#ifndef SOLVER_H
#define SOLVER_H

#include "MACGrid.h"

namespace Solver {

    // === Velocity Advection ===
    // Semi-Lagrangian advection of velocity fields on the MAC grid
    void advectVelocities(MACGrid& grid, double dt);

    // === Velocity Sampling Helpers ===
    // Bilinearly interpolates u (x-velocity) at world position (x, y)
    double sampleU(const MACGrid& grid, double x, double y);

    // Bilinearly interpolates v (y-velocity) at world position (x, y)
    double sampleV(const MACGrid& grid, double x, double y);

    // === Position Helpers ===
    // Returns the world-space position of the u-face at (i, j)
    std::pair<double, double> uFaceWorldPos(int i, int j, double h);

    // Returns the world-space position of the v-face at (i, j)
    std::pair<double, double> vFaceWorldPos(int i, int j, double h);



    //gravity
    void applyGravity(MACGrid& grid, double gravity, double dt);

    //helper functions 
    void updateMaxMACCellVelocity(MACGrid& grid, double u, double v);
}
    #endif // SOLVER_H