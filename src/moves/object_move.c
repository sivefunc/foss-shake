#include <inttypes.h>           // intN_t uintN_t
#include <stdbool.h>            // bool in game_types.h
#include "../matrix_legend.h"   // constants like HAND_NORMAL
#include "../game_types.h"      // typedef struct Fields, Activators, Player
#include "none_move.h"          // move hand to box by overwriting it..

/*
 * Function: object_move
 * ----------------------
 * place hand where object is currently on and then place the box to the new
 * position according to move (preserving orientation, useful for cut hand)
 * update player stats and replace the "past" position
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
 
 * returns: void (nothing)
 *
 * note: It's assumed that Fields[i][j].object or activator where i,j is object
 *          new location is empty or open because it's going to be overwritten.
 *
 *       It's assumed that player -> moves_spent < player -> moves_total.
 *
 *       Prior to calling this function all of that must be checked,
 *       make_move.c does that.
 *
 * note: player_stats updated are:
 *          -> int16_t *moves, move is going to be inserted at
 *                  old_position + 1.
 *          -> int16_t moves_spent it increases by 1.
 *          -> int16_t hand_last_x, hand_last_y these get set to the new
 *                  location of the hand.
 */
void object_move(
        int16_t move,
        Fields **game_board,
        Player *player
        )
{
    int16_t Y = player -> hand_last_y;
    int16_t X = player -> hand_last_x;

    X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

    // Saving orientation and object because none_move will overwrite that spot
    // to a hand with a different orientation.
    int16_t orientation, object;
    orientation = game_board[Y][X].object_orientation;
    object = game_board[Y][X].object;
    
    none_move(
            move,
            game_board,
            player);

    X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

    game_board[Y][X].object = object;
    game_board[Y][X].object_orientation = orientation;
}
