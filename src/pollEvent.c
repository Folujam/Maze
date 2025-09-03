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
                return 1;

            case SDL_KEYDOWN:
                key = event.key;

                if (key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return 1;

                /* Track if player actually moved this frame */
                int moved = 0;

                /* Forward (W) */
                if (key.keysym.scancode == SDL_SCANCODE_W) {
                    double nx = posX + dirX * moveSpeed;
                    double ny = posY + dirY * moveSpeed;
                    if (worldMap[(int)nx][(int)posY] == 0) { posX = nx; moved = 1; }
                    if (worldMap[(int)posX][(int)ny] == 0) { posY = ny; moved = 1; }
                }

                /* Backward (S) */
                if (key.keysym.scancode == SDL_SCANCODE_S) {
                    double nx = posX - dirX * moveSpeed;
                    double ny = posY - dirY * moveSpeed;
                    if (worldMap[(int)nx][(int)posY] == 0) { posX = nx; moved = 1; }
                    if (worldMap[(int)posX][(int)ny] == 0) { posY = ny; moved = 1; }
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

                /* Strafe Left (Q) */
                if (key.keysym.scancode == SDL_SCANCODE_Q) {
                    double nx = posX - planeX * moveSpeed;
                    double ny = posY - planeY * moveSpeed;
                    if (worldMap[(int)nx][(int)posY] == 0) { posX = nx; moved = 1; }
                    if (worldMap[(int)posX][(int)ny] == 0) { posY = ny; moved = 1; }
                }

                /* Strafe Right (E) */
                if (key.keysym.scancode == SDL_SCANCODE_E) {
                    double nx = posX + planeX * moveSpeed;
                    double ny = posY + planeY * moveSpeed;
                    if (worldMap[(int)nx][(int)posY] == 0) { posX = nx; moved = 1; }
                    if (worldMap[(int)posX][(int)ny] == 0) { posY = ny; moved = 1; }
                }

                /* Post-move hooks */
                if (moved) {
                    notify_player_moved();          /* –2 if red */
                    try_collect_spirit(posX, posY); /* +3 on collection */
                }
                break;
        }
    }
    return 0;
}
