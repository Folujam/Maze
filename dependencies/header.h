#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <stdio.h>
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

int init_instance(SDL_Instance *instance);
int poll_events(void);
#endif
