#include "core/Solver.h"
#include "core/Constants.h"
#include <iostream>
#include <cmath>
namespace Solver
{

    // === Velocity Advection ===
    // Semi-Lagrangian advection of velocity fields on the MAC grid
    void advectVelocities(MACGrid &grid, double dt)
    {
        double x_prev;
        double y_prev;
        int i;
        int j;
        std::pair<double, double> rwPair;
        std::unordered_map<std::pair<int, int>, double, hash_pair> newUL, newVB;
        for (const auto &[pos, cell] : grid.grid)
        {
            i = pos.first;
            j = pos.second;

            // left
            rwPair = uFaceWorldPos(i, j, cell->size);
            x_prev = rwPair.first - dt * cell->uL;
            y_prev = rwPair.second - dt * sampleV(grid, rwPair.first, rwPair.second);
            newUL[pos] = sampleU(grid, x_prev, y_prev);

            // bottom
            rwPair = vFaceWorldPos(i, j, cell->size);
            x_prev = rwPair.first - dt * sampleU(grid, rwPair.first, rwPair.second);
            y_prev = rwPair.second - dt * cell->vB;
            newVB[pos] = sampleV(grid, x_prev, y_prev);
        }

        // now copy back to the grid
        for (const auto &[pos, cell] : grid.grid)
        {
            if (newUL.count(pos))
                cell->uL = newUL[pos];
            if (newVB.count(pos))
                cell->vB = newVB[pos];
            updateMaxMACCellVelocity(grid, cell->uL, cell->vB);
        }
    }

    // === Velocity Sampling Helpers ===
    // Bilinearly interpolates u (x-velocity) at world position (x, y)
    double sampleU(const MACGrid &grid, double x, double y)
    {
        // grab cell size from the constants header
        double h = CELL_SIZE;

        // Determine grid-space index of bottom-left u-face
        int i = static_cast<int>(x / h);
        int j = static_cast<int>((y - 0.5 * h) / h); // u-faces are vertically centered

        // World position of bottom-left face
        double x0 = i * h;
        double y0 = (j + 0.5) * h;

        // Compute interpolation weights (normalized to [0,1])
        double tx = (x - x0) / h;
        double ty = (y - y0) / h;

        // Get surrounding cells that hold u-values at the left face
        const MACCell *c00 = grid.getCell(i, j);
        const MACCell *c10 = grid.getCell(i + 1, j);
        const MACCell *c01 = grid.getCell(i, j + 1);
        const MACCell *c11 = grid.getCell(i + 1, j + 1);

        // Ensure none of the cells are null to avoid segfault
        if (!c00 || !c10 || !c01 || !c11){
            return 0.0;
        }

        // Get u-values from the left face of each cell
        double u00 = c00->uL;
        double u10 = c10->uL;
        double u01 = c01->uL;
        double u11 = c11->uL;

        // Perform bilinear interpolation
        return (1 - tx) * (1 - ty) * u00 +
               tx * (1 - ty) * u10 +
               (1 - tx) * ty * u01 +
               tx * ty * u11;
    }

    // Bilinearly interpolates v (y-velocity) at world position (x, y)
    double sampleV(const MACGrid &grid, double x, double y)
    {
        // grab cell size from the constants header
        double h = CELL_SIZE;

        // Vertical velocity (v) is stored at horizontal faces:
        // v(i,j) is located at ((i + 0.5) * h, j * h)
        int i = static_cast<int>((x - 0.5 * h) / h);
        int j = static_cast<int>(y / h);

        // World-space position of bottom-left v-face
        double x0 = (i + 0.5) * h;
        double y0 = j * h;

        double tx = (x - x0) / h;
        double ty = (y - y0) / h;

        const MACCell *c00 = grid.getCell(i, j);
        const MACCell *c10 = grid.getCell(i + 1, j);
        const MACCell *c01 = grid.getCell(i, j + 1);
        const MACCell *c11 = grid.getCell(i + 1, j + 1);

        if (!c00 || !c10 || !c01 || !c11)
            return 0.0;

        double v00 = c00->vB;
        double v10 = c10->vB;
        double v01 = c01->vB;
        double v11 = c11->vB;

        return (1 - tx) * (1 - ty) * v00 +
               tx * (1 - ty) * v10 +
               (1 - tx) * ty * v01 +
               tx * ty * v11;
    }

    std::pair<double, double> uFaceWorldPos(int i, int j, double h)
    {
        return std::make_pair(i*h, (j + 0.5) *h);
    }

    std::pair<double, double> vFaceWorldPos(int i, int j, double h)
    {
        return std::make_pair((i + 0.5) * h, j*h);
    }


    //gravity
    void applyGravity(MACGrid& grid, double gravity, double dt){
        for (auto& [pos, cell] : grid.grid) {
            if (cell->type == WATER) {
                cell->vB += gravity * dt;
            }
        }
    }

    void updateMaxMACCellVelocity(MACGrid& grid, double u, double v){
        double velocity = std::sqrt(u*u + v*v);
        grid.maxVelocity = std::max(grid.maxVelocity, velocity);
    }

}