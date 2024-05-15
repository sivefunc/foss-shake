#include "SDL.h"       // Graphic library
#include "SDL_mixer.h"
#include <stdio.h>
#include <inttypes.h>       // Support for uintN_t
#include <stdbool.h>        // Support for true, false and bool macros

#include "game_types.h"     // Player, Activators, Textures, Fields
#include "sdl_types.h"
#include "matrix_legend.h"  // Definitions related to game, e.g: PLAY_LEVEL
#include "graphics.h"       // Level selector graphics
#include "get_key.h"        // Keyboard presses
#include "fps.h"

typedef struct Selector_Board
{
    int16_t rows;
    int16_t columns;
    int16_t **board;
} Selector_Board;

/*
 * Function: select_level
 * ----------------------
 * selects a level from a list of files arranged in a NxM matrix ascendently.
 * this doesn't check that the files actual exist, it just returns a level
 * from a file_count.
 *
 * {0, 1, 2}
 * {3, 4, 5} -> example.
 * {6, 7, 8}
 *
 * Parameters:
 * -----------
 * file_count   : int16_t -> 0 ... 255 -> number of files aka levels
 *              : that the player can choose.
 *
 * window       : where it will be drawn the graphics.
 * renderer     : shows the graphics of the window.
 * textures     : collection of SDL_Texture related to graphics
 *
 * returns: the index where the level is located on the files directory
 *          returns -1 on error (if pressed quit)
 */
int16_t select_level(
        int16_t file_count,
        int16_t last_level_played,
        Selector_Board *selector_board,
        SDL_Window *window, SDL_Renderer *renderer, Textures *textures,
        AudioChunks *audio_chunks
        )
{
    int16_t key = 0;            // Where it will move or QUIT
    int16_t Y = last_level_played / selector_board -> columns;
    int16_t X = last_level_played % selector_board -> columns; 
    int16_t level = -1;          // No level selected
    bool selecting_level = true;

    int32_t last_frame_time;
    int32_t delay;
    int32_t time_to_wait;

    // Draws a grid into window using renderer where the 
    // numbers are ascending.
    level_selector_graphics(
            selector_board -> rows,
            selector_board -> columns,
            selector_board -> board, file_count,
            window, renderer, textures);

    while (selecting_level)
    {
        last_frame_time = SDL_GetTicks();

        // Key presses of the user [Left, Up, Right, Down, Quit or restart]
        get_key(&key);

        if (key == ENTER)
        {
            // Idx number (start at 0), we do Y * columns to get how many
            // squares it has passed then add X to get the column in which
            // it is.
            Mix_PlayChannel(-1, audio_chunks -> start_move, 0);
            level = Y * selector_board -> columns + X;
            break;
        }

        else if (key == QUIT)
        {
            break; // Player didn't select a level: -1
        }

        // Temp variables to store Y and X
        int16_t _Y = Y, _X = X;

        // Only one coord will change his value it could be X or Y
        // We use ternary to avoid using 4 if statements to indicate
        // each direction (left, down, up, right)
        _X += (key == MOVE_LEFT) ? -1 : (key == MOVE_RIGHT ? 1 : 0);
        _Y += (key == MOVE_UP) ? -1 : (key == MOVE_DOWN ? 1 : 0);

        // Invalid move (out of boundaries)
        if ((_X < 0 || _X >= selector_board -> columns) ||
            (_Y < 0 || _Y >= selector_board -> rows))
        {/* Do nothing, just generate graphics and check fps*/}

        
        // Invalid move (Level > file_count doesn't exist, these are the extra
        // levels by making the grid a rectangle with integer sides)
        else if (_Y * selector_board -> columns + _X + 1 > file_count)
        {/* Do nothing, just generate graphics and check fps*/}

        else
        {
            selector_board -> board[Y][X] =  selector_board -> board[Y][X] ==
                    SELECT_BOARD_PLAYING_COMPLETED ? 
                SELECT_BOARD_COMPLETED :
                SELECT_BOARD_UNCOMPLETED;

            Y = (int16_t) _Y;
            X = (int16_t) _X;

            selector_board -> board[Y][X] = selector_board -> board[Y][X] ==
                    SELECT_BOARD_COMPLETED ? 
                SELECT_BOARD_PLAYING_COMPLETED :
                SELECT_BOARD_PLAYING_UNCOMPLETED;
        }

        // Draws a grid into window using renderer where the 
        // numbers are ascending.
        level_selector_graphics(
                selector_board -> rows,
                selector_board -> columns,
                selector_board -> board, file_count,
                window, renderer, textures);
        key = 0; 

        delay = SDL_GetTicks() - last_frame_time;
        time_to_wait = MS_PER_FRAME - delay;
        if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
            SDL_Delay(time_to_wait);

    }
    return level;
}
