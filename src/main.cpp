#include <iostream>
#include "core/MACGrid.h"
#include "core/Shape.h"
#include "core/Solver.h"
#include "visual/SDLRenderer.h"

int main()
{
    MACGrid grid;
    Rectangle tank(1, 1, 1, 3, 1);
    tank.applyToGrid(grid);
    MACCell* testCell = grid.getCell(14, 14);
    if (testCell) testCell->vB = -1;

    SDLRenderer renderer(1920, 1080, 25);

    const double dt = 0.1;
    bool running = true;
    int frame = 0;

    while (running)
    {
        //std::cout << "Running frame: " << frame++ << "\n";
        frame++;
        // Simulation step
        Solver::applyGravity(grid, -9.81, dt);
        //grid.printVelocities("After Gravity");
        Solver::advectVelocities(grid, dt);
        //grid.printVelocities("After Advection");

        // Render
        //printf("\n\n\nRendering\n\n");
        renderer.drawGrid(grid);

        SDL_Delay(100); // Delay in ms (for demo purposes)

        // Optional: break after X frames
        if (frame > 10)
            running = false;
    }

    return 0;
}