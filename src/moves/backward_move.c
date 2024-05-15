#include <inttypes.h>           // intN_t uintN_t
#include <stdbool.h>            // bool in game_types.h
#include "../matrix_legend.h"   // constants like HAND_NORMAL
#include "../game_types.h"      // typedef struct Fields, Activators, Player
#include "hand_and_arm.h"       // orientation according to new move
                                // and old direction.
/*
 * Function: backward_move
 * ----------------------
 * place hand into his old hand position.
 * update player stats and replace the new "past" position
 * of the hand to emptiness (set game_board[i][j].object and .orientation to
 * NONE)
 * 
 * e.g ( the 'E' is the hand)
 *
 * ===E (before)
 * ==E  (after)
 *
 * Here the hand got back to the old position and now the new considered old
 * position get's overwitten to NONE.
 *
 * Parameters:
 * -----------
 * move         : constant indicating the direction where it will move
 *              : backwards {LEFT, UP, DOWN, RIGHT}
 *
 * game_board   : Board where player plays and is a 2D array composed of 
 *              : Fields typedef struct, each Fields represent a cell that
 *              : stores {surface, activator and object}
 * 
 * player       : Player * -> pointer to the typedef struct player that whose
 *              : hand is going to be moved.
 * 
 * returns: void (nothing)
 *
 * note: It's assumed that Fields[i][j].object where i, j is the past position
 *          of the hand is the arm and not anything else.
 *
 *       It's assummed that player -> moves_spent > 1 (player must do
 *          another move aside from the initial where the hand is attached to
 *           body, redudancy of past statement to clarify things, because if
 *           it's equal to 1 we are getting out of bounds due to doing
 *           player -> moves[player -> moves_spent - 2])
 *
 *       It's assumed that player -> moves_spent < player -> moves_total.
 *
 *       Prior to calling this function all of that must be checked,
 *       make_move.c does that.
 *
 * note: player_stats updated are:
 *          -> int16_t *moves, last move at old_position is deleted set to '\0'
 *          -> int16_t moves_spent it decreases by 1.
 *          -> int16_t hand_last_x, hand_last_y these get set to the new
 *                  location of the hand. (it's the past location)`
 */
void backward_move(
        int16_t move,
        Fields **game_board,
        Player *player)
{
    int16_t orientation = hand_orientation(
            player -> moves[player -> moves_spent - 2]);

    // Place object and orientation to emptiness
    int16_t Y = player -> hand_last_y;
    int16_t X = player -> hand_last_x;
    game_board[Y][X].object = NONE;
    game_board[Y][X].object_orientation = NONE;

    X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

    // Move the hand to the past location.
    game_board[Y][X].object = HAND_NORMAL;
    game_board[Y][X].object_orientation = orientation;

    player -> moves[player -> moves_spent - 1] = '\0';
    player -> hand_last_x = X;
    player -> hand_last_y = Y;
    player -> moves_spent -= 1;
}
