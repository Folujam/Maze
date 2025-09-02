#include "../dependencies/header.h"

/**
* poll_events - loops tru to finds what event occured and executes it
* Return: int
*/
int poll_events(void)
{
	SDL_Event event;
	SDL_KeyboardEvent key;
    int moved = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (1);
			case SDL_KEYDOWN:
				key = event.key;
				/*if esc pressed*/
				if (key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					return (1);

                /*DISALLOW MOVEMENT WHILE RED*/
                int allowMove = can_player_move();

				// Forward (W)
                if (key.keysym.scancode == SDL_SCANCODE_W && allowMove)
                {
                    double nx = posX + dirX * moveSpeed;
                    double ny = posY + dirY * moveSpeed;
                    if (nx >= 0 && nx < MAP_WIDTH && worldMap[(int)nx][(int)posY] == 0) 
                        posX = nx;
                    if (ny >= 0 && ny < MAP_HEIGHT && worldMap[(int)posX][(int)ny] == 0)
                        posY = ny;

                    if (will_collide_with_obstacles(posX, posY))
                    {
                        posX -= dirX * moveSpeed;
                        posY -= dirY * moveSpeed;
                    }
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }

                // Backward (S)
                if (key.keysym.scancode == SDL_SCANCODE_S && allowMove) 
                {
                    double nx = posX - dirX * moveSpeed;
                    double ny = posY - dirY * moveSpeed;
                    if (nx >= 0 && nx < MAP_WIDTH && worldMap[(int)nx][(int)posY] == 0)
                        posX = nx;
                    if (ny >= 0 && ny < MAP_HEIGHT && worldMap[(int)posX][(int)ny] == 0)
                        posY = ny;
                    if (will_collide_with_obstacles(posX, posY))
                    {
                        posX += dirX * moveSpeed;
                        posY += dirY * moveSpeed;
                    }
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }

                /* Rotate Left (A) */
                if (key.keysym.scancode == SDL_SCANCODE_A) {
                    double c = cos(-rotSpeed), s = sin(-rotSpeed);
                    double oldDirX = dirX;
                    dirX = dirX * c - dirY * s;
                    dirY = oldDirX * s + dirY * c;
                    double oldPlaneX = planeX;
                    planeX = planeX * c - planeY * s;
                    planeY = oldPlaneX * s + planeY * c;
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }

                /* Rotate Right (D) */
                if (key.keysym.scancode == SDL_SCANCODE_D) {
                    double c = cos(rotSpeed), s = sin(rotSpeed);
                    double oldDirX = dirX;
                    dirX = dirX * c - dirY * s;
                    dirY = oldDirX * s + dirY * c;
                    double oldPlaneX = planeX;
                    planeX = planeX * c - planeY * s;
                    planeY = oldPlaneX * s + planeY * c;
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }

                /* Strafe Left (Q) */
                if (key.keysym.scancode == SDL_SCANCODE_Q && allowMove)
                {
                    double nx = posX - planeX * moveSpeed;
                    double ny = posY - planeY * moveSpeed;
                    if (nx >= 0 && nx < MAP_WIDTH && worldMap[(int)nx][(int)posY] == 0)
                        posX = nx;
                    if (ny >= 0 && ny < MAP_HEIGHT && worldMap[(int)posX][(int)ny] == 0)
                        posY = ny;
                    if (will_collide_with_obstacles(posX, posY))
                    {
                        posX += planeX * moveSpeed;
                        posY += planeY * moveSpeed;
                    }
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }

                /* Strafe Right (E) */
                if (key.keysym.scancode == SDL_SCANCODE_E && allowMove) 
                {
                    double nx = posX + planeX * moveSpeed;
                    double ny = posY + planeY * moveSpeed;
                    if (nx >= 0 && nx < MAP_WIDTH && worldMap[(int)nx][(int)posY] == 0)
                        posX = nx;
                    if (ny >= 0 && ny < MAP_HEIGHT && worldMap[(int)posX][(int)ny] == 0)
                        posY = ny;
                    if (will_collide_with_obstacles(posX, posY))
                    {
                        posX -= planeX * moveSpeed;
                        posY -= planeY * moveSpeed;
                    }
                }
                if (moved) 
                {
                    check_traffic_violation(moved);
                    check_passed_obstacles(posX, posY);
                }
				break;
		}
	}
	return (0);
}
