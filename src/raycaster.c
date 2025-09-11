#include "../dependencies/header.h"

void render_scene(SDL_Instance *instance)
{
    int w, h;
    
    SDL_GetRendererOutputSize(instance->renderer, &w, &h);

    /* --- Sky (top half) --- */
    SDL_Rect ceiling = {0, 0, w, h/2};
    SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 255); /* sky blue */
    SDL_RenderFillRect(instance->renderer, &ceiling);

    /* --- Brick floor (bottom half) --- */
    SDL_Rect floor = {0, h/2, w, h/2};
    SDL_SetRenderDrawColor(instance->renderer, 34, 139, 34, 255);   /* forest green or firebrick red (178,34,34,255)*/
    SDL_RenderFillRect(instance->renderer, &floor);

    /* --- Wall casting --- */
    /* store per-column depth for sprite occlusion */
    static double zbuf[4096]; /* enough for <= 4096 width */
    if (w > (int)(sizeof(zbuf)/sizeof(zbuf[0])))
        w = (int)(sizeof(zbuf)/sizeof(zbuf[0]));

    for (int x = 0; x < w; ++x)
    {
        double cameraX = 2.0 * x / (double)w - 1.0;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double deltaDistX = (rayDirX == 0.0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0.0) ? 1e30 : fabs(1.0 / rayDirY);
        double sideDistX, sideDistY;

        int stepX, stepY, side = 0, hit = 0;

        if (rayDirX < 0) { stepX = -1; sideDistX = (posX - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0 - posX) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (posY - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0 - posY) * deltaDistY; }

        while (!hit)
        {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) { hit = -1; break; }
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }
        if (hit <= 0) continue;

        double perpWallDist = (side == 0)
            ? (mapX - posX + (1 - stepX)/2.0) / rayDirX
            : (mapY - posY + (1 - stepY)/2.0) / rayDirY;
        if (perpWallDist <= 0) perpWallDist = 1e-6;

        zbuf[x] = perpWallDist;

        int lineH = (int)(h / perpWallDist);
        int drawStart = -lineH/2 + h/2; if (drawStart < 0) drawStart = 0;
        int drawEnd   =  lineH/2 + h/2; if (drawEnd >= h) drawEnd = h-1;

        /* WALL COLOR: cool grey; darker on Y sides */
        Uint8 r = side ? 130 : 180;
        Uint8 g = side ? 130 : 180;
        Uint8 b = side ? 140 : 200;
        SDL_SetRenderDrawColor(instance->renderer, r, g, b, 255);
        SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);
    }

    /* --- Render spirits as billboards (rects), occluded by walls --- */
    const Spirit *arr = NULL;
    int cnt = get_spirits(&arr);
    if (cnt > 256)
        cnt = 256; /* sanity limit */
    if (cnt < 1)
        return;
    for (int i = 0; i < cnt; ++i) 
    {
        if (!arr[i].alive)
            continue;

        double sx = arr[i].x - posX;
        double sy = arr[i].y - posY;

        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double tx = invDet * (dirY * sx - dirX * sy);
        double ty = invDet * (-planeY * sx + planeX * sy);

        if (ty <= 0)
            continue;

        int screenX = (int)((w / 2.0) * (1.0 + tx / ty));
        int spriteH = abs((int)(h / ty));
        int drawStartY = -spriteH / 2 + h / 2; if (drawStartY < 0) drawStartY = 0;
        int drawEndY   =  spriteH / 2 + h / 2; if (drawEndY >= h) drawEndY = h - 1;

        int spriteW = spriteH;
        int drawStartX = -spriteW / 2 + screenX; if (drawStartX < 0) drawStartX = 0;
        int drawEndX   =  spriteW / 2 + screenX; if (drawEndX >= w) drawEndX = w - 1;

        for (int stripe = drawStartX; stripe < drawEndX; ++stripe) {
            if (stripe < 0 || stripe >= w)
                continue;
            if (ty < zbuf[stripe]) {
                SDL_SetRenderDrawColor(instance->renderer, 255, 165, 0, 255); /* orange spirit */
                SDL_RenderDrawLine(instance->renderer, stripe, drawStartY, stripe, drawEndY);
            }
        }
    }

    /* --- Render ghosts if in stage 2 --- */
        /* --- Ghosts: red billboards --- */
    if (get_stage() == 2) {
        int gc;
        const Ghost *g = get_ghosts(&gc);
        for (int i = 0; i < gc; i++) {
            if (!g[i].active) continue;

            /* camera-space transform */
            double sx = g[i].x - posX;
            double sy = g[i].y - posY;
            double invDet = 1.0 / (planeX * dirY - dirX * planeY);
            double tx = invDet * (dirY * sx - dirX * sy);
            double ty = invDet * (-planeY * sx + planeX * sy);
            if (ty <= 0) continue;

            int screenX = (int)((w / 2.0) * (1.0 + tx / ty));
            int spriteH = abs((int)(h / ty));
            int drawStartY = -spriteH/2 + h/2; if (drawStartY < 0) drawStartY = 0;
            int drawEndY   =  spriteH/2 + h/2; if (drawEndY >= h) drawEndY   = h-1;

            int spriteW = spriteH;
            int drawStartX = -spriteW/2 + screenX; if (drawStartX < 0) drawStartX = 0;
            int drawEndX   =  spriteW/2 + screenX; if (drawEndX >= w) drawEndX   = w-1;

            for (int stripe = drawStartX; stripe < drawEndX; ++stripe) {
                if (stripe < 0 || stripe >= w) continue;
                if (ty < zbuf[stripe]) {
                    SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255); /* red */
                    SDL_RenderDrawLine(instance->renderer, stripe, drawStartY, stripe, drawEndY);
                }
            }
        }
    }


    /* --- Minimap (yours, already in separate file) --- */
    draw_minimap(instance);  /* From your minimap.c. :contentReference[oaicite:2]{index=2} */

    /* --- HUD: traffic light, score, remaining spirits --- */
    /* Traffic light box at top-right of minimap */
    int hudSize = MINI_MAP_WIDTH / 2;
    SDL_Rect tl = { MINI_MAP_PADDING + MINI_MAP_WIDTH + 12, MINI_MAP_PADDING, hudSize, hudSize };
    if (get_traffic_green())
        SDL_SetRenderDrawColor(instance->renderer, 0, 220, 0, 255);   /* green */
    else
        SDL_SetRenderDrawColor(instance->renderer, 220, 0, 0, 255);   /* red */
    
    SDL_RenderFillRect(instance->renderer, &tl);
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(instance->renderer, &tl);

    /* Score + remaining spirits text (SDL_ttf; fallback if font missing) */
    TTF_Font *font = TTF_OpenFont("assets/free-sans.ttf", 22);
    char buf[96];
    int collected = playerCollected;          /* use collected counter */
    int target    = get_spirits(NULL);        /* spirits initially placed */
    snprintf(buf, sizeof(buf), "%d / %d", collected, target);


    if (font)
    {
        SDL_Color white = {255,255,255,255};
        SDL_Surface *surf = TTF_RenderText_Solid(font, buf, white);
        
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(instance->renderer, surf);
            SDL_Rect dst = { MINI_MAP_PADDING, MINI_MAP_PADDING + MINI_MAP_HEIGHT + 10, surf->w, surf->h };
            SDL_RenderCopy(instance->renderer, tex, NULL, &dst);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surf);
        }
        TTF_CloseFont(font);
    } 
    else
    {
        /* fallback: draw a simple white rectangle if font not found */
        SDL_Rect bar = { MINI_MAP_PADDING, MINI_MAP_PADDING + MINI_MAP_HEIGHT + 10, 220, 20 };
        SDL_SetRenderDrawColor(instance->renderer, 255,255,255,120);
        SDL_RenderFillRect(instance->renderer, &bar);
    }
}
