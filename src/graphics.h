#pragma once
// renders onto window the game board incluiding players, activators
// and background.
typedef struct BG_Dots
{
    int16_t rows;
    int16_t columns;
    int16_t increment;
    SDL_Surface *image;
} BG_Dots;
void game_board_graphics(
        int16_t game_rows,
        int16_t game_columns,
        Fields **game_board,
        Player *player_1,
        Player *player_2,
        SDL_Window *window,
        SDL_Renderer *renderer,
        Textures *textures,
        char *level_description
        );

// renders onto window the level selector incluiding the numbers
// background and hovering effect.
void level_selector_graphics(
        int16_t rows,
        int16_t columns,
        int16_t **selection_board,
        int16_t levels,
        SDL_Window *window,
        SDL_Renderer *renderer,
        Textures *textures
        );

// Renders onto window the welcome text, logo and instructions to quit
// and enter the game.
void welcome_graphics(
        Textures *textures, SDL_Window *window, SDL_Renderer *renderer);
