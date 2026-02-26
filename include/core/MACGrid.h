
#ifndef MACGRID_H
#define MACGRID_H

#include <unordered_map>
#include <utility>
#include "core/MACCell.h"
#include <string>

// Hash function for coordinate pairs (needed for unordered_map)
struct hash_pair
{
    size_t operator()(const std::pair<int, int> &p) const
    {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class MACGrid
{
public:
    std::unordered_map<std::pair<int, int>, MACCell *, hash_pair> grid;

    MACGrid() = default;
    ~MACGrid();

    void addCell(int x, int y, Type type);
    MACCell *getCell(int x, int y);
    const MACCell* getCell(int x, int y) const;
    void linkNeighbors(int x, int y);
    void setCellType(int x, int y, Type t);
    void printAscii(int minX, int maxX, int minY, int maxY) const;
    void printVelocities(const std::string& label) const;

private:
    void deleteCells();
};

#endif