#include <stdbool.h>            // bool, true and false.
#include <stdint.h>             // intN_t, uintN_t
#include <stdlib.h>             // calloc and realloc
#include "game_types.h"         // Activators, Player and Fields typedef
                                // structs.

// Struct that saves information of a play given in a specific moment
// is like a frame on a video.
typedef struct Game_Status
{
    Player player_1;
    Player player_2;
    Activators activators;
    Board game_board;
} Game_Status;

/*
 * Function: copy_game_status
 * ----------------------
 *
 * Deep copies the struct to another struct, so the pointers addresses will be
 * different.
 *
 * Parameters:
 * -----------
 * game_status  : typedef struct Game_Status variable that's going to be copied.
 *
 * return: typedef struct Game_Status that is deep copied.
 */
Game_Status copy_game_status(Game_Status game_status)
{
    Game_Status g_copy = game_status;

    //////////////////////////
    // Copying the pointers //
    //////////////////////////
    
    // Copying the moves array from the first player
    g_copy.player_1.moves = (int16_t*) calloc(
            g_copy.player_1.moves_total,
            sizeof(int16_t));

    for (int16_t i = 0; i < g_copy.player_1.moves_total; i++)
    {
        g_copy.player_1.moves[i] = game_status.player_1.moves[i];
    }


    // Copying the moves array from the second player
     g_copy.player_2.moves = (int16_t*) calloc(
            g_copy.player_2.moves_total,
            sizeof(int16_t));

    for (int16_t i = 0; i < g_copy.player_2.moves_total; i++)
    {
        g_copy.player_2.moves[i] = game_status.player_2.moves[i];
    }

    // Copying the purple_buttons
    int16_t total = g_copy.activators.total_purple;
    g_copy.activators.purple_buttons = (int16_t **)calloc(
            total, sizeof(int16_t*));
    for (int16_t button = 0; button < total; button++)
    {
        g_copy.activators.purple_buttons[button] = 
            (int16_t *)calloc(g_copy.activators.fields, sizeof(int16_t));
        
        for (int16_t field = 0; field < g_copy.activators.fields; field++)
        {
            g_copy.activators.purple_buttons[button][field] =
                game_status.activators.purple_buttons[button][field];
        }
    }

    // Copying the green_buttons
    total = g_copy.activators.total_green;
    g_copy.activators.green_buttons = (int16_t **)calloc(
            total, sizeof(int16_t*));

    for (int16_t button = 0; button < total; button++)
    {
        g_copy.activators.green_buttons[button] = 
            (int16_t *)calloc(g_copy.activators.fields, sizeof(int16_t));
        
        for (int16_t field = 0; field < g_copy.activators.fields; field++)
        {
            g_copy.activators.green_buttons[button][field] =
                game_status.activators.green_buttons[button][field];
        }
    }

    // Copying doors
    total = g_copy.activators.total_doors;
    (g_copy.activators.doors) = (int16_t **)calloc(total, sizeof(int16_t*));

    for (int16_t door = 0; door < total; door++)
    {
        g_copy.activators.doors[door] = 
            (int16_t *)calloc(g_copy.activators.fields, sizeof(int16_t));
        for (int16_t field = 0; field < g_copy.activators.fields; field++)
        {
            g_copy.activators.doors[door][field] =
                game_status.activators.doors[door][field];
        }
    }

    // Copying board
    g_copy.game_board.board = (Fields **)calloc(
            g_copy.game_board.rows, sizeof(Fields*));

    for (int16_t i = 0; i < g_copy.game_board.rows; i++)
    {

        g_copy.game_board.board[i] = (Fields*)calloc(
                g_copy.game_board.columns, sizeof(Fields));

        for (int16_t j = 0; j < g_copy.game_board.columns; j++)
        {
            g_copy.game_board.board[i][j] = game_status.game_board.board[i][j];
        }
    }

    return g_copy;
}

/*
 * Function: free_game_status
 * ----------------------
 *
 * Free the pointers.
 *
 * Parameters:
 * -----------
 * game_status  : typedef struct Game_Status variable that's going be freed.
 *
 * return: nothing (void)
 */

void free_game_status(Game_Status game_status)
{
    // Freeing the player
    free(game_status.player_1.moves);
    free(game_status.player_2.moves);

    // Freeing the old board
    for (int16_t i = 0; i < game_status.game_board.rows; i++)
    {
        free(game_status.game_board.board[i]);
    }
    free(game_status.game_board.board);

    // Freeing the purple button
    int16_t total = game_status.activators.total_purple;
    for (int16_t button = 0; button < total; button++)
    {
        free(game_status.activators.purple_buttons[button]);
    }
    free(game_status.activators.purple_buttons);

    // Freeing the green button
    total = game_status.activators.total_green;
    for (int16_t button = 0; button < total; button++)
    {
        free(game_status.activators.green_buttons[button]);
    }
    free(game_status.activators.green_buttons);

    // Freeing the doors
    total = game_status.activators.total_doors;
    for (int16_t door = 0; door < total; door++)
    {
        free(game_status.activators.doors[door]);
    }
    free(game_status.activators.doors);
}

/*
 * Function: free_games_status
 * ----------------------
 *
 * free an array of Game_Status by calling free_game_status on each element.
 *
 * Parameters:
 * -----------
 *
 * size          : numbers of elements that the array contain.
 *
 * games_status  : pointer to typedef struct Game_Status representing an array
 *               : of game_status.
 *
 * return: nothing (void)
 */

/*
 * Function: free_games_status
 * ----------------------
 *
 * free an array of Game_Status by calling free_game_status on each element.
 *
 * Parameters:
 * -----------
 *
 * size          : numbers of elements that the array contain.
 *
 * games_status  : pointer to typedef struct Game_Status representing an array
 *               : of game_status.
 *
 * return: nothing (void)
 */
void free_games_status(
        int16_t size, Game_Status *games_status)
{
    for (int16_t game_status = 0; game_status < size; game_status++)
    {
        free_game_status(games_status[game_status]);
    }
    free(games_status);
}

/*
 * Function: append_game_status
 * ----------------------
 *
 * increase the lenght of the array by one and add as last element the
 * new game_status.
 *
 * Parameters:
 * -----------
 *
 * game_status   : typedef struct Game_Status variable that is going to be
 *               : added to the array.
 *
 * size          : number of elements in the array.
 *
 * games_status  : pointer to typedef struct Game_Status representing an array
 *               : of game_status.
 *
 * return: the new size of the array (it increased by one)
 */
int16_t append_game_status(
        Game_Status game_status, int16_t size, Game_Status **games_status)
{
    // Past self: Check for NULL on realloc
    *games_status = (Game_Status *)realloc(
            *games_status, sizeof(Game_Status) * (size + 1));
    (*games_status)[size] = game_status;
    return size + 1;
}

/*
 * Function: pop_game_status
 * ----------------------
 *
 * decrease the lenght of the array by one and remove last element and
 * frees it.
 *
 * Parameters:
 * -----------
 *
 * size          : number of elements in the array.
 *
 * games_status  : pointer to typedef struct Game_Status representing an array
 *               : of game_status.
 *
 * return: the new size of the array (it decreased by one)
 */
int16_t pop_game_status(int16_t size, Game_Status **games_status)
{
    free_game_status((*games_status)[size - 1]);
    *games_status = (Game_Status *)realloc(
            *games_status, sizeof(Game_Status) * (size - 1));
    return size - 1;
}
