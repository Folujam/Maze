#ifndef HEADER_H
#define HEADER_H

#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>

/*constants*/
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MINI_CELL_SIZE 4
#define MINI_MAP_WIDTH (MAP_WIDTH * MINI_CELL_SIZE)
#define MINI_MAP_HEIGHT (MAP_HEIGHT * MINI_CELL_SIZE)
#define MINI_MAP_PADDING 10


/**
* struct SDL_instance- struct to hold window && renderer
* this struct handles the sdlinstance to be shown on screen
* @window: a window to be created
* @renderer: vector to visualizer
*/
typedef struct SDL_instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

//global player	variable declarations (extern)
extern double posX, posY;
extern double dirX, dirY;
extern double planeX, planeY;
extern double moveSpeed, rotSpeed;

// Map declaration (extern)
extern int worldMap[MAP_WIDTH][MAP_HEIGHT];

// Function declarations (prototypes)
int init_instance(SDL_Instance *instance);
int poll_events(void);
void render_scene(SDL_Instance *instance);


#endif
