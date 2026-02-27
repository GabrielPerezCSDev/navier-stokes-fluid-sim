#ifndef MACCELL_H
#define MACCELL_H

#include <cstddef>
#include "core/Constants.h"

enum Type
{
  WATER,
  SOLID,
  AIR
};

struct MACCell
{
  Type type;       // Fluid type
  double uL;   // Left X-face velocity (m/s)
  double vB;   // Bottom Y-face velocity (m/s)
  double pressure; // Pascals (N/m^2)
  double density;  // kg/m^3
  double size;     // Cell resolution (m)
  int x, y;        // integer cell loc

  // Neighbor pointers (quaqd linked list)
  MACCell *left = nullptr;
  MACCell *right = nullptr;
  MACCell *top = nullptr;
  MACCell *bottom = nullptr;

  // Constructor (defaults)
  MACCell(int x_, int y_, Type t)
      : x(x_), y(y_), type(t), size(CELL_SIZE),
      uL(0), vB(0)
  {
    switch (t)
    {
    case WATER:
      density = DENSITY_WATER;
      pressure = DEFAULT_PRESSURE;
      break;
    case AIR:
      density = DENSITY_AIR;
      pressure = DEFAULT_PRESSURE;
      break;
    case SOLID:
      density = DENSITY_SOLID;
      pressure = DEFAULT_PRESSURE;
      break;
    }
  }
};

#endif
