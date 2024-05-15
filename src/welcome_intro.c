#include "SDL.h"
#include "SDL_mixer.h"
#include <inttypes.h>
#include <stdbool.h>

#include "matrix_legend.h"
#include "game_types.h"
#include "sdl_types.h"
#include "get_key.h"
#include "graphics.h"
#include "fps.h"

bool welcome_intro(Textures *textures,
        SDL_Window *window, SDL_Renderer *renderer)
{
    int16_t key;
    int16_t waiting = true;

    int32_t last_frame_time;
    int32_t delay;
    int32_t time_to_wait;
    welcome_graphics(textures, window, renderer);

    bool result;
    while (waiting)
    {
        last_frame_time = SDL_GetTicks();
        get_key(&key);
        if (key == ENTER)
        {
            result = true;
            break;
        }   
        else if (key == QUIT)
        {
            result = false;
            break;
        }

        welcome_graphics(textures, window, renderer);
        delay = SDL_GetTicks() - last_frame_time;
        time_to_wait = MS_PER_FRAME - delay;
        if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
            SDL_Delay(time_to_wait);
    }

    return result;
}
