#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "SDL.h"
#include "../matrix_legend.h"
#include "../game_types.h"

// TYPES:           1    2    3    4    5    6    7    8    9
int16_t Purple[] =    {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
int16_t Green[] =     {'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'};
int16_t D_Closed[] =  {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
int16_t D_Open[] =    {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
int16_t P1_starts[] = {'X', 'Y', 'Z', 'W'}; // Right, Up, Left, Down
int16_t P2_starts[] = {'x', 'y', 'z', 'w'}; // Right, Up, Left, Down

int16_t file_to_matrix(
        char *file_path,
        int16_t *game_rows,
        int16_t *game_columns,
        char ***file);

int16_t get_orientation_and_increment(
        int16_t ch,
        int16_t *x_m, int16_t *y_m, int16_t *mv);
bool is_in_array(int16_t ch, int16_t len, int16_t *array);

/*
 * Function: initialize_level
 * ----------------------
 * reads the content of the file located on a path and according to the
 * information stored there create the level my making a game_board,
 * two players and activators.
 *
 * Parameters:
 * -----------
 * file_path    : char * -> Null terminated string that contains the path
 *              : of the fifle to read.
 *
 * player_1 and : Player * -> pointer to a Player typedef struct to store
 * player_2     : {init_x, init_y, last_x, last_y, moves_total and moves}
 *
 * activators   : Activators * -> pointer to a Activators typedef struct to
 *              : store {purple_buttons, green_buttons and doors} on map.
 *
 * game_rows    : int16_t * -> pointer to a unitialized int16_t to store the
 *              : rows of the new board.
 *
 * game_columns : int16_t * -> pointer to a unitialized int16_t to store the
 *              : columns of the new board.
 *
 * game_board   : Fields *** -> pointer to a NULL 2D array composed of 
 *              : Fields typedef struct, each Fields represent a cell that
 *              : stores {surface, activator and object}
 *
 * returns: 0 on SUCCESS, -1 on FAILURE.
 */
char * initialize_level(
        char *file_path,
        Player *player_1, Player *player_2,
        Activators *activators,
        int16_t *game_rows,
        int16_t *game_columns,
        Fields ***game_board)
{
    // Create a char 2D array to store the contents of the file
    char **file = NULL;
    int16_t result = file_to_matrix(
            file_path,
            game_rows,
            game_columns,
            &file
            ); // Note: We don't do &game_rows or &game_columns because
               // we already have the addresses of the variables.

    if (result != 0)
        return NULL;
    
    // Information about the player.
    int16_t mv1, x_m1, y_m1, orientation1 = NONE;
    int16_t mv2, x_m2, y_m2, orientation2 = NONE;

    // The first line of a level must be the number of moves of each player
    // something like '15 10'
    sscanf(file[0], "%hd %hd",
            &(player_1 -> moves_total), &(player_2 -> moves_total));
    
    // Increases the moves quantity by one because the player already made
    // a move by pointing in which direction the character starts
    player_1 -> moves_total += 1;
    player_2 -> moves_total += 1;

    // (-1) == Button doesn't exist
    int16_t total = activators -> total_purple;
    (activators -> purple_buttons) = (int16_t **)calloc(
            total, sizeof(int16_t*));

    for (int16_t button = 0; button < total; button++)
    {
        (activators -> purple_buttons)[button] = 
            (int16_t *)calloc(activators -> fields, sizeof(int16_t));
        activators -> purple_buttons[button][0] = -1;
    }

    // (-1) == Button doesn't exist
    total = activators -> total_green;
    (activators -> green_buttons) = (int16_t **)calloc(total, sizeof(int16_t*));
    for (int16_t button = 0; button < total; button++)
    {
        (activators -> green_buttons)[button] = 
            (int16_t *)calloc(activators -> fields, sizeof(int16_t));
        activators -> green_buttons[button][0] = -1;
    }

    // (-1) == Door doesn't exist
    total = activators -> total_doors;
    (activators -> doors) = (int16_t **)calloc(total, sizeof(int16_t*));

    for (int16_t door = 0; door < total; door++)
    {
        (activators -> doors)[door] = 
            (int16_t *)calloc(activators -> fields, sizeof(int16_t));
        activators -> doors[door][0] = -1;
    }
   
    // '\0' == The player didn't make a move (Yet)
    total = player_1 -> moves_total;
    player_1 -> moves = (int16_t *)calloc(
            player_1 -> moves_total, sizeof(int16_t));
    for (int16_t move=0; move < total; move++)
        player_1 -> moves[move] = '\0';

    // '\0' == The player didn't make a move (yet)
    total = player_2 -> moves_total;
    player_2 -> moves = (int16_t *)calloc(
            player_2 -> moves_total, sizeof(int16_t));
    for (int16_t move=0; move < total; move++)
        player_2 -> moves[move] = '\0';    
   
    // Transform the content of the file to the hopefully corresponding
    // game
    Fields **board = (Fields **)calloc(*game_rows - 1, sizeof(Fields*));
    for (int16_t i = 0; i < *game_rows - 1; i++)
        board[i] = (Fields *)calloc(*game_columns, sizeof(Fields));
    

    int16_t lvl_desc_len = strnlen(file[1], *game_columns);
    char *level_description = (char *)calloc(lvl_desc_len + 1, sizeof(char));
    for (int16_t i = 0; i < lvl_desc_len; i++)
    {
        level_description[i] = file[1][i];
    }
    level_description[lvl_desc_len] = '\0';

    int16_t max_len = 0;
    for (int16_t y=2; y < *game_rows; y++)
    {
        max_len = fmax(max_len, strnlen(file[y], *game_columns));
    }
    *game_columns = max_len;


    // Starts from 2 because we removed 2 lines to get moves and description.
    for (int16_t y=2; y < *game_rows; y++)
    {
        for (int16_t x=0; x < *game_columns; x++)
        {
            int16_t ch = file[y][x];
            // By default we set the surface to be the floor not background
            // And set to NONE the activator and object.
            board[y - 2][x].surface = FLOOR;
            board[y - 2][x].activator = NONE;
            board[y - 2][x].object = NONE;
            board[y - 2][x].object_orientation = NONE;

            if (ch == '#') // Box
            {
                board[y - 2][x].object = BOX;
            }
            else if (ch == '/') // Border
                board[y - 2][x].object = BORDER;

            else if (ch == '.') // Floor
                continue;
            
            // Belongs to purple button
            else if (is_in_array(ch, 9, Purple))
            {
                int16_t type = ch - Purple[0] + 1;
                for (int16_t i = 0; i < activators -> total_purple; i++)
                {
                    if (activators -> purple_buttons[i][0] == -1)
                    {
                        activators -> purple_buttons[i][0] = type;
                        activators -> purple_buttons[i][1] = x;
                        activators -> purple_buttons[i][2] = y - 2;
                        activators -> purple_buttons[i][3] =
                            PURPLE_BUTTON_UNPRESSED;
                        board[y - 2][x].activator = PURPLE_BUTTON_UNPRESSED;
                        break;
                    }
                }
                            }
            
            // Belongs to green button
            else if (is_in_array(ch, 9, Green))
            {
                int16_t type = ch - Green[0] + 1;
                for (int16_t i = 0; i < activators -> total_green; i++)
                {
                    if (activators -> green_buttons[i][0] == -1)
                    {
                        activators -> green_buttons[i][0] = type;
                        activators -> green_buttons[i][1] = x;
                        activators -> green_buttons[i][2] = y - 2;
                        activators -> green_buttons[i][3] =
                            GREEN_BUTTON_UNPRESSED;
                        board[y - 2][x].activator = GREEN_BUTTON_UNPRESSED;
                        break;
                    }
                }
            }

            // Belongs to door closed
            else if (is_in_array(ch, 9, D_Closed))
            {
                int16_t type = ch - D_Closed[0] + 1;
                int16_t idx = 0;
                for (idx = 0; idx < activators -> total_doors; idx++)
                    if (activators -> doors[idx][0] == -1)
                        break;
                activators -> doors[idx][0] = type;                 // TYPE
                activators -> doors[idx][1] = x;                    // X
                activators -> doors[idx][2] = y - 2;                // Y
                                                                    // STATE
                activators -> doors[idx][3] = DOOR_CLOSED;
                board[y - 2][x].activator = DOOR_CLOSED;
            }

            // Belongs to door open
            else if (is_in_array(ch, 9, D_Open))
            {
                int16_t type = ch - D_Open[0] + 1;
                int16_t idx = 0;
                for (idx = 0; idx < activators -> total_doors; idx++)
                    if (activators -> doors[idx][0] == -1)
                        break;
                activators -> doors[idx][0] = type;                 // TYPE
                activators -> doors[idx][1] = x;                    // X
                activators -> doors[idx][2] = y - 2;                // Y
                activators -> doors[idx][3] = DOOR_OPEN;            // STATE
                board[y - 2][x].activator = DOOR_OPEN;
            }

            // Belongs to player 1
            else if (is_in_array(ch, 4, P1_starts))
            {

                x_m1 = x, y_m1 = y - 2;
                orientation1 = get_orientation_and_increment(
                        ch, &x_m1, &y_m1, &mv1);

                player_1 -> body_init_x = x;
                player_1 -> body_init_y = y - 2;
                player_1 -> hand_last_x = x_m1;
                player_1 -> hand_last_y = y_m1;
                player_1 -> hand_init_x = x_m1;
                player_1 -> hand_init_y = y_m1;
                player_1 -> moves[0] = mv1;
                player_1 -> last_move = mv1;
                player_1 -> moves_spent = 1;

                board[y - 2][x].object = BODY;
                board[y - 2][x].object_orientation = orientation1;

            }

            // Belongs to player 2
            else if (is_in_array(ch, 4, P2_starts))
            {
                x_m2 = x, y_m2 = y - 2;
                orientation2 = get_orientation_and_increment(
                        ch, &x_m2, &y_m2, &mv2);

                player_2 -> body_init_x = x;
                player_2 -> body_init_y = y - 2;
                player_2 -> hand_last_x = x_m2;
                player_2 -> hand_last_y = y_m2;
                player_2 -> hand_init_x = x_m2;
                player_2 -> hand_init_y = y_m2;
                player_2 -> moves[0] = mv2;
                player_2 -> last_move = mv2;
                player_2 -> moves_spent = 1;
                board[y - 2][x].object = BODY;
                board[y - 2][x].object_orientation = orientation2;
            }

            else // Background
            {
                board[y - 2][x].surface = BACKGROUND;
            }
        }
    }
    board[y_m1][x_m1].object = HAND_NORMAL;
    board[y_m1][x_m1].object_orientation = orientation1;
    board[y_m2][x_m2].object = HAND_NORMAL;
    board[y_m2][x_m2].object_orientation = orientation2;
    *game_board = board;

    // Freeing from memory the file
    for (int16_t i = 0; i < *game_rows; i++)
        free(file[i]);
    free(file);

    *game_rows -= 2; // Remove rows because the first row was the moves of 
                     // each player
    return level_description;
}

int16_t get_orientation_and_increment(
        int16_t ch,
        int16_t *x_m, int16_t *y_m, int16_t *mv)
{
    *mv =
        toupper(ch) == 'X' ? MOVE_RIGHT :
        toupper(ch) == 'Y' ? MOVE_UP :
        toupper(ch) == 'Z' ? MOVE_LEFT: MOVE_DOWN;

    int16_t move = NONE;
    switch (*mv)
    {
        case MOVE_RIGHT:
            {
                *x_m += 1;
                move = RIGHT;
                break;
            }


        case MOVE_LEFT:
            {
                *x_m -= 1;
                move = LEFT;
                break;
            }

        case MOVE_UP:
            {
                *y_m -= 1;
                move = UP;
                break;
            }

        case MOVE_DOWN:
            {
                *y_m += 1;
                move = DOWN;
                break;
            }

        default: {}
    }
    return move;
}

bool is_in_array(int16_t ch, int16_t len, int16_t *array)
{
    for (int16_t i=0; i < len; i++)
        if (ch == array[i])
            return 1;
    
    return 0;
}

int16_t file_to_matrix(
        char *file_path,
        int16_t *rows,
        int16_t *columns,
        char ***file)
{
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Can't load level: %s\n", file_path);
        return -1;
    }

    // Matrix will grow dynamically
    int16_t max_columns = 1, max_rows = 1;
    char **matrix = (char **) calloc(max_rows, sizeof(char*));

    for (int16_t i = 0; i < max_rows; i++)
       (matrix)[i] = (char*) calloc(max_columns, sizeof(char));

    int16_t curr_column = 1;    // Number of columns in current line
    int16_t curr_row = 1;       // Number of rows in current file
    int16_t ch;
    int16_t *lines_columns = (int16_t *) calloc(max_rows, sizeof(int16_t));
    for (int16_t i = 0; i < max_rows; i++)
        lines_columns[i] = max_columns;

    int16_t last_ch = 0;
    while( (ch = getc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            curr_column = 1;
            if (last_ch == ch)
                curr_row--;

            curr_row++;
            last_ch = '\n';
            continue;
        }

        // Modify size of each row to curr_columns
        if (curr_column > max_columns)
        {
            max_columns = curr_column;
            for (int16_t i=0; i < max_rows; i++)
            {
                matrix[i] = (char*)realloc(
                        (matrix)[i], max_columns * sizeof(char));

                // Initialize the columns that are less than max_columns
                for (int16_t j = lines_columns[i]; j < max_columns; j++)
                    matrix[i][j] = '\0';

                lines_columns[i] = max_columns;
            }
        }

        // Modify number of rows to curr_row
        if (curr_row > max_rows)
        {
            max_rows = curr_row;

            // Adds a new row
            matrix = (char**)realloc(
                    matrix, max_rows * sizeof(char*));
            
            // Locate memory on pointer[max_rows-1]
            matrix[max_rows - 1] = (char*) calloc(max_columns,
                    sizeof(char));

            // Increase lines_columns by 1
            lines_columns = (int16_t*) realloc(lines_columns,
                    max_rows * sizeof(int16_t));
            lines_columns[max_rows-1] = max_columns;
         }

        matrix[curr_row - 1][curr_column - 1] = ch;
        curr_column++;
        last_ch = ch;
    }
    *rows = max_rows, *columns = max_columns;
    free(lines_columns);
    fclose(fp);
    *file = matrix;
    return 0;
}
