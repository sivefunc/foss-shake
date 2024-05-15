#pragma once
typedef struct AudioChunks
{
    Mix_Chunk *background;
    Mix_Chunk *unsuccessful_move;
    Mix_Chunk *successful_move;
    Mix_Chunk *button_move;
    Mix_Chunk *win_move;
    Mix_Chunk *undo_move;
    Mix_Chunk *restart_move;
    Mix_Chunk *quit_move;
    Mix_Chunk *start_move;
} AudioChunks;

typedef struct Textures
{
    SDL_Texture *background_texture;
    SDL_Texture *box_texture;
    SDL_Texture *hand_left_player;
    SDL_Texture *body_left_player;
    SDL_Texture *arm_extended_left_player;
    SDL_Texture *arm_corner_left_player;
    SDL_Texture *hand_pressing_button_left_player;
    SDL_Texture *hand_right_player;
    SDL_Texture *body_right_player;
    SDL_Texture *arm_extended_right_player;
    SDL_Texture *arm_corner_right_player;
    SDL_Texture *hand_pressing_button_right_player;
    SDL_Texture *floor_texture;
    SDL_Texture *border_texture;
    SDL_Texture *green_button_pressed_texture;
    SDL_Texture *green_button_unpressed_texture;
    SDL_Texture *purple_button_pressed_texture;
    SDL_Texture *purple_button_unpressed_texture;
    SDL_Texture *door_open_texture;
    SDL_Texture *door_closed_texture;
    SDL_Texture *hand_shake_left_player;
    SDL_Texture *hand_shake_right_player;

    SDL_Texture *hand_cut;
    SDL_Texture *welcome_logo_texture;
    SDL_Texture *background_shape;
    SDL_Texture *select_board_completed;
    SDL_Texture *select_board_uncompleted;
    SDL_Texture *select_board_playing_completed;
    SDL_Texture *select_board_playing_uncompleted;

} Textures;
