#include <SDL2/SDL.h>
#include "../../include/core/MACGrid.h"
#include "../../include/core/Shape.h"
#include "../../include/core/Solver.h"
#include "visual/SDLRenderer.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

const double dt = 0.1;

const char *typeToStr(Type t)
{
    switch (t)
    {
    case SOLID:
        return "SOLID";
    case WATER:
        return "WATER";
    case AIR:
        return "AIR";
    default:
        return "UNKNOWN";
    }
}

SDLRenderer::SDLRenderer(int width, int height, int cellSize)
    : screenWidth(width), screenHeight(height), cellSize(cellSize),
      window(nullptr), renderer(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow(
        "Fluid Sim",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screenWidth, screenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    SDL_MaximizeWindow(window);
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

SDLRenderer::~SDLRenderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SDLRenderer::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            return false;
    }
    return true;
}

void SDLRenderer::drawGrid(const MACGrid& grid) {
    if (!offsetInitialized){
        printf("\n\nCalculating offset\n\n");
        calculateCenterOffset(grid);
    }
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // background
    SDL_RenderClear(renderer);

    const int arrowScale = 10;
    int cellNum = 1;

    for (const auto& [pos, cell] : grid.grid) {
        
        int x = pos.first;
        int y = pos.second;

        int drawX = x * cellSize + offsetX;
        int drawY = offsetY + (gridMaxY - y) * cellSize;
        //int drawX = x * cellSize;
        //int drawY = (y * cellSize);

        SDL_Rect rect = { drawX, drawY, cellSize, cellSize };

        switch (cell->type) {
            case SOLID: SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); break;
            case WATER: SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); break;
            case AIR:   SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255); break;
        }

        SDL_RenderFillRect(renderer, &rect);

        if (cell->type == WATER || cell->type == AIR) {
            const MACCell* rightCell = grid.getCell(x + 1, y);
            const MACCell* topCell = grid.getCell(x, y + 1);
            double u = (cell->uL + (rightCell ? rightCell->uL : cell->uL)) / 2.0;
            double v = (cell->vB + (topCell ? topCell->vB : cell->vB)) / 2.0;

            double scale = arrowScale;
            u *= scale;
            v *= scale;

            int cx = drawX + cellSize / 2;
            int cy = drawY + cellSize / 2;
            int ex = cx + static_cast<int>(u);
            int ey = cy - static_cast<int>(v);

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // arrow shaft
            SDL_RenderDrawLine(renderer, cx, cy, ex, ey);

            // Arrowhead
            double dx = ex - cx;
            double dy = ey - cy;
            double len = std::sqrt(dx * dx + dy * dy);

            if (len > 1e-5) {
                dx /= len; dy /= len;
                double perpX = -dy, perpY = dx;
                double headLength = 6.0, headWidth = 4.0;

                int ax1 = ex - static_cast<int>(dx * headLength + perpX * headWidth);
                int ay1 = ey - static_cast<int>(dy * headLength + perpY * headWidth);
                int ax2 = ex - static_cast<int>(dx * headLength - perpX * headWidth);
                int ay2 = ey - static_cast<int>(dy * headLength - perpY * headWidth);

                SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // arrow tips
                SDL_RenderDrawLine(renderer, ex, ey, ax1, ay1);
                SDL_RenderDrawLine(renderer, ex, ey, ax2, ay2);
            }
            cellNum++;
        }
    }

    SDL_RenderPresent(renderer);
}

void SDLRenderer::calculateCenterOffset(const MACGrid& grid) {
    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;

    for (const auto& [pos, _] : grid.grid) {
        minX = std::min(minX, pos.first);
        maxX = std::max(maxX, pos.first);
        minY = std::min(minY, pos.second);
        maxY = std::max(maxY, pos.second);
    }

    int simPixelWidth  = (maxX - minX + 1) * cellSize;
    int simPixelHeight = (maxY - minY + 1) * cellSize;

    offsetX = (screenWidth  - simPixelWidth)  / 2;
    offsetY = (screenHeight - simPixelHeight) / 2;

    gridMaxY = maxY;

    offsetInitialized = true;

    // === Debug Output ===
    std::cout << "\n=== Simulation Grid Bounds ===\n";
    std::cout << "Min X: " << minX << ", Max X: " << maxX << "\n";
    std::cout << "Min Y: " << minY << ", Max Y: " << maxY << "\n";

    std::cout << "\n=== Rendering Info ===\n";
    std::cout << "Cell Size: " << cellSize << " px\n";
    std::cout << "Sim Pixel Width: " << simPixelWidth << " px\n";
    std::cout << "Sim Pixel Height: " << simPixelHeight << " px\n";
    std::cout << "Screen Width: " << screenWidth << " px\n";
    std::cout << "Screen Height: " << screenHeight << " px\n";

    std::cout << "\n=== Calculated Offsets ===\n";
    std::cout << "Offset X: " << offsetX << " px\n";
    std::cout << "Offset Y: " << offsetY << " px\n";

    std::cout << "\n=== Max Y ===\n";
    std::cout << "Max Y: " << offsetX << " px\n";
    std::cout << "Offset Y: " << offsetY << " px\n";

    std::cout << "\n=== Grid Stats ===\n";
    std::cout << "Total Cells: " << grid.grid.size() << "\n";
}
