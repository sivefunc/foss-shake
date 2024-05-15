#pragma once
typedef struct Selector_Board
{
    int16_t rows;
    int16_t columns;
    int16_t **board;
} Selector_Board;

// selects a level from a list of files arranged in a NxM matrix ascendently.
// returns -1 in case of using pressing QUIT key.
int16_t select_level(
        int16_t file_count,
        int16_t last_level_played,
        Selector_Board *selector_board,
        SDL_Window *window, SDL_Renderer *renderer, Textures *textures,
        AudioChunks *audio_chunks
        );
