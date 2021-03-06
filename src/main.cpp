#include  <SDL.h>
#include <stdio.h>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error Requires SDL 2.0.17! Aborting!
#endif

//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		}
		else
		{
			SDL_Event e;
			while (true)
			{
				SDL_PollEvent(&e);
				if (e.type == SDL_QUIT)
				{
					SDL_Log("Program quit after %i ticks", e.quit.timestamp);
					break;
				}
				screenSurface = SDL_GetWindowSurface(window);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
				SDL_UpdateWindowSurface(window);

			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
