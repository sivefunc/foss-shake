#include <stdbool.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "../sdl_types.h"

void initialize_audio(AudioChunks *chunks)
{

    Mix_OpenAudio(
            48000,                  // Frequency hz
            MIX_DEFAULT_FORMAT,     // Format
            2,                      // Stereo channel
            2048);                  // Chunksize

    chunks -> background = Mix_LoadWAV("res/music/background_music.ogg");

    chunks -> unsuccessful_move = Mix_LoadWAV(
            "res/music/unsuccessful_music.wav");

    chunks -> successful_move = Mix_LoadWAV("res/music/normal_music.wav");

    chunks -> undo_move = Mix_LoadWAV("res/music/undo_music.wav");

    chunks -> button_move = Mix_LoadWAV("res/music/button_music.wav");

    chunks -> win_move = Mix_LoadWAV("res/music/win_music.wav");

    chunks -> restart_move = Mix_LoadWAV("res/music/restart_music.wav");
    chunks -> quit_move = Mix_LoadWAV("res/music/quit_music.mp3");
    chunks -> start_move = Mix_LoadWAV("res/music/start_music.mp3");



}
