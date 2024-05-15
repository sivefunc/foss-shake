#include "SDL.h"
#include <stdbool.h> // true and false

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
bool initialize_window_and_renderer(SDL_Renderer **renderer, SDL_Window **window)
{
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return false;
    }

    *window = SDL_CreateWindow
        (
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_FULLSCREEN_DESKTOP |
            SDL_WINDOW_BORDERLESS
        );
    
    if (!window)
    {
        return false;
    }
    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (!renderer)
    {
        return false;
    }
    return true;
}
