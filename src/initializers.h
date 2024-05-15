#pragma once
bool initialize_window_and_renderer(SDL_Renderer **renderer, SDL_Window **window);
void initialize_textures(SDL_Renderer *renderer, Textures *textures);
void initialize_audio(AudioChunks *chunks);
char * initialize_level(
        char *file_path,
        Player *player_1, Player *player_2,
        Activators *activators,
        int16_t *game_rows,
        int16_t *game_columns,
        Fields ***game_board);

