#include <inttypes.h>
#include "SDL.h"
#include "matrix_legend.h"

/*
 * Function: get_key
 * ----------------------
 * Check for the events close window and key presses using SDL_PollEvent.
 *
 * Parameters:
 * -----------
 * key          : pointer to a int16_t constant that will be overwritten to
 *              : key pressed by user.
 *
 * returns: int16_t constant indicatng the category where key belongs.
 *          PLAYER_1 -> key moves left player
 *          PLAYER_2 -> key moves right player
 *          NO_PLAYER -> key is QUIT, UNDO, ENTER or RESTART
 *
 */
int16_t get_key(int16_t *key)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {
        switch (event.type)
        {
            case SDL_QUIT:
                *key = QUIT; // Quit the game
                break;

            case SDL_KEYDOWN:
                {
                    SDL_Keycode key_code = event.key.keysym.sym;
                    if (key_code == SDLK_ESCAPE || key_code == SDLK_q)
                    {
                        *key = QUIT; // Quit the game 
                        break;
                    }
                
                    else if (key_code == SDLK_RETURN)
                    {
                        *key = ENTER;
                        break;
                    }
                    else if (key_code == SDLK_r)
                    {
                        *key = RESTART; // Restart the game
                        break;
                    }
                    else if (key_code == SDLK_u)
                    {
                        *key = UNDO;
                        break;
                    }
                    else if (key_code == SDLK_w ||
                            key_code == SDLK_k ||
                            key_code == SDLK_UP)
                    {
                        *key = MOVE_UP; // UP
                         return (key_code == SDLK_w) ? PLAYER_1 : PLAYER_2;
                    }

                    else if (key_code == SDLK_s ||
                            key_code == SDLK_j ||
                            key_code == SDLK_DOWN)
                    {
                        *key = MOVE_DOWN; // DOWN
                        return (key_code == SDLK_s) ? PLAYER_1 : PLAYER_2;
                    }

                    else if (key_code == SDLK_d ||
                            key_code == SDLK_l ||
                            key_code == SDLK_RIGHT)
                    {
                        *key = MOVE_RIGHT; // RIGHT
                         return (key_code == SDLK_d) ? PLAYER_1 : PLAYER_2;
                    }
                    else if (key_code == SDLK_a ||
                            key_code == SDLK_h ||
                            key_code == SDLK_LEFT)
                    {
                        *key = MOVE_LEFT; // LEFT
                        return (key_code == SDLK_a) ? PLAYER_1 : PLAYER_2;
                    }
                }
        }
    }
    return NO_PLAYER; // Not play on this turn (Restart, quit or unknown)
}
