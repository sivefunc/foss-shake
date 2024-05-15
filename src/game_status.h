#pragma once
typedef struct Game_Status
{
    Player player_1;
    Player player_2;
    Activators activators;
    Board game_board;
} Game_Status;

Game_Status copy_game_status(Game_Status game_status);
void free_game_status(Game_Status game_status);
void free_games_status(int16_t size, Game_Status *games_status);
int16_t append_game_status(
        Game_Status game_status, int16_t size, Game_Status **games_status);

int16_t pop_game_status(int16_t size, Game_Status **games_status);
