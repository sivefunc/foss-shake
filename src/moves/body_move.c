#include <stdlib.h>             // abs function
#include <inttypes.h>           // intN_t uintN_t
#include <stdbool.h>            // bool in game_types.h
#include "../matrix_legend.h"   // constants like HAND_NORMAL
#include "../game_types.h"      // typedef struct Fields, Activators, Player
#include "none_move.h"          // to move player_pushing and
                                // player_being_pushed.
#include "check_activators.h"   // buttons pressed by each user.

bool belongs_to_player(Player *player, int16_t Y, int16_t X);
bool body_will_touch_objects(
        int16_t move,
        Player *player,
        Fields **game_board);

/*
 * Function: body_move
 * ----------------------
 * move all parts of player that's going to be moved (body, hand, arm extended
 * and arm corner) update player stats and replace the "past" positions of
 * player to emptiness, also place hand into the new position for the player
 * pushing.
 * 
 * Parameters:
 * -----------
 * move                 : constant indicating the direction where it will move
 *                      : all parts of player {LEFT, UP, DOWN, RIGHT}
 *
 * game_board           : Board where player plays and is a 2D array composed
 *                      : of Fields typedef struct, each Fields represent a
 *                      : cell that stores {surface, activator and object}
 * 
 * player_pushing       : Player * -> pointer to the typedef struct player,
 *                      : this is the player making the move not the player
 *                      : that will be push.
 *
 * player_being_pushed  : Player * -> pointer to the typedef struct player,
 *                      : this is the player that is going to be pushed.
 * 
 * returns: void (nothing)
 *
 * note:
 *       It's assumed that player being pushed is not itself.
 *       It's assumed that player -> moves_spent < player -> moves_total.
 *
 *       Prior to calling this function all of that must be checked,
 *       make_move.c does that.
 *
 * note: player_stats of player_pushing updated are:
 *          -> int16_t *moves, move is going to be inserted at old_position + 1
 *          -> int16_t moves_spent it increases by 1.
 *          -> int16_t hand_last_x, hand_last_y these get set to the new
 *                  location of the hand.
 *
 *       player_stats of player_being_pushed updated are:
 *          -> int16_t hand_last_x, hand_last_y new location
 *          -> int16_t body_init_x, body_init_y new location
 */
