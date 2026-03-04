# Navier-Stokes Fluid Simulation

A real-time 2D incompressible fluid simulation built in C++ using the Marker-and-Cell (MAC) staggered grid method. Solves the Navier-Stokes equations via semi-Lagrangian advection with SDL2-based visualization.

## Build Instructions

Requires CMake 3.10+, a C++17 compiler, and SDL2.

```bash
mkdir build
cd build
cmake ..
make
./fluid_sim
```

## Project Structure

```
fluid_sim/
│── src/                     # Source code (.cpp files)
│   │── main.cpp             # Entry point
│   │── core/                # Core fluid simulation logic
│   │   │── MACCell.cpp
│   │   │── MACGrid.cpp
│   │   │── Solver.cpp
│   │── visual/              # Visualization and GUI logic
│   │   │── SDLRenderer.cpp
│
│── include/                 # Header files
│   │── core/
│   │   │── Boundary.h
│   │   │── Constants.h
│   │   │── MACCell.h
│   │   │── MACGrid.h
│   │   │── Shape.h
│   │   │── Solver.h
│   │── visual/
│   │   │── SDLRenderer.h
│
│── physics/                 # Physics extensions (in progress)
│   │── FreeSurface.cpp / .h
│   │── Turbulence.cpp / .h
│
│── assets/                  # GUI assets (future)
│── tests/                   # Unit tests (future)
│── CMakeLists.txt
│── README.md
```

## Technical Details

- **Grid:** MAC staggered grid — velocity components stored at bottom and left cell faces, pressure at cell centers
- **Advection:** Semi-Lagrangian method for stable velocity transport
- **Gravity:** Per-timestep body force applied to fluid cells
- **Rendering:** Real-time SDL2 visualization
- **Cell types:** WATER, AIR, SOLID

## In Progress

- Pressure solver (projection step)
- Free surface tracking
- Turbulence modeling