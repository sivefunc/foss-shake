#include <inttypes.h>           // intN_t uintN_t
#include <stdbool.h>            // bool in game_types.h
#include "../matrix_legend.h"   // constants like HAND_NORMAL
#include "../game_types.h"      // typedef struct Fields, Activators, Player
#include "hand_and_arm.h"       // orientation according to new move
                                // and old direction.
/*
 * Function: none_move
 * ----------------------
 * place hand into the "empty spot" Fields[i][j].object of the game_board
 * according to direction, update player stats and replace the "past" position
 * of the hand to an arm extended or an arm corner depending on past and new
 * direction.
 * 
 * Parameters:
 * -----------
 * move         : constant indicating the direction where it will move
 *              : LEFT, UP, DOWN, RIGHT
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
 * note: It's assumed that Fields[i][j].object is empty because it's
 *          going to be overwritten.
 *       It's assumed that player -> moves_spent < player -> moves_total.
 *
 *       Prior to calling this function it must be checked, make_move.c does
 *       that.
 *
 * note: player_stats updated are:
 *          -> int16_t *moves, move is going to be inserted at
 *                  old_position + 1.
 *          -> int16_t moves_spent it increases by 1.
 *          -> int16_t hand_last_x, hand_last_y these get set to the new
 *                  location of the hand.
 */
void none_move(
        int16_t move,
        Fields **game_board,
        Player *player
        )
{
    int16_t arm, orientation;
    arm = arm_type(move, player -> moves[player -> moves_spent - 1]);
    orientation = hand_orientation(move);
    
    int16_t Y = player -> hand_last_y;
    int16_t X = player -> hand_last_x;
    game_board[Y][X].object = arm;

    X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

    game_board[Y][X].object = HAND_NORMAL;
    game_board[Y][X].object_orientation = orientation;
    
    // updating player_stats
    player -> moves[player -> moves_spent] = move;
    player -> moves_spent += 1;
    player -> hand_last_x = X;
    player -> hand_last_y = Y;
}
