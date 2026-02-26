fluid_sim/
│── src/                     # Source code (.cpp files)
│   │── main.cpp             # Entry point
│   │── core/                # Core fluid simulation logic
│   │   │──  MACCell.cpp
│   │   │──  MACGrid.cpp
│   │   │──  Solver.cpp
│   │── visual/           # Visualization and GUI logic
│   │   │── SDLRenderer.cpp
│
│── include/                 # Header files (.h files)
│   │── core/                # Headers for core simulation
│   │   │── Boundary.h
│   │   │── Constants.h
│   │   │── MACCell.h
│   │   │── MACGrid.h
│   │   │── Shape.h
|   |   |── Solver.h
│   │── visual/           # Headers for visualization
│   │   │── SDLRenderer.h
│
│── physics/                 # Physics-related extensions (future)
│   │── FreeSurface.cpp
│   │── FreeSurface.h
│   │── Turbulence.cpp
│   │── Turbulence.h
│
│── assets/                  # GUI-related assets (icons, textures) (future)
│── tests/                   # Unit tests for core functionality (future)
│── CMakeLists.txt           # CMake build configuration
│── README.md                # Project documentation
