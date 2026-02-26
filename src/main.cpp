#include <iostream>
#include "core/MACGrid.h"
#include "core/Shape.h"
#include "core/Solver.h"
#include "visual/SDLRenderer.h"

int main()
{
    MACGrid grid;
    Rectangle tank(1, 1, 1, 1, 1); // 10x5 tank filled fully
    tank.applyToGrid(grid);

    SDLRenderer renderer(1920, 1080, 30);

    const double dt = 0.1;
    bool running = true;
    int frame = 0;

    while (running)
    {
        std::cout << "Running frame: " << frame++ << "\n";

        // Simulation step
        Solver::applyGravity(grid, -9.81, dt);
        grid.printVelocities("After Gravity");
        //Solver::advectVelocities(grid, dt);
        //grid.printVelocities("After Advection");

        // Render
        printf("\n\n\nRendering\n\n");
        renderer.drawGrid(grid);

        SDL_Delay(100); // Delay in ms (for demo purposes)

        // Optional: break after X frames
        if (frame > 50)
            running = false;
    }

    return 0;
}