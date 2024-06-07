#include "../dependacies/header.h"

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
				break;
		}
	}
	return (0);
}
