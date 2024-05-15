#pragma once
int16_t body_move(
        int16_t move,
        Fields **game_board,
        Player *player_pushing,
        Player *player_being_pushed,
        Activators *activators
        );