int16_t body_move(
        int16_t move,
        Fields **game_board,
        Player *player_pushing,
        Player *player_being_pushed,
        Activators *activators
        )
{
    // All body parts of player are going to be moved there, if 
    // it touchs a object and that object doesn't belong to the player itself
    // like a box then it's invalid, because it can't move other objects.
    if (body_will_touch_objects(move, player_being_pushed, game_board))
    {
        return UNSUCCESSFUL_MOVE;
    }

    // Information about the player that is going to be pushed
    int16_t Py = player_being_pushed -> body_init_y;
    int16_t Px = player_being_pushed -> body_init_x;
    int16_t orientation = game_board[Py][Px].object_orientation;

    // Information about hand last coordinates from both players for future
    // use
    int16_t player_pushing_last_x = player_pushing -> hand_last_x;
    int16_t player_pushing_last_y = player_pushing -> hand_last_y;
    int16_t player_being_pushed_last_x = player_being_pushed -> hand_last_x;
    int16_t player_being_pushed_last_y = player_being_pushed -> hand_last_y;

    // Body clearing of the player that's going to be pushed.
    game_board[Py][Px].object = NONE;
    game_board[Py][Px].object_orientation = NONE;

    // Hand and arm clearing of the player that's going to be pushed.
    for (int16_t i = 0, mv = 0; i < player_being_pushed -> moves_total; i++)
    {
        mv = player_being_pushed -> moves[i];
        if (mv == '\0')
            break;

        Px += (mv == MOVE_LEFT) ? -1 : (mv == MOVE_RIGHT ? 1 : 0);
        Py += (mv == MOVE_UP) ? -1 : (mv == MOVE_DOWN ? 1 : 0);
        game_board[Py][Px].object = NONE;
        game_board[Py][Px].object_orientation = NONE;
    }

    int16_t X_M, Y_M;
    X_M = (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y_M = (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);
    
    // Update the information about the player
    // Locations and moves_spent that's is being currently moved.
    player_being_pushed -> body_init_y += Y_M;
    player_being_pushed -> body_init_x += X_M;
    player_being_pushed -> hand_init_y += Y_M;
    player_being_pushed -> hand_init_x += X_M;
    player_being_pushed -> hand_last_y = player_being_pushed -> hand_init_y;
    player_being_pushed -> hand_last_x = player_being_pushed -> hand_init_x;
    player_being_pushed -> moves_spent = 1;
    
    // Place on the boord the player moved.
    // Placing hand object and the orientation that it had.
    Py = player_being_pushed -> hand_init_y;
    Px = player_being_pushed -> hand_init_x;
    game_board[Py][Px].object = HAND_NORMAL;
    game_board[Py][Px].object_orientation = orientation;

    // Placing body object and the orientation that it had.
    Py = player_being_pushed -> body_init_y;
    Px = player_being_pushed -> body_init_x;
    game_board[Py][Px].object = BODY;
    game_board[Py][Px].object_orientation = orientation;

    // Moves the hand now
    for (int16_t i = 1, mv = 0; i < player_being_pushed -> moves_total; i++)
    {
        mv = player_being_pushed -> moves[i];
        if (mv == '\0')
            break;

        X_M = (mv == MOVE_LEFT) ? -1 : (mv == MOVE_RIGHT ? 1 : 0);
        Y_M = (mv == MOVE_UP) ? -1 : (mv == MOVE_DOWN ? 1 : 0);
        none_move(
            mv,
            game_board,
            player_being_pushed);
    }

    // Move the hand of the player that moved the other player.
    // player_pushing moved player_being_pushed.
    none_move(
            move,
            game_board,
            player_pushing);

    // BUTTONS CHECKING
    // Of the new coordinates where the player being pushed is.
    int16_t By, Bx, Hy, Hx;
    By = player_being_pushed -> body_init_y;
    Bx = player_being_pushed -> body_init_x;
    Hy = player_being_pushed -> hand_last_y;
    Hx = player_being_pushed -> hand_last_x;

    // Note: The last coords of players are always the player -> hand_last.
    // If the player body didn't move to the last player hand
    // It means that buttons need to be checked.
    // Because otherwise it would be touching the button, so status doesn't
    // get changed.
    if (abs(player_being_pushed_last_y - By) +
        abs(player_being_pushed_last_x - Bx) != 0)
    {
        check_activators(
            Bx, By, game_board,
            activators,
            player_being_pushed, player_pushing);

        //  Check the button pressed by hand of player_being_pushed
        //  before being pushed.
         if (game_board[player_being_pushed_last_y][
                 player_being_pushed_last_x].\
                    activator == PURPLE_BUTTON_PRESSED ||
            game_board[player_being_pushed_last_y][
                player_being_pushed_last_x].\
                    activator == PURPLE_BUTTON_UNPRESSED)
         {
            check_activators(
                player_being_pushed_last_x,
                player_being_pushed_last_y, game_board, activators,
                player_being_pushed, player_pushing);
         }
    }

    // check if player pushed new hand location is touching an activator
    // (purple or green)
    check_activators(
        Hx, Hy, game_board,
        activators,
        player_being_pushed, player_pushing);

    // Checking the old position of the hand of the player P1 pushing player P2
    if (game_board[player_pushing_last_y][player_pushing_last_x].activator ==
            PURPLE_BUTTON_PRESSED ||
        game_board[player_pushing_last_y][player_pushing_last_x].activator ==
            PURPLE_BUTTON_UNPRESSED)
    {
        check_activators(
            player_pushing_last_x,
            player_pushing_last_y, game_board,
            activators,
            player_being_pushed, player_pushing);
    }

    return SUCCESSFUL_MOVE;
}

bool body_will_touch_objects(
        int16_t move,
        Player *player,
        Fields **game_board)
{

    int16_t Y = player -> body_init_y;
    int16_t X = player -> body_init_x;
    X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
    Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

    // Body checking
    if (game_board[Y][X].activator == DOOR_CLOSED ||
        (game_board[Y][X].object != NONE && 
            belongs_to_player(player, Y, X) == false))
    {
        return true;
    }
    
    // Hand and arm checking
    for (int16_t i = 0; i < player -> moves_total; i++)
    {
        move = player -> moves[i];
        if (move == '\0')
            break;
        X += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
        Y += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);


        if (game_board[Y][X].activator == DOOR_CLOSED ||
            (game_board[Y][X].object != NONE && 
                belongs_to_player(player, Y, X) == false))
        {
            return true;
        }

    }
    return false;
}

bool belongs_to_player(Player *player, int16_t Y, int16_t X)
{
    int16_t Py = player -> body_init_y;
    int16_t Px = player -> body_init_x;

    // Body checking
    if (Px == X && Py == Y)
    {
        return true;

    }
    // Hand and arm checking
    for (int16_t i = 0; i < player -> moves_total; i++)
    {

        int16_t move = player -> moves[i];
        if (move == '\0')

            break;

        Px += (move == MOVE_LEFT) ? -1 : (move == MOVE_RIGHT ? 1 : 0);
        Py += (move == MOVE_UP) ? -1 : (move == MOVE_DOWN ? 1 : 0);

        if (Px == X && Py == Y)
        {

            return true;
        }
    }
    return false;
}
