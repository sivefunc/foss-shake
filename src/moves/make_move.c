// Standard libraries
#include <inttypes.h>       // Exact width integers
#include <stdbool.h>        // true and false

// Game definitions
#include "../game_types.h"
#include "../matrix_legend.h"

// Differents move of players
#include "none_move.h"
#include "backward_move.h"
#include "object_move.h"
#include "body_move.h"

#include "check_activators.h"

int16_t make_move(
        int16_t move,
        Player *player_moving,
        Player *player_not_moving,
        Activators *activators,
        int16_t game_rows,
        int16_t game_columns,
        Fields **game_board)
{

    // Coordinates of the hand of user.
    int16_t Y = player_moving -> hand_last_y;
    int16_t X = player_moving -> hand_last_x;
    
    // Movement on the Y or X coordinate according to the MOVE key
    int32_t X_M, Y_M;
    X_M = (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y_M = (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);
    
    if (Y + Y_M < 0 || Y + Y_M >= game_rows ||
        X + X_M < 0 || X + X_M >= game_columns)
        return UNSUCCESSFUL_MOVE;

    
    // Do move according to the object that is on the new place where hand is
    // going to.

    int16_t result;
    switch (game_board[Y + Y_M][X + X_M].object)
    {
        // Activator and surface doesn't contain an object on top of it
        case NONE:
            {
                // He spent all his available moves
                if (player_moving -> moves_spent ==
                        player_moving -> moves_total)
                    result = UNSUCCESSFUL_MOVE;

                // Checking if hand will be on top of a closed door
                else if (game_board[Y + Y_M][X + X_M].activator == DOOR_CLOSED)
                    result = UNSUCCESSFUL_MOVE;
                
                else
                {
                    // Moves the hand
                    none_move(move, game_board, player_moving);

                    // The user was pressing a purple button
                    if (game_board[Y][X].activator == PURPLE_BUTTON_PRESSED ||
                        game_board[Y][X].activator == PURPLE_BUTTON_UNPRESSED)

                        // Change states of purple buttons and doors attached
                        // to it and checks if hand got cut.
                        check_activators(
                                X, Y,
                                game_board,
                                activators, player_moving,
                                player_not_moving);

                    // Checks if hand new position will press an activator.
                    // Change states of activators and doors attached to it and
                    // checks if hand got cut.
                    check_activators(
                            X + X_M, Y + Y_M,
                            game_board, activators,
                            player_moving, player_not_moving);


                    result = SUCCESSFUL_MOVE;
                }
                break;
            }

        // Handshake (The player hand is going to another player hand)
        case HAND_NORMAL: case HAND_PRESSING:
            {
                int16_t last_mv = player_not_moving -> moves[
                    player_not_moving -> moves_spent - 1];

                if (player_moving -> moves_spent ==
                        player_moving -> moves_total)
                {
                    result = UNSUCCESSFUL_MOVE;
                }
                
                // It's the opposite move.
                else if ((move == MOVE_LEFT && last_mv == MOVE_RIGHT) ||
                    (move == MOVE_RIGHT && last_mv == MOVE_LEFT) ||
                    (move == MOVE_DOWN && last_mv == MOVE_UP) ||
                    (move == MOVE_UP && last_mv == MOVE_DOWN))
                {
                    // Remove the last move of player_not_moving because
                    // the player_moving is going to be there
                    // It's unnecesary at gameplay but useful for graphics.c
                    // to allow rendering handshake.
                    player_not_moving -> moves[
                        player_not_moving -> moves_spent - 1] = '\0';

                    none_move(
                            move,
                            game_board,
                            player_moving);

                    game_board[Y + Y_M][X + X_M].object = HANDSHAKE;
                    result = WIN_MOVE;
                }
                else
                {
                    result = UNSUCCESSFUL_MOVE;
                }
                
                break;
            }

        // Object moving
        case BOX: case HAND_CUT:
            {
                // Player spent all his available moves
                if (player_moving -> moves_spent == 
                        player_moving -> moves_total)
                    result = UNSUCCESSFUL_MOVE;
                
                // The object can't land on another object, only NONE
                else if (game_board[Y + Y_M*2][X + X_M * 2].object != NONE)
                    result = UNSUCCESSFUL_MOVE;

                // The object can't move to a closed door
                else if (game_board[Y + Y_M*2][X + X_M * 2].activator ==
                     DOOR_CLOSED)
                    result = UNSUCCESSFUL_MOVE;
                
                else
                {
                    object_move(move, game_board, player_moving);

                    // The user was pressing a purple button
                    if (game_board[Y][X].activator == PURPLE_BUTTON_PRESSED ||
                        game_board[Y][X].activator == PURPLE_BUTTON_UNPRESSED)

                        // Change states of purple buttons and doors attached
                        // to it and checks if hand got cut.
                        check_activators(
                                X, Y,
                                game_board,
                                activators,
                                player_moving, player_not_moving);
                    
                    // Checking if object is on top of an activator
                    check_activators(
                            X + X_M * 2, Y + Y_M * 2,
                            game_board,
                            activators,
                            player_moving, player_not_moving);

                    result = SUCCESSFUL_MOVE;
                }

                break;
            }

        case ARM_HOR: case ARM_VER: case ARM_LEFT_DOWN: case ARM_LEFT_UP:
        case ARM_RIGHT_UP: case ARM_RIGHT_DOWN:
            {
                int16_t last_x = player_moving -> hand_last_x;
                int16_t last_y = player_moving -> hand_last_y;
                int16_t last_mv = player_moving -> moves[
                    player_moving -> moves_spent - 1];

                last_x -= (last_mv == MOVE_LEFT) ? -1 : 
                            (last_mv == MOVE_RIGHT ? 1 : 0);

                last_y -= (last_mv == MOVE_UP) ? -1 :
                            (last_mv == MOVE_DOWN ? 1 : 0);
                
                result = UNSUCCESSFUL_MOVE;
                
                // Undo if the where the player is going to is the same
                // as the player past location and such past location is not
                // the initial body of player.
                if ((last_x == X + X_M && last_y == Y + Y_M) &&
                    (last_x != player_moving -> body_init_x ||
                        last_y != player_moving -> body_init_y))
                {
                    result = SUCCESSFUL_MOVE;
                    backward_move(move, game_board, player_moving);

                    check_activators(
                                    X + X_M, Y + Y_M, game_board,
                                    activators,
                                    player_moving, player_not_moving);

                    if (game_board[Y][X].activator == PURPLE_BUTTON_PRESSED ||
                        game_board[Y][X].activator == PURPLE_BUTTON_UNPRESSED)
                    {
                        check_activators(
                                X, Y,
                                game_board, activators,
                                player_moving, player_not_moving);
                    }
                }
                break;
            }

        case BODY:
            {
                // He spent all his available moves
                if (player_moving -> moves_spent ==
                        player_moving -> moves_total)
                    result = UNSUCCESSFUL_MOVE;
                
                // Player can't move itself
                else if (Y + Y_M == player_moving -> body_init_y && 
                    X + X_M == player_moving -> body_init_x)
                    result = UNSUCCESSFUL_MOVE;

                else
                {
                    // This function does more checks
                    // So it's the only function that returns value.
                    result = body_move(
                            move,
                            game_board,
                            player_moving, player_not_moving,
                            activators);
                }

                break;
            }

        default:
            {
                result = UNSUCCESSFUL_MOVE;
                break;
            }
    }
    return result;
}
