#include <inttypes.h>               // intN_t uintN_t
#include "../matrix_legend.h"       // constants like MOVE_RIGHT

int16_t hand_orientation(int16_t move)
{
    int16_t orientation = NONE;
    switch (move)
    {
        case MOVE_LEFT:
            {
                orientation = LEFT;
                break;
            }

        case MOVE_RIGHT:
            {
                orientation = RIGHT;
                break;
            }

        case MOVE_UP:
            {
                orientation = UP;
                break;
            }

        case MOVE_DOWN:
            {
                orientation = DOWN;
                break;
            }
        
        default: {}
    }
    return orientation;
}

int16_t arm_type(int16_t move, int16_t last_move)
{
    int16_t arm = NONE;
    switch (move)
    {
        case MOVE_LEFT:
            // * *
            //   *
            if (last_move == MOVE_UP)
                arm = ARM_RIGHT_DOWN;
            
            //   *
            // * *
            else if (last_move == MOVE_DOWN)
                arm = ARM_RIGHT_UP;

            else // It's going on the same direction as before
                arm = ARM_HOR;

            break;

        case MOVE_RIGHT:
            // * *
            // *
            if (last_move == MOVE_UP)
                arm = ARM_LEFT_DOWN;
            
            // *  
            // * *
            else if (last_move == MOVE_DOWN)
                arm = ARM_LEFT_UP;
            
            else // It's going on the same direction as before
                arm = ARM_HOR;
            break;

        case MOVE_UP:
            // *  
            // * *
            if (last_move == MOVE_LEFT)
                arm = ARM_LEFT_UP;
            
            //   *
            // * *
            else if (last_move == MOVE_RIGHT)
                arm = ARM_RIGHT_UP;

            else // It's going on the same direction as before
                arm = ARM_VER;

            break;

        case MOVE_DOWN:
            // * *
            // *  
            if (last_move == MOVE_LEFT)
                arm = ARM_LEFT_DOWN;
            
            // * *
            //   *
            else if (last_move == MOVE_RIGHT)
                arm = ARM_RIGHT_DOWN;

            else // It's going on the same direction as before
                arm = ARM_VER;

            break;

        default: {}
    }

    return arm;
}
