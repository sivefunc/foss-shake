#pragma once
enum matrix_legend {
    NONE = 0,
    BACKGROUND,             // Outside the border
    BORDER,
    BOX,
    FLOOR,                      // Inside the borderr
    
    HAND_NORMAL,
    HAND_CUT,
    HAND_PRESSING,
    HANDSHAKE,
    BODY,

    // Hand orientations
    RIGHT,
    DOWN,
    LEFT,
    UP,

    // Extended Arm orientations
    ARM_HOR,
    ARM_VER,
   
    // Arm corner orientation
    ARM_LEFT_DOWN,
    ARM_RIGHT_DOWN,
    ARM_RIGHT_UP,
    ARM_LEFT_UP,
    
    // Purple button states
    PURPLE_BUTTON_BOX,
    PURPLE_BUTTON_UNPRESSED,
    PURPLE_BUTTON_PRESSED,
    
    // Green buttons
    GREEN_BUTTON_BOX,
    GREEN_BUTTON_UNPRESSED,
    GREEN_BUTTON_PRESSED,

    // Doors
    DOOR_OPEN,
    DOOR_CLOSED,
    DOOR_WITH_HAND,
    DOOR_WITH_BOX,
    
    // Moves
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,

    // Quit
    QUIT,
    RESTART,

    PLAYER_1,
    PLAYER_2,
    NO_PLAYER,
    SUCCESSFUL_MOVE,
    UNSUCCESSFUL_MOVE,
    WIN_MOVE,
    ENTER,
    UNDO
};

enum SelectLevelLegend
{
    SELECT_BOARD_UNCOMPLETED,
    SELECT_BOARD_COMPLETED,
    SELECT_BOARD_PLAYING_UNCOMPLETED,
    SELECT_BOARD_PLAYING_COMPLETED
};
