#include "../dependencies/header.h"

/**
* poll_events - loops tru to finds what event occured and executes it
* Return: int
*/
int poll_events(void)
{
	SDL_Event event;
	SDL_KeyboardEvent key;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (1);
			case SDL_KEYDOWN:
				key = event.key;
				/*if esc pressed*/
				if (key.keysym.scancode == 0x29)
					return (1);

				// Forward (W)
                if (key.keysym.scancode == SDL_SCANCODE_W)
                {
                    if (worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0)
                        posX += dirX * moveSpeed;
                    if (worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0)
                        posY += dirY * moveSpeed;
                }

                // Backward (S)
                if (key.keysym.scancode == SDL_SCANCODE_S)
                {
                    if (worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0)
                        posX -= dirX * moveSpeed;
                    if (worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0)
                        posY -= dirY * moveSpeed;
                }

                // Rotate Left (A)
                if (key.keysym.scancode == SDL_SCANCODE_A)
                {
                    double oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                    double oldPlaneX = planeX;
                    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }

                // Rotate Right (D)
                if (key.keysym.scancode == SDL_SCANCODE_D)
                {
                    double oldDirX = dirX;
                    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                    double oldPlaneX = planeX;
                    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
                }

				break;
		}
	}
	return (0);
}
