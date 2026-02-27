#ifndef SDLRenderer_H
#define SDLRenderer_H

#include "../core/MACGrid.h"
#include <SDL2/SDL.h>

class SDLRenderer {
    public:
    SDLRenderer(int screenWidth, int screenHeight, int cellSize);
    ~SDLRenderer();

    void drawGrid(const MACGrid& grid);
    bool pollEvents(); //return false on quit

    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenWidth, screenHeight, cellSize; 
    int offsetX = 0;  
    int offsetY = 0;
    int gridMaxY = 0;
    bool offsetInitialized = false;

    void calculateCenterOffset(const MACGrid& grid);
    
    SDL_Color velocityColor(double speed, double maxSpeed);
};

#endif