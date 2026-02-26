#include "core/MACGrid.h"
#include "core/Constants.h"

class Shape
{
public:
    virtual void applyToGrid(MACGrid &grid) = 0; // Forces child classes to implement this
    double inlet_d = 0.0, outlet_d = 0.0;        // Diameter of inlet/outlet (default 0)

protected:
    double startx, starty;   // Absolute starting position meters
    double waterLevel = 1.0; // Default = full water (1.0), 0.0 = empty
};

class Rectangle : public Shape
{
public:
    int gridX, gridY; // Cell coordinates
    int gridWidth, gridHeight;

    Rectangle(double x_m, double y_m, double h_m, double w_m, double wl = 1.0)
    {
        startx = x_m;
        starty = y_m;
        waterLevel = wl;

        gridX = static_cast<int>(x_m / CELL_SIZE);
        gridY = static_cast<int>(y_m / CELL_SIZE);
        gridHeight = static_cast<int>(h_m / CELL_SIZE);
        gridWidth  = static_cast<int>(w_m / CELL_SIZE);
    }

    void applyToGrid(MACGrid &grid) override
    {

        int outerX0 = gridX - 1;
        int outerX1 = gridX + gridWidth;
        int outerY0 = gridY - 1;
        int outerY1 = gridY + gridHeight;

        // 1. Set outer boundary as SOLID
        for (int i = outerX0; i <= outerX1; i++) {
            grid.setCellType(i, outerY0, SOLID); // bottom
            grid.setCellType(i, outerY1, SOLID); // top
        }
        for (int j = outerY0; j <= outerY1; j++) {
            grid.setCellType(outerX0, j, SOLID); // left
            grid.setCellType(outerX1, j, SOLID); // right
        }

        // 2. Fill interior with WATER or AIR depending on level
        for (int i = gridX; i < gridX + gridWidth; i++) {
            for (int j = gridY; j < gridY + gridHeight; j++) {
                if (j < gridY + static_cast<int>(gridHeight * waterLevel)) {
                    grid.setCellType(i, j, WATER);
                } else {
                    grid.setCellType(i, j, AIR);
                }
            }
        }
    }
};