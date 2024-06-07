#include "../dependencies/header.h"

/**
* init_instance - the broad architecture of the program
* @instance: a giving of page on screen
* Return: int (0-Success/failure-others)
*/
int init_instance(SDL_Instance *instance)
{
	/*initializes SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "unable to initailize SDL: %s\n", SDL_GetError());
		return (1);
	}
	/*creates a new window instance*/
	instance->window = SDL_CreateWindow("SDL2 \\o/", SDL_WINDOWPOS_CENTERED,\
		SDL_WINDOWPOS_CENTERED, 1260, 720, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	/*Creates a new Renderer instance linked to the window*/
	instance->renderer = SDL_CreateRenderer(instance->window, -1,\
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTASYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "SDL_CreatrRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}
