#pragma once
int16_t make_move(
        int16_t move,
        Player *player_moving,
        Player *player_not_moving,
        Activators *activators,
        int16_t game_rows,
        int16_t game_columns,
        Fields **game_board);
