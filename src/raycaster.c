#include "../dependencies/header.h"

void render_scene(SDL_Instance *instance)
{
    int w, h;
    SDL_GetRendererOutputSize(instance->renderer, &w, &h);
	const Obstacle *oarr = NULL;
	int oc = get_obstacles(&oarr);
    // Clear once per frame (leave in main is also fine, but okay here)
    // SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(instance->renderer);

    for (int x = 0; x < w; x++)
    {
        double cameraX = 2.0 * x / (double)w - 1.0;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double deltaDistX = (rayDirX == 0.0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0.0) ? 1e30 : fabs(1.0 / rayDirY);
        double sideDistX, sideDistY, perpWallDist;

        int stepX, stepY;
        int hit = 0, side = 0;

        if (rayDirX < 0) { stepX = -1; sideDistX = (posX - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0 - posX) * deltaDistX; }

        if (rayDirY < 0) { stepY = -1; sideDistY = (posY - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0 - posY) * deltaDistY; }

        while (!hit)
        {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else { sideDistY += deltaDistY; mapY += stepY; side = 1; }

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = -1; /* out-of-bounds ray; drop this column */
                break;
            }
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        if (hit <= 0) continue; /* skip drawing this column */

        if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2.0) / rayDirX;
        else           perpWallDist = (mapY - posY + (1 - stepY) / 2.0) / rayDirY;

        if (perpWallDist <= 0) continue;

        int lineHeight = (int)(h / perpWallDist);
        int drawStart = -lineHeight / 2 + h / 2; if (drawStart < 0) drawStart = 0;
        int drawEnd   =  lineHeight / 2 + h / 2; if (drawEnd >= h) drawEnd = h - 1;

        SDL_SetRenderDrawColor(instance->renderer,
            side ? 200 : 255, side ? 0 : 255, 0, 255);
        SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);
    }

	/* draw obstacles as simple colored squares in 3D space */
	for (int i = 0; i < oc; ++i) {
		double spriteX = oarr[i].x - posX;
		double spriteY = oarr[i].y - posY;

		/* inverse camera transform */
		double invDet = 1.0 / (planeX * dirY - dirX * planeY);
		double transformX = invDet * (dirY * spriteX - dirX * spriteY);
		double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

		if (transformY <= 0) continue; /* behind player */

		int screenX = (int)((w / 2) * (1 + transformX / transformY));

		int spriteHeight = abs((int)(h / transformY));
		int drawStartY = -spriteHeight / 2 + h / 2; if (drawStartY < 0) drawStartY = 0;
		int drawEndY   =  spriteHeight / 2 + h / 2; if (drawEndY >= h) drawEndY = h - 1;

		int spriteWidth = spriteHeight;
		int drawStartX = -spriteWidth / 2 + screenX; if (drawStartX < 0) drawStartX = 0;
		int drawEndX   =  spriteWidth / 2 + screenX; if (drawEndX >= w) drawEndX = w - 1;

		SDL_SetRenderDrawColor(instance->renderer, 255, 165, 0, 255);
		for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
			SDL_RenderDrawLine(instance->renderer, stripe, drawStartY, stripe, drawEndY);
		}
	}

    /* --- draw minimap ONCE per frame --- */
    SDL_Rect miniBg = { MINI_MAP_PADDING, MINI_MAP_PADDING, MINI_MAP_WIDTH, MINI_MAP_HEIGHT };
    SDL_SetRenderDrawColor(instance->renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(instance->renderer, &miniBg);

    for (int my = 0; my < MAP_HEIGHT; ++my) {
        for (int mx = 0; mx < MAP_WIDTH; ++mx) {
            SDL_Rect cell = {
                MINI_MAP_PADDING + mx * MINI_CELL_SIZE,
                MINI_MAP_PADDING + my * MINI_CELL_SIZE,
                MINI_CELL_SIZE, MINI_CELL_SIZE
            };
            int tile = worldMap[mx][my];
            /* walls = red, open = green */
            if (tile) SDL_SetRenderDrawColor(instance->renderer, 200, 0, 0, 255);
            else      SDL_SetRenderDrawColor(instance->renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(instance->renderer, &cell);
        }
    }

    /* player dot */
    SDL_Rect playerRect = {
        MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE) - 2,
        MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE) - 2,
        4, 4
    };
    SDL_SetRenderDrawColor(instance->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(instance->renderer, &playerRect);

    /* facing line */
    int px = MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE);
    int py = MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE);
    int dx = px + (int)(dirX * MINI_CELL_SIZE * 4);
    int dy = py + (int)(dirY * MINI_CELL_SIZE * 4);
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(instance->renderer, px, py, dx, dy);

    /* HUD (traffic light & obstacles will draw more here later) */
	/* draw obstacles on minimap */
	for (int i = 0; i < oc; ++i) {
		SDL_Rect o = {
			MINI_MAP_PADDING + (int)(oarr[i].x * MINI_CELL_SIZE) - 2,
			MINI_MAP_PADDING + (int)(oarr[i].y * MINI_CELL_SIZE) - 2,
			4, 4
		};
		SDL_SetRenderDrawColor(instance->renderer, 255, 165, 0, 255); /* orange */
		SDL_RenderFillRect(instance->renderer, &o);
	}

	/* traffic light HUD (top-right) */
	int hudSize = 28;
	SDL_Rect hud = { .x = (int)(MINI_MAP_PADDING + MINI_MAP_WIDTH + 12),
					.y = MINI_MAP_PADDING,
					.w = hudSize, .h = hudSize };
	if (get_traffic_green())
		SDL_SetRenderDrawColor(instance->renderer, 0, 220, 0, 255);
	else
		SDL_SetRenderDrawColor(instance->renderer, 220, 0, 0, 255);
	SDL_RenderFillRect(instance->renderer, &hud);

    /* draw a label box */
    SDL_Rect hudFrame = { hud.x - 6, hud.y - 6, hud.w + 12, hud.h + 12 };
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 180);
    SDL_RenderDrawRect(instance->renderer, &hudFrame);

    /* score HUD (top-left corner) */
    char buf[64];
    sprintf(buf, "Score: %d", playerScore);
    SDL_Color white = {255, 255, 255, 255};
}
