#include "../dependencies/header.h"

void draw_minimap(SDL_Instance *instance)
{
    SDL_Rect miniBg = {
        MINI_MAP_PADDING,
        MINI_MAP_PADDING,
        MINI_MAP_WIDTH,
        MINI_MAP_HEIGHT
    };
    SDL_SetRenderDrawColor(instance->renderer, 50, 50, 50, 255); // dark gray
    SDL_RenderFillRect(instance->renderer, &miniBg);

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            SDL_Rect cell = {
                MINI_MAP_PADDING + x * MINI_CELL_SIZE,
                MINI_MAP_PADDING + y * MINI_CELL_SIZE,
                MINI_CELL_SIZE,
                MINI_CELL_SIZE
            };
            int tile = worldMap[x][y];
            SDL_SetRenderDrawColor(instance->renderer, tile ? 0 : 200, 0, 0, 255); 
            SDL_RenderFillRect(instance->renderer, &cell);
        }
    }

    // player
    SDL_Rect playerRect = {
        MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE) - 2,
        MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE) - 2,
        4, 4
    };
    SDL_SetRenderDrawColor(instance->renderer, 0, 255, 0, 255); 
    SDL_RenderFillRect(instance->renderer, &playerRect);

    int px = MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE);
    int py = MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE);
    int dx = px + (int)(dirX * MINI_CELL_SIZE * 4);
    int dy = py + (int)(dirY * MINI_CELL_SIZE * 4);
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 0, 255); 
    SDL_RenderDrawLine(instance->renderer, px, py, dx, dy);
}
