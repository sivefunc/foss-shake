#include <stdbool.h>
#include <inttypes.h>
#include "../game_types.h"
#include "../matrix_legend.h"

// Global variables located on play_level
extern bool DOOR_CHANGED_STATE;    // If a button was pressed or stop being
                                   // pressed in the case of purples

extern bool RESTART_LEVEL;         // If door cut's the body of a player or
                                   // hand can't go back (moves_spent == 1)
int16_t get_button_type(
        int16_t X, int16_t Y, int16_t total_buttons,
        int16_t **buttons);

void activate_doors(
    int16_t type, Fields **game_board,
    Activators *activators,
    Player *player_moving, Player *player_not_mvong);

void cut_player_hand(Player *player, Fields **game_board);
bool is_door_cutting_hand(int16_t Dy, int16_t Dx, Player *player);

void check_activators(
        int16_t X, int16_t Y,
        Fields **game_board,
        Activators *activators,
        Player *player_moving, Player *player_not_moving)
{
    int16_t activator = game_board[Y][X].activator;
    if (activator == GREEN_BUTTON_UNPRESSED ||
        activator == GREEN_BUTTON_PRESSED)
    {
        game_board[Y][X].activator = activator == GREEN_BUTTON_PRESSED
            ? GREEN_BUTTON_UNPRESSED : GREEN_BUTTON_PRESSED;

        if (game_board[Y][X].object == HAND_NORMAL)
            game_board[Y][X].object = HAND_PRESSING;

        int16_t type = get_button_type(
                X, Y,
                activators -> total_green,
                activators -> green_buttons);
        
        if (type != -1)
        {
            activate_doors(type, game_board, activators,
                    player_moving, player_not_moving);
        }
    }

    else if (activator == PURPLE_BUTTON_UNPRESSED ||
            activator == PURPLE_BUTTON_PRESSED)
    {
        game_board[Y][X].activator = activator == PURPLE_BUTTON_PRESSED
            ? PURPLE_BUTTON_UNPRESSED : PURPLE_BUTTON_PRESSED;

        if (game_board[Y][X].object == HAND_NORMAL)
            game_board[Y][X].object = HAND_PRESSING;

        int16_t type = get_button_type(
                X, Y,
                activators -> total_purple,
                activators -> purple_buttons);

        if (type != -1)
        {
            activate_doors(type, game_board, activators,
                    player_moving, player_not_moving);
        }

    }
}

void activate_doors(
    int16_t type, Fields **game_board,
    Activators *activators,
    Player *player_moving, Player *player_not_moving)
{
    for (int16_t i = 0; i < activators -> total_doors; i++)
    {
        if (activators -> doors[i][0] == type)
        {
            int16_t _X = activators -> doors[i][1];
            int16_t _Y = activators -> doors[i][2];

            DOOR_CHANGED_STATE = true;

            if (game_board[_Y][_X].activator == DOOR_OPEN)
            {
                game_board[_Y][_X].activator = DOOR_CLOSED;
                if (is_door_cutting_hand(_Y, _X, player_moving))
                {
                    cut_player_hand(player_moving, game_board); 
                }

                if (is_door_cutting_hand(_Y, _X, player_not_moving))
                {
                    cut_player_hand(player_not_moving, game_board);
                }

                if ((player_not_moving -> body_init_y == _Y &&
                        player_not_moving -> body_init_x == _X) ||
                    (player_not_moving -> hand_last_y == _Y &&
                        player_not_moving -> hand_last_x == _X) ||
                    (player_moving -> body_init_y == _Y &&
                        player_moving -> body_init_x == _X) ||
                    (player_moving -> hand_last_y == _Y &&
                        player_moving -> hand_last_x == _X))
                {
                    RESTART_LEVEL = true;
                }

                
                game_board[_Y][_X].object = NONE;
                game_board[_Y][_X].object_orientation = NONE;
            }

            else
            {
                game_board[_Y][_X].activator = DOOR_OPEN;
            }
        }
    }
}

int16_t get_button_type(
        int16_t X, int16_t Y, int16_t total_buttons,
        int16_t **buttons)
{
    int16_t button_type = -1;
    for (int16_t button = 0; button < total_buttons; button++)
    {
        // Check if the button on the game_board is equal to the button
        // on the Activators struct.
        if (buttons[button][1] == X && buttons[button][2] == Y)
        {
            button_type = buttons[button][0];
            break;
        }
    }
    return button_type;
}

bool is_door_cutting_hand(int16_t Dy, int16_t Dx, Player *player)
{
    int16_t Px = player -> body_init_x;
    int16_t Py = player -> body_init_y;
    int16_t move;

    for (int16_t i = 0; i < player -> moves_total; i++)
    {
        move = player -> moves[i];
        Px += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
        Py += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);
        if (Dy == Py && Dx == Px)
            return true;
    }
    return false;

}

void cut_player_hand(Player *player, Fields **game_board)
{
    int16_t Px = player -> hand_init_x;
    int16_t Py = player -> hand_init_y;
    int16_t move;
    for (int16_t i = 1; i < player -> moves_total; i++)
    {
        move = player -> moves[i];
        Px += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
        Py += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);
        game_board[Py][Px].object = NONE;
        player -> moves[i] = '\0';
    }
    game_board[player -> hand_last_y][player -> hand_last_x].object = HAND_CUT;
    player -> hand_last_x = player -> hand_init_x;
    player -> hand_last_y = player -> hand_init_y;
    game_board[player -> hand_last_y][player -> hand_last_x].object = HAND_NORMAL;
    player -> moves_spent = 1;
}
