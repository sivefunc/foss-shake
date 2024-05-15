// Standard libraries
#include <stdlib.h>             // Memory allocation
#include <stdio.h>
#include <stdbool.h>            // bool, true, false.
#include <math.h>               // sqrt, fmin.

// SDL libraries
#include "SDL.h"                // Graphics library.
#include "SDL_image.h"          // Rendering images xcf and .png.
#include "SDL_ttf.h"            // Rendering .ttf text.
#include "SDL_mixer.h"          // Loading music (Mix_Chunk*) in .wav and .ogg.

// Game definitions
#include "game_types.h"         // Player, Activators and Fields typedef structs.
#include "sdl_types.h"          // Textures and AudioChunks typedef structs.

#include "initializers.h"       // initialize_level(), initialize_textures(),
                                // initialize_window_and_renderer() and
                                // initialize_audio().

#include "welcome_intro.h"      // welcome_intro() to play initial scene.

#include "matrix_legend.h"      // Constants like PLAYER_1, QUIT, RESTART,
                                // WIN_MOVE.

#include "get_files.h"          // Gets all regular files on a directory
                                // sorted ascendently.

#include "select_level.h"       // select_level() to select the level_idx
                                // from Selector_Board typedef struct.

#include "play_level.h"         // reads level from file and plays it using
                                // play_level() function.

#include "graphics.h"           // game_board_graphics, welcome_intro_graphics,
                                // BG_Dots struct and more.

// Global variable that will be accessed by background_graphics function
// on graphics.c to allow generate the moving shape background.
BG_Dots bg_dots;

void destroy_app(SDL_Renderer *renderer, SDL_Window *window);
int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Textures game_textures;         // typedef struct containing a lot of
                                    // SDL_Texture * members.

    AudioChunks audio_chunks;       // typedef struct containing a lot of
                                    // Mix_Chunk * members.

    bg_dots.rows = 5;
    bg_dots.columns = 10;
    bg_dots.increment = 0; // Distance of pixels that each shape has to be
                           // moved from original place.
    bg_dots.image = IMG_Load("res/Background/background_shape.xcf");

    // Regular files inside argv[1] directory.
    char **files = NULL;
    int16_t file_count = -1; 
    
    //--------------------------------------------------//
    // Initialization                                   //
    // -------------------------------------------------//
    
    if (argc != 2)
    {
        fprintf(stderr, "You need to run it as: ./PROG_NAME folder/\n");
        return -1;
    }

    // get's the regular files and corresponding count inside the directory
    // on argv[1], if no regular files are found or no directory returns -1.
    file_count = get_files(argv[1], &files);

    // No files / No directory found.
    if (file_count <= 0)
    {
        fprintf(stderr, "No regular files or directory doesn't exist\n");
        return -1;
    }

    // true if window and renderer is not null else false.
    bool initialized = initialize_window_and_renderer(&renderer, &window); 
    if (initialized == false)
    {
        fprintf(stderr, "Couldn't initialize window, exiting...\n");
        destroy_app(renderer, window);
        return -1;
    }
    
    // This take some time to load
    // maybe on future add loading screen?
    initialize_textures(renderer, &game_textures);
    initialize_audio(&audio_chunks);

    
    // -----------------------------------------------------------------//
    // Creates the selector level NxM board where the levels will be    //
    // displayed to be selected by the user                             //
    // -----------------------------------------------------------------//
    
    Selector_Board selector_board;
    // Find the closest perfect square <= file_count, we want to make the
    // selection board to be as close as a square as possible
    selector_board.rows = sqrt(file_count);

    // Normally file_count won't be a perfect square then rows != columns
    // so we are going to make it a rectangle by adding the left over as
    // new columns where each column is of size {selector_rows}
    // this is the same as saying ceil(file_count / rows)
    selector_board.columns = (file_count + selector_board.rows - 1) /
        selector_board.rows;

    // Note: We use ceil because we want rectangle with integer sides
    // This will make the {Area_of_rectangle} >= {file_count}
    // So we will ignore the extra that could have. (left blank)

    selector_board.board = (int16_t **) calloc(
            selector_board.rows, sizeof(int16_t*));

    for (int16_t i = 0; i < selector_board.rows; i++)
        selector_board.board[i] = (int16_t *)calloc(selector_board.columns,
                sizeof(int16_t));

    // by default the first level that shows selected on the select_board
    // is the one on level_idx 0.
    int16_t level_idx = 0;
    int16_t level_y = level_idx / selector_board.columns;
    int16_t level_x = level_idx % selector_board.columns;
    selector_board.board[level_y][level_x] = SELECT_BOARD_PLAYING_UNCOMPLETED;

    //----------------------------//
    // Running the Handshake game //
    //----------------------------//

    Mix_PlayChannel(-1, audio_chunks.background, -1);
    bool app_is_running = welcome_intro(&game_textures, window, renderer);
    int16_t result_of_playing_level;
    while (app_is_running)
    {
        // selects a level from a list of files arranged in a NxM matrix
        // ascendently.
        // returns -1 in case of using pressing QUIT key.
        level_idx = select_level(
            file_count, level_idx, &selector_board,
            window, renderer, &game_textures, &audio_chunks);
       
        // Quit
        if (level_idx == -1)
        {
            app_is_running = false;
        }

        else
        {
            result_of_playing_level = play_level(
                files[level_idx],
                window, renderer, &game_textures, &audio_chunks);

            if (result_of_playing_level == WIN_MOVE)
            {
                level_y = level_idx / selector_board.columns;
                level_x = level_idx % selector_board.columns;
                selector_board.board[level_y][level_x] =
                    SELECT_BOARD_PLAYING_COMPLETED;
            }
        }
    }
    destroy_app(renderer, window);
	return 0;
}

// Destroys the window and the renderer
void destroy_app(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
