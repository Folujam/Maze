#include "../dependencies/header.h"

void draw_minimap(SDL_Instance *instance)
{
    /* draws background */
    SDL_Rect miniBg = {
        MINI_MAP_PADDING,
        MINI_MAP_PADDING,
        MINI_MAP_WIDTH,
        MINI_MAP_HEIGHT
    };

    SDL_SetRenderDrawColor(instance->renderer, 50, 50, 50, 255); // dark gray
    SDL_RenderFillRect(instance->renderer, &miniBg);

    /* draws walls and floor */
    for (int y = 0; y < MAP_HEIGHT; ++y) 
    {
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

     /* draw alive spirits */
    const Spirit *arr = NULL; int cnt = get_spirits(&arr);
    for (int i = 0; i < cnt; ++i) {
        if (!arr[i].alive) continue;
        SDL_Rect spiritRect = {
            MINI_MAP_PADDING + (int)(arr[i].x * MINI_CELL_SIZE) - 2,
            MINI_MAP_PADDING + (int)(arr[i].y * MINI_CELL_SIZE) - 2,
            4, 4
        };
        SDL_SetRenderDrawColor(instance->renderer, 255, 165, 0, 255); // orange
        SDL_RenderFillRect(instance->renderer, &spiritRect);
    }

    /* player dot */
    SDL_Rect playerRect = {
        MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE) - 2,
        MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE) - 2,
        4, 4
    };
    SDL_SetRenderDrawColor(instance->renderer, 0, 255, 0, 255); 
    SDL_RenderFillRect(instance->renderer, &playerRect);
    
    /* ---draws directional line on player dot--- */ 
    int px = MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE);
    int py = MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE);
    int dx = px + (int)(dirX * MINI_CELL_SIZE * 4);
    int dy = py + (int)(dirY * MINI_CELL_SIZE * 4);
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 0, 255); 
    SDL_RenderDrawLine(instance->renderer, px, py, dx, dy);
}
