#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "../sdl_types.h"
void initialize_textures(
        SDL_Renderer *renderer,
        Textures *textures
        )
{
    // Left player
    textures -> hand_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/hand_left_player.xcf");

    textures -> body_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/body_left_player.xcf");
    
    textures -> arm_extended_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/arm_extended_left_player.xcf");

    textures -> arm_corner_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/arm_corner_left_player.xcf");

    textures -> hand_pressing_button_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/hand_pressing_button_left_player.xcf");

    // Right player
    textures -> hand_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/hand_right_player.xcf");

    textures -> body_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/body_right_player.xcf");
    
    textures -> arm_extended_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/arm_extended_right_player.xcf");

    textures -> arm_corner_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/arm_corner_right_player.xcf");

    textures -> hand_pressing_button_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/hand_pressing_button_right_player.xcf");

    // Loading all textures
    textures -> background_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/background.xcf");
    textures -> box_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/box.xcf");
    textures -> floor_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/floor.xcf");
    textures -> border_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/border.xcf");

    textures -> green_button_pressed_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/green_button_pressed.xcf");

    textures -> green_button_unpressed_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/green_button_unpressed.xcf");

    textures -> purple_button_pressed_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/purple_button_pressed.xcf");

    textures -> purple_button_unpressed_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/purple_button_unpressed.xcf");

    textures -> door_open_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/door_open.xcf");

    textures -> door_closed_texture = IMG_LoadTexture(renderer,
            "res/LevelPlaying/door_closed.xcf");
    
    textures -> hand_shake_left_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/handshake_left_player.xcf");

    textures -> hand_shake_right_player = IMG_LoadTexture(renderer,
            "res/LevelPlaying/handshake_right_player.xcf");   
    textures -> hand_cut = IMG_LoadTexture(renderer,
            "res/LevelPlaying/hand_cut.xcf");

    textures -> welcome_logo_texture = IMG_LoadTexture(renderer,
            "res/WelcomeIntro/welcome_logo.xcf");

    textures -> select_board_completed = IMG_LoadTexture(renderer,
            "res/LevelSelector/selection_level_completed.xcf");

    textures -> select_board_uncompleted = IMG_LoadTexture(renderer,
            "res/LevelSelector/selection_level_uncompleted.xcf");

    textures -> select_board_playing_uncompleted = IMG_LoadTexture(renderer,
            "res/LevelSelector/selection_level_playing_uncompleted.xcf");

    textures -> select_board_playing_completed = IMG_LoadTexture(renderer,
            "res/LevelSelector/selection_level_playing_completed.xcf");
}
