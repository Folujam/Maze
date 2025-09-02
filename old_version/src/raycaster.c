#include "../dependencies/header.h"


void render_scene(SDL_Instance *instance)
{
	int w = 1260;
	int h = 720;

	for (int x = 0; x < w; x++)
	{
		// 1. Camera plane x coordinate [-1, 1]
		double cameraX = 2 * x / (double)w - 1;
		double rayDirX = dirX + planeX * cameraX;
		double rayDirY = dirY + planeY * cameraX;

		// 2. Which box of the map we're in
		int mapX = (int)posX;
		int mapY = (int)posY;

		// 3. Length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		// 4. Length of ray from one x or y-side to next x or y-side
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;

		// 5. Direction to step in x and y
		int stepX;
		int stepY;

		int hit = 0; // Was a wall hit?
		int side;    // NS or EW wall?

		// 6. Calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - posY) * deltaDistY;
		}

		// 7. Perform DDA
		while (hit == 0)
		{
			if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
        		break; // Out of bounds check
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			/*if (worldMap[mapX][mapY] > 0)
			{
    			hit = 1;
    			printf("Ray %d hit wall at map (%d, %d)\n", x, mapX, mapY);
			}*/
			if (worldMap[mapX][mapY] > 0) hit = 1;
		}

		// 8. Calculate distance to wall
		if (side == 0)
			perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

		// 9. Calculate height of line to draw
		int lineHeight = (int)(h / perpWallDist);

		// 10. Calculate lowest and highest pixel
		int drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h) drawEnd = h - 1;

		// 11. Set wall color
		SDL_SetRenderDrawColor(instance->renderer,
			side == 1 ? 200 : 255, side == 1 ? 0 : 255, 0, 255);

		// 12. Draw vertical line
		SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);

		// Mini-map rendering
		// Draw minimap background
		SDL_Rect miniBg = {
			MINI_MAP_PADDING,
			MINI_MAP_PADDING,
			MINI_MAP_WIDTH,
			MINI_MAP_HEIGHT
		};
		SDL_SetRenderDrawColor(instance->renderer, 50, 50, 50, 255); // Dark gray
		SDL_RenderFillRect(instance->renderer, &miniBg);

		// Draw map tiles
		for (int y = 0; y < MAP_HEIGHT; ++y) {
			for (int x = 0; x < MAP_WIDTH; ++x) {
				SDL_Rect cell = {
					MINI_MAP_PADDING + x * MINI_CELL_SIZE,
					MINI_MAP_PADDING + y * MINI_CELL_SIZE,
					MINI_CELL_SIZE,
					MINI_CELL_SIZE
				};
				int tile = worldMap[x][y];
				SDL_SetRenderDrawColor(instance->renderer, tile ? 0 : 200, 0, 0, 255); // walls red, open green
				SDL_RenderFillRect(instance->renderer, &cell);
			}
		}

		// Draw player
		SDL_Rect playerRect = {
			MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE) - 2,
			MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE) - 2,
			4, 4
		};
		SDL_SetRenderDrawColor(instance->renderer, 0, 255, 0, 255); // green dot
		SDL_RenderFillRect(instance->renderer, &playerRect);

		// Draw direction line
		int px = MINI_MAP_PADDING + (int)(posX * MINI_CELL_SIZE);
		int py = MINI_MAP_PADDING + (int)(posY * MINI_CELL_SIZE);
		int dx = px + (int)(dirX * MINI_CELL_SIZE * 4);
		int dy = py + (int)(dirY * MINI_CELL_SIZE * 4);
		SDL_SetRenderDrawColor(instance->renderer, 255, 255, 0, 255); // yellow direction line
		SDL_RenderDrawLine(instance->renderer, px, py, dx, dy);

	}
}
