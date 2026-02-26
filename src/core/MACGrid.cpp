#include "core/MACGrid.h"
#include <iostream>
#include <iomanip>
#include <string>

MACGrid::~MACGrid() {
    deleteCells();
}

// Add a new cell to the grid 
void MACGrid::addCell(int x, int y, Type type){

    //avoid duplication 
    if(grid.find({x,y}) != grid.end()) return;

    grid[{x,y}] = new MACCell(x,y,type);

    //link with neighbors
    linkNeighbors(x,y);
}

//retreive a cell (if !exist return nullptr)
MACCell* MACGrid::getCell(int x, int y){
    auto it = grid.find({x,y});
    return (it != grid.end()) ? it->second : nullptr;
}

const MACCell* MACGrid::getCell(int x, int y) const {
    auto it = grid.find({x, y});
    return (it != grid.end()) ? it->second : nullptr;
}

//link newly created cell to neigbors
void MACGrid::linkNeighbors(int x, int y){
    MACCell* cell = getCell(x,y);

    if(!cell) return;

    // Connect left neighbor
    MACCell* leftCell = getCell(x - 1, y);
    if (leftCell) {
        cell->left = leftCell;
        leftCell->right = cell;
    }

    // Connect right neighbor
    MACCell* rightCell = getCell(x + 1, y);
    if (rightCell) {
        cell->right = rightCell;
        rightCell->left = cell;
    }

    // Connect bottom neighbor
    MACCell* bottomCell = getCell(x, y - 1);
    if (bottomCell) {
        cell->bottom = bottomCell;
        bottomCell->top = cell;
    }

    // Connect top neighbor
    MACCell* topCell = getCell(x, y + 1);
    if (topCell) {
        cell->top = topCell;
        topCell->bottom = cell;
    }
}

//delete all cells
void MACGrid::deleteCells() {
    for(auto& entry : grid) {
        delete entry.second;
    }
    grid.clear();
}

void MACGrid::setCellType(int x, int y, Type t) {
    MACCell* cell = getCell(x, y);
    if (!cell) {
        addCell(x, y, t);
    } else {
        cell->type = t;
    }
}

void MACGrid::printAscii(int minX, int maxX, int minY, int maxY) const {
    for (int y = maxY - 1; y >= minY; --y) {
        for (int x = minX; x < maxX; ++x) {
            auto it = grid.find({x, y});
            if (it == grid.end()) {
                std::cout << ' ';
            } else {
                switch (it->second->type) {
                    case SOLID:
                        std::cout << '#';
                        break;
                    case WATER:
                        std::cout << "\033[34m~\033[0m";  // Blue water
                        break;
                    case AIR:
                        std::cout << '.';
                        break;
                }
            }
        }
        std::cout << '\n';
    }
}

void MACGrid::printVelocities(const std::string& label) const {
    std::cout << "\n=== " << label << " ===\n";
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& [pos, cell] : grid) {
        if (cell->type == WATER) {
            std::cout << "(" << pos.first << "," << pos.second << ")"
                      << " uL=" << std::setw(8) << cell->uL
                      << " uR=" << std::setw(8) << cell->uR
                      << " vB=" << std::setw(8) << cell->vB
                      << " vT=" << std::setw(8) << cell->vT
                      << " P="  << std::setw(10) << cell->pressure
                      << "\n";
        }
    }
}
