#include "../dependscies/header.h"

/**
* main - entry point
* Return: int (success(0))
*/
int main(void)
{
	SDL_Instance instance;

	if (init_instance(&instance) != 0)
		return (1);
	/*each loop represents a frame*/
	while (True)
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		/*clear the renderer*/
		SDL_RenderClear(instance.renderer);
		if (poll_events() == 1)
			break;
		/*draw stuff on renderer*/
		/*flush the renderer*/
		SDL_RenderPresent(instance.renderer);
	}
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();
	return (0);
}
