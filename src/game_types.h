#pragma once
typedef struct Activators
{
    int16_t total_purple; int16_t total_green; int16_t total_doors;
    int16_t fields;
    int16_t **purple_buttons;                 // Type, X, Y, STATE
    int16_t **green_buttons;                  // Type, X, Y, STATE
    int16_t **doors;                          // Type, X, Y, STATE

} Activators;

typedef struct Player
{
    int16_t body_init_x;
    int16_t body_init_y;     
    int16_t hand_init_x;
    int16_t hand_init_y;
    int16_t hand_last_x;
    int16_t hand_last_y;            // Hand
    int16_t *moves;                 // MOVE -> {DOWN, UP, LEFT, RIGHT}
    int16_t moves_total;            // Number of moves player has (total)
    int16_t moves_spent;
    int16_t last_move;
} Player;

typedef struct Fields
{
    int16_t surface;
    int16_t activator;
    int16_t object;
    int16_t object_orientation;
} Fields;

typedef struct Board
{
    int16_t rows;
    int16_t columns;
    Fields **board;
} Board;
