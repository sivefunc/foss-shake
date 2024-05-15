#include "SDL.h"
#include "SDL_mixer.h"          // Audio
#include <stdbool.h>            // bool, true and false.

#include "matrix_legend.h"      // constants like QUIT
#include "game_types.h"         // Player, Activators.
#include "sdl_types.h"          // Textures and AudioChunks
#include "make_move.h"          // object_move, none_move, backward and
                                // activators checking.
#include "initializers.h"       // Initialize level
#include "get_key.h"            // Key presses of the user.
#include "graphics.h"           // Renders the game into screen using SDL.
#include "game_status.h"        // 
#include "fps.h"                // Frames per second game should run

// Global variables used in check_activators
bool DOOR_CHANGED_STATE;    // If a button was pressed or stop being pressed
                            // in case of purples, useful for sound

bool RESTART_LEVEL;         // If door cut's the body of a player or
                            // hand can't go back (moves_spent == 1)
int16_t play_level(
        char *level,
        SDL_Window *window, 
        SDL_Renderer *renderer,
        Textures *game_textures,
        AudioChunks *audio_chunks
        )
{
    // NxM board where the game will be played
    int16_t game_rows = 0, game_columns = 0;
    Fields **game_board = NULL;

    // Related to gameplay
    int16_t result = 0;
    int16_t player_moving = 0;
    int16_t key = 0;

    // Players
    Player player_1, player_2;

    //
    RESTART_LEVEL = false;
    DOOR_CHANGED_STATE = false;

    // Buttons of the level and doors that get activated by these
    // Button and door types incluiding states and whether or not they exist.
    Activators activators = 
    {
        81,      // Total purple buttons
        81,      // Total green buttons
        81+81,     // Total doors
        4,
        NULL,
        NULL,
        NULL
    };

    // Reads the contents of the file to create the level
    char * level_description = NULL;
    level_description = initialize_level(
        level,
        &player_1, &player_2,
        &activators, &game_rows, &game_columns, &game_board);

    // Creates array of game status
    Game_Status *games_status = (Game_Status*)calloc(1, sizeof(Game_Status));
    int16_t games_status_count = 1;

    // Add a game_status
    games_status[0] = (Game_Status){player_1, player_2, activators,
        (Board){game_rows, game_columns, game_board}};

    bool playing_level = true;
    game_board_graphics(
                games_status[games_status_count - 1].game_board.rows,
                games_status[games_status_count - 1].game_board.columns,
                games_status[games_status_count - 1].game_board.board,
                &(games_status[games_status_count - 1].player_1), 
                &(games_status[games_status_count - 1].player_2),
                window, renderer,
                game_textures,
                level_description
                );

    int32_t last_frame_time;
    int32_t time_to_wait;
    int32_t delay;
    while (playing_level)

    {

        last_frame_time = SDL_GetTicks();
        player_moving = NO_PLAYER;
        player_moving = get_key(&key);
        if (player_moving == NO_PLAYER)
        {
            if (key == QUIT)
            {
                Mix_PlayChannel(-1, audio_chunks -> quit_move, 0);
                playing_level = false;
            }

            else if (key == RESTART)
            {
                Mix_PlayChannel(-1, audio_chunks -> restart_move, 0);

                // Frees array
                free_games_status(games_status_count, games_status);
                free(level_description);
                games_status = (Game_Status*)calloc(1, sizeof(Game_Status));
                games_status_count = 1;

                // Reads the contents of the file to create the level
                level_description = initialize_level(
                    level,
                    &player_1, &player_2,
                    &activators, &game_rows, &game_columns,
                    &game_board);

                games_status[0] = (Game_Status){player_1, player_2, activators,
                    (Board){game_rows, game_columns, game_board}};
            }
            
            else if (key == UNDO && games_status_count != 1)
            {
                Mix_PlayChannel(-1, audio_chunks -> undo_move, 0);
                games_status_count = pop_game_status(
                    games_status_count,
                    &games_status);
            }
        }

        else 
        {
            games_status_count = append_game_status(
                copy_game_status(games_status[games_status_count - 1]),
                games_status_count,
                &games_status);

            Game_Status new_game = games_status[games_status_count - 1];
            Player *p_moving, *p_not_moving;
            if (player_moving == PLAYER_1)
            {
                p_moving = &new_game.player_1;
                p_not_moving = &new_game.player_2;
            }

            else
            {
                p_moving = &new_game.player_2;
                p_not_moving = &new_game.player_1;

            }
            result = make_move(
                    key,
                    p_moving,
                    p_not_moving,
                    &new_game.activators,
                    new_game.game_board.rows,
                    new_game.game_board.columns,
                    new_game.game_board.board);
            

            games_status[games_status_count - 1] = new_game;
            if (result == UNSUCCESSFUL_MOVE)
            {
                Mix_PlayChannel(-1, audio_chunks -> unsuccessful_move, 0);

                games_status_count = pop_game_status(
                    games_status_count,
                    &games_status);
            }
           
            else if (DOOR_CHANGED_STATE)
            {
                Mix_PlayChannel(-1, audio_chunks -> button_move, 0);
                DOOR_CHANGED_STATE = false;
            }

            else
            {
                Mix_PlayChannel(-1, audio_chunks -> successful_move, 0);

            }
        }

        if (RESTART_LEVEL)
        {
            Mix_PlayChannel(-1, audio_chunks -> restart_move, 0);
            RESTART_LEVEL = false;

            // Frees array
            free_games_status(games_status_count, games_status);
            free(level_description);
            games_status = (Game_Status*)calloc(1, sizeof(Game_Status));
            games_status_count = 1;

            // Reads the contents of the file to create the level
            level_description = initialize_level(
                level,
                &player_1, &player_2,
                &activators, &game_rows, &game_columns,
                &game_board);

            games_status[0] = (Game_Status){player_1, player_2, activators,
                (Board){game_rows, game_columns, game_board}};
        }

        game_board_graphics(
                games_status[games_status_count - 1].game_board.rows,
                games_status[games_status_count - 1].game_board.columns,
                games_status[games_status_count - 1].game_board.board,
                &(games_status[games_status_count - 1].player_1), 
                &(games_status[games_status_count - 1].player_2),
                window, renderer,
                game_textures,
                level_description
                );

        delay = SDL_GetTicks() - last_frame_time;
        time_to_wait = MS_PER_FRAME - delay;
        if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
            SDL_Delay(time_to_wait);

        if (result == WIN_MOVE)
        {
            Mix_PlayChannel(-1, audio_chunks -> win_move, 0);

            playing_level = false;
            uint32_t delay_in_ms = 1000;
            uint32_t start_frame_time = SDL_GetTicks();
            while (SDL_GetTicks() - start_frame_time < delay_in_ms)
            {
                last_frame_time = SDL_GetTicks();
                game_board_graphics(
                    games_status[games_status_count - 1].game_board.rows,
                    games_status[games_status_count - 1].game_board.columns,
                    games_status[games_status_count - 1].game_board.board,
                    &(games_status[games_status_count - 1].player_1), 
                    &(games_status[games_status_count - 1].player_2),
                    window, renderer,
                    game_textures,
                    level_description
                    );

                time_to_wait = MS_PER_FRAME - (SDL_GetTicks() - last_frame_time);
                if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
                {
                    SDL_Delay(time_to_wait);
                }
            }
        }
        else
        {
            key = false, result = false;
        }
    }
    // Frees array
    free_games_status(games_status_count, games_status);
    free(level_description);
    return result;
}
