#include "SDL.h"            // Support for graphics and input
#include "SDL_ttf.h"        // Support for truetype font rendering
#include "SDL_mixer.h"      // Support for audio types
#include <stdio.h>          // sprintf unsafe but it works.
#include <math.h>           // Support for Square root to get a perfect square.
#include <inttypes.h>       // Support for uintN_t
#include <stdbool.h>        // support for true and false
#include "matrix_legend.h"  // Definitions related to game, e.g: FLOOR
#include "game_types.h"     // Typedef struct Activators, Player
#include "sdl_types.h"      // typedef struct AudioChunks and Textures

typedef struct BG_Dots
{
    int16_t rows;
    int16_t columns;
    int16_t increment;
    SDL_Surface *image;
} BG_Dots;

extern BG_Dots bg_dots;
bool _belongs_to_player(Player *player, int16_t Y, int16_t X);

// returns a pointer to the corresponding texture of the activator
// if activator is NONE returns null
SDL_Texture * activator_texture(int16_t activator, Textures *textures);

// returns a pointer to the corresponding texture of the surface.
// if surface is NONE returns null
SDL_Texture * surface_texture(int16_t surface, Textures *textures);

// returns a pointer to the corresponding texture of the object and
// changes the value of the angle. if object is NONE returns null and angle 0.
SDL_Texture *object_texture(
        bool left_player, int16_t object, int16_t orientation, int16_t *angle,
        Textures *textures);

// renders onto the center of body the number of moves left the player has.
void draw_number_on_body(
        Player *player,
        char *message,
        TTF_Font *font,
        SDL_Rect *image_hitbox,
        SDL_Renderer *renderer
        );
void background_graphics(
        SDL_Renderer *renderer, SDL_Window *window,
        uint8_t r, uint8_t g, uint8_t b);
/*
 * Function: game_board_graphics
 * ----------------------
 * renders onto window the game board incluiding players, activators and
 * background.
 * 
 * Parameters:
 * -----------
 * game_rows    : int16_t number of rows
 *              : that the game board has.
 *
 * game_columns : int16_t number of columns that
 *              : the game board has.
 *
 * game_board   : NxM board where each cell is of
 *              : Fields type {surface, activator and object}
 *
 * window       : where it will be drawn the graphics.
 * renderer     : shows the graphics of the window.
 * textures     : collection of SDL_Texture related to graphics
 *
 * returns: void - nothing due to function just doing drawings
 *
 */
void game_board_graphics(
        int16_t game_rows,
        int16_t game_columns,
        Fields **game_board,
        Player * player_1,
        Player * player_2,
        SDL_Window *window,
        SDL_Renderer *renderer,
        Textures *textures,
        char *level_description
        )
{
    TTF_Init();
    TTF_Font* top_font = TTF_OpenFont("res/font/font.ttf", 144);

    background_graphics(renderer, window, 62, 137, 72);
    
    // To be able to resize each texture acording to the size of window.
    int screen_width, screen_height;
    SDL_GetWindowSize(window, &screen_width, &screen_height);
   
    char *top_text_1 = "LEFT_HAND  QUIT  UNDO  RESTART  RIGHT_HAND";
    char *top_text_2 = "  WASD      Q     U       R     ARROW KEYS";
    int16_t text_padding = fmin(screen_width / 5, screen_height / 5) / 2;

    int16_t board_width = screen_width - text_padding * 2;
    int16_t board_height = screen_height - text_padding * 2;

    SDL_Texture *top_texture = NULL;
    
    // Create the surface that constains the text
    SDL_Color Text_Color = {254, 174, 53, 0};
    SDL_Surface* text_surface = TTF_RenderText_Solid(
            top_font,  top_text_1, Text_Color); 

    top_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_box =
    {
        text_padding,
        0,
        board_width,
        text_padding / 2
    };

    SDL_RenderCopy(
                renderer,
                top_texture,
                NULL,
                &text_box
                );
    
    SDL_DestroyTexture(top_texture);
    SDL_FreeSurface(text_surface);

    text_box.y = text_padding / 2;
    text_surface = TTF_RenderText_Solid(top_font,  top_text_2, Text_Color); 
    top_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_RenderCopy(
                renderer,
                top_texture,
                NULL,
                &text_box
                );
    SDL_FreeSurface(text_surface);
    // Image boundaries and where it will start
    int16_t side_lenght = fmin(
            board_width / game_columns,
            board_height / game_rows);

    // Padding to center the graphics
    int16_t board_x_padding = (board_width - side_lenght * game_columns) / 2;
    int16_t board_y_padding = (board_height - side_lenght * game_rows) / 2;

    SDL_Texture *cell_texture = NULL;    // Image to draw on cell Fields
    SDL_Rect image_hitbox =
    {
        text_padding + board_x_padding,     // Start at this X coord
        text_padding + board_y_padding,     // Start at this Y coord
        side_lenght,                        // Ends in this hor. lenght
        side_lenght                         // Ends in this ver. lenght
    };
    // Due to moves_spent being int16_t
    char *message = (char*)calloc(sizeof(int16_t) + 1, sizeof(char));
    int16_t angle = 0;                   // Angle to rotate the image
                                         // This is made to only have 1 texture
                                         // that produces another 3 textures.
                                         // {0, 90, 180 and 270}

    
    // Goes to each cell of type Fields on the game_board
    for (int16_t y = 0; y < game_rows; y++)
    {
        for (int16_t x = 0; x < game_columns; x++)
        {
            cell_texture = surface_texture(game_board[y][x].surface, textures);
            SDL_RenderCopy(
                    renderer,
                    cell_texture,
                    NULL,
                    &image_hitbox);
           
            cell_texture = activator_texture(
                    game_board[y][x].activator, textures),
            SDL_RenderCopy(
                    renderer,
                    cell_texture,
                    NULL,
                    &image_hitbox);

            cell_texture = object_texture(
                    _belongs_to_player(player_1, y, x),
                    game_board[y][x].object,
                    game_board[y][x].object_orientation,
                    &angle, textures);

            SDL_RenderCopyEx(
                    renderer,
                    cell_texture,
                    NULL,
                    &image_hitbox,
                    angle,
                    NULL,
                    SDL_FLIP_NONE);
            
            if (player_1 -> body_init_x == x && player_1 -> body_init_y == y)
            {
                draw_number_on_body(
                        player_1,
                        message,
                        top_font,
                        &image_hitbox,
                        renderer
                        );
            }
            else if (player_2 -> body_init_x == x && player_2 -> body_init_y == y)
            {
                draw_number_on_body(
                        player_2,
                        message,
                        top_font,
                        &image_hitbox,
                        renderer
                        );
            }
            // Draw the next field column right after the column before.
            image_hitbox.x += image_hitbox.w;
        }
        
        // Starts from the beginning (0 on X + the padding to center board)
        image_hitbox.x = text_padding + board_x_padding;
        
        // Draw the next field row right after the row before
        image_hitbox.y += image_hitbox.h;
    }
    text_surface = TTF_RenderText_Solid(
            top_font, level_description, Text_Color); 
    SDL_Texture * bottom_texture = SDL_CreateTextureFromSurface(
            renderer, text_surface);

    text_box.y = text_padding + board_height;
    text_box.h = text_padding;
    SDL_RenderCopy(
                renderer,
                bottom_texture,
                NULL,
                &text_box
                );
    // Shows all to the window
    SDL_RenderPresent(renderer);
    
    // Frees data.
    SDL_DestroyTexture(top_texture);
    SDL_DestroyTexture(bottom_texture);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(top_font);
    TTF_Quit();
}

/*
 * Function: level_selector_graphics
 * ----------------------
 * renders onto window the level selector incluiding the numbers
 * background and hovering effect.
 * 
 * Parameters:
 * -----------
 * selector_rows    : int16_t -> number of rows
 *                  : that the selector board has.
 *
 * selector_columns : int16_t -> number of columns that
 *                  : the selector board has.
 *
 * selector_board   : NxM board where each cell is of bool type
 *                  : 1/True means selecting, 0/False means not selecting.
 *
 * window       : where it will be drawn the graphics.
 * renderer     : shows the graphics of the window.
 * textures     : collection of SDL_Texture related to graphics
 *
 * returns: void - nothing due to function just doing drawings
 *
 */

void level_selector_graphics(
        int16_t selector_rows,
        int16_t selector_columns,
        int16_t **selector_board,
        int16_t levels,
        SDL_Window *window,
        SDL_Renderer *renderer,
        Textures *textures
        )
{
    TTF_Init();
    TTF_Font* selector_font = TTF_OpenFont("res/font/font.ttf", 144);

    background_graphics(renderer, window, 84, 50, 87);
        
    // To be able to resize each texture acording to the size of window.
    int screen_width, screen_height;
    SDL_GetWindowSize(window, &screen_width, &screen_height);

    int16_t text_padding = fmin(screen_width / 5, screen_height / 5) / 2;

    int16_t board_width = screen_width - text_padding * 2;
    int16_t board_height = screen_height - text_padding * 2;

    // Image boundaries and where it will start
    int16_t side_lenght = fmin(
            board_width / selector_columns,
            board_height / selector_rows);

    // Padding to center the graphics
    int16_t board_x_padding = (board_width - side_lenght * selector_columns) /
        2;
    int16_t board_y_padding = (board_height - side_lenght * selector_rows) /
        2;
            
    // Image boundaries and where it will start
    SDL_Rect image_hitbox =
    {
        text_padding + board_x_padding,     // Start at this X coord
        text_padding + board_y_padding,     // Start at this Y coord
        side_lenght,                        // Ends in this hor. lenght
        side_lenght                         // Ends in this ver. lenght
    };

    SDL_Texture *cell_texture = NULL;
    SDL_Texture *number_texture = NULL;
    
    // Due to level being int16_t {0, 255} we only need 3 digits. + '\0'
    char *message = (char*)calloc(3 + 1, sizeof(char));

    char *top_header = "LEFT_HAND    QUIT    RIGHT_HAND    PRESS ENTER";
    char *top_msg =    "  WASD        Q      ARROW KEYS    SELECT LEVEL";
    
    // Create the surface that constains the text
    SDL_Color Text_Color = {254, 174, 53, 0};
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(selector_font,  top_header, Text_Color); 

    // Make it a texture.
    number_texture = SDL_CreateTextureFromSurface(
            renderer, surfaceMessage);

    SDL_Rect text_box =
    {
        text_padding,
        0,
        board_width,
        text_padding / 3
    };
    SDL_RenderCopy(
                renderer,
                number_texture,
                NULL,
                &text_box
                );

    // Frees the pointers

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(number_texture);

    text_box.y = text_padding / 3;
    surfaceMessage = TTF_RenderText_Solid(selector_font,  top_msg, Text_Color); 

    // Make it a texture.
    number_texture = SDL_CreateTextureFromSurface(
            renderer, surfaceMessage);

    SDL_RenderCopy(
                renderer,
                number_texture,
                NULL,
                &text_box
                );
    
    // Goes through each cell on the selector_board
    int16_t current_level = 0;
    for (int16_t Y = 0; Y < selector_rows; Y++)
    {
        for (int16_t X = 0; X < selector_columns; X++)
        {
            if (selector_board[Y][X] == SELECT_BOARD_UNCOMPLETED)
            {
                cell_texture = textures -> select_board_uncompleted;
            }

            else if (selector_board[Y][X] == SELECT_BOARD_COMPLETED)
            {
                cell_texture = textures -> select_board_completed;
            }

            else if (selector_board[Y][X] == SELECT_BOARD_PLAYING_COMPLETED)
            {
                cell_texture = textures -> select_board_playing_completed;
            }
            else
            {
                cell_texture = textures -> select_board_playing_uncompleted;
            }

            // Converts the current level to a string
            sprintf(message, "%hd", current_level);
            
            // Create the surface that constains the text
            SDL_Color Text_Color = {0, 0, 0, 0};
            SDL_FreeSurface(surfaceMessage);
            surfaceMessage =
                TTF_RenderText_Solid(selector_font,  message, Text_Color); 

            // Make it a texture.
            SDL_DestroyTexture(number_texture);
            number_texture = SDL_CreateTextureFromSurface(
                    renderer, surfaceMessage);
            
            // Draws the cell and then the text not vice-versa
            // if you do it vice-versa the text won't be shown.
            SDL_RenderCopy(
                renderer,
                cell_texture,
                NULL,
                &image_hitbox
                );

            SDL_RenderCopy(
                renderer,
                number_texture,
                NULL,
                &image_hitbox
                );
            
            // Draw the next column right after the column before.
            image_hitbox.x += image_hitbox.w;
            current_level += 1;
            if (current_level == levels)
                break;
        }
        if (current_level == levels)
            break;

        // Starts from the beginning (0 on X + the padding to center board)
        image_hitbox.x = text_padding + board_x_padding;
        
        // Draw the next row right after the row before
        image_hitbox.y += image_hitbox.h;

    }

    SDL_RenderPresent(renderer);

    // Frees from memory the data.
    free(message);
    SDL_DestroyTexture(number_texture);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(selector_font);
    TTF_Quit();
}

/*
 * Function: surface_texture
 * ----------------------
 * 
 * Parameters:
 * -----------
 * surface      : int16_t -> value of constant with descriptive
 *              : name belonging to enum matrix_legend.h, e.g:
 *              : GREEN_BUTTON_UNPRESSED
 *
 * textures     : Textures * -> A pointer to a Textures typedef struct
 *              : that contains the textures of each activator.
 *
 * returns: SDL_Texture * -> a pointer to the corresponding texture that
 *          represents the object.
 *
 */
SDL_Texture * surface_texture(int16_t surface, Textures *textures)
{
    SDL_Texture *texture = NULL;
    switch (surface)
    {
        case NONE: case FLOOR:
            {
                texture = textures -> floor_texture;
                break;
            }

        case BACKGROUND:
            {
                texture = NULL;
                break;
            }

        default: {}
    }

    return texture;
}

/*
 * Function: activator_texture
 * ----------------------
 * 
 * Parameters:
 * -----------
 * activator    : int16_t -> value of constant with descriptive
 *              : name belonging to enum matrix_legend.h, e.g:
 *              : GREEN_BUTTON_UNPRESSED
 *
 * textures     : Textures * -> A pointer to a Textures typedef struct
 *              : that contains the textures of each activator.
 *
 * returns: SDL_Texture * -> a pointer to the corresponding texture that
 *          represents the object.
 *
 */
SDL_Texture * activator_texture(int16_t activator, Textures *textures)
{
    SDL_Texture *texture = NULL;
    switch (activator)
    {
        case GREEN_BUTTON_UNPRESSED:
            {
                texture = textures -> green_button_unpressed_texture;
                break;
            }

        case PURPLE_BUTTON_UNPRESSED:
            {
                texture = textures -> purple_button_unpressed_texture;
                break;
            }
        
        case GREEN_BUTTON_PRESSED:
            {
                texture = textures -> green_button_pressed_texture;
                break;
            }

        case PURPLE_BUTTON_PRESSED:
            {
                texture = textures -> purple_button_pressed_texture;
                break;
            }

        case DOOR_OPEN:
            {
                texture = textures -> door_open_texture;
                break;
            }

        case DOOR_CLOSED:
            {
                texture = textures -> door_closed_texture;
                break;
            }

        case NONE:
            {
                texture = NULL;
                break;
            }

        default: {}
    }

    return texture;
}

/*
 * Function: object_texture
 * ----------------------
 * 
 * Parameters:
 * -----------
 * object       : int16_t -> value of constant with descriptive
 *              : name belonging to enum matrix_legend.h, e.g: HAND_LEFT.
 *
 * angle        : int16_t * -> pointer to a integer whose value will be
 *              : changed to the quantity of degrees that the texture should
 *              : rotate the texture to match the actual object.
 *
 * textures     : Textures * -> A pointer to a Textures typedef struct
 *              : that contains the textures of each object.
 *
 * returns: SDL_Texture * -> a pointer to the corresponding texture that
 *          represents the object.
 *
 */
SDL_Texture *object_texture(
        bool left_player, int16_t object, int16_t orientation, int16_t *angle, Textures *textures)
{
    SDL_Texture *texture = NULL;
    switch (object)
    {
        case HAND_NORMAL:
            {
                *angle = 90 * (3 - (UP - orientation));
                texture = left_player ? textures -> hand_left_player :
                    textures -> hand_right_player;
                break;
            }

        case HAND_PRESSING:
            {

                *angle = 90 * (3 - (UP - orientation));
                texture = left_player ?
                    textures -> hand_pressing_button_left_player :
                    textures -> hand_pressing_button_right_player;
                break;
            }

        case HAND_CUT:
            {

                *angle = 90 * (3 - (UP - orientation));
                texture = textures -> hand_cut;
                break;
            }

        case HANDSHAKE:
            {
                *angle = 90 * (3 - (UP - orientation));
                texture = left_player ?
                    textures -> hand_shake_left_player :
                    textures -> hand_shake_right_player;
                break;
            }

        case BODY:
            {
                *angle = 90 * (3 - (UP - orientation));
                texture = left_player ?
                    textures -> body_left_player :
                    textures -> body_right_player;

                break;
            }

        case ARM_HOR: case ARM_VER:
            {
                *angle = 90 * (1 - (ARM_VER - object));
                texture = left_player ?
                    textures -> arm_extended_left_player :
                    textures -> arm_extended_right_player;
                break;
            }

        case ARM_LEFT_UP: case ARM_LEFT_DOWN:
        case ARM_RIGHT_UP: case ARM_RIGHT_DOWN:
            {

                *angle = (90 * (3 - (ARM_LEFT_UP - object)));
                texture = left_player ?
                    textures -> arm_corner_left_player :
                    textures -> arm_corner_right_player;
                break;
            }

        case BORDER:
            {
                *angle = 0;
                texture = textures -> border_texture;
                break;
            }

        case BOX:
            {
                *angle = 0;
                texture = textures -> box_texture;
                break;
            }

        case NONE:
            {
                *angle = 0;
                texture = NULL;
                break;
            }

        default: {}
    }
    return texture;
}

void draw_number_on_body(
        Player *player,
        char *message,
        TTF_Font *font,
        SDL_Rect *image_hitbox,
        SDL_Renderer *renderer
        )
{
    // Converts the current level to a string
    SDL_Surface *msg_surface = NULL;
    SDL_Texture *msg_texture = NULL;

    int16_t moves_left = player -> moves_total - player -> moves_spent;
    sprintf(message, "%hd", moves_left);
    
    SDL_Color text_color = moves_left ?
        (SDL_Color){0, 0, 0, 0} : (SDL_Color){255, 0, 0, 0};

    // Create the surface that constains the text
    msg_surface = TTF_RenderText_Solid(font,  message, text_color); 

    // Make it a texture.
    msg_texture = SDL_CreateTextureFromSurface(renderer, msg_surface);

    SDL_Rect text_body_box =
    {
        0, 0,
        image_hitbox -> w / 2,
        image_hitbox -> h / 2,
    };

    text_body_box.x = (image_hitbox -> w - text_body_box.w) / 2 + 
        image_hitbox -> x;

    text_body_box.y = (image_hitbox -> h - text_body_box.h) / 2 +
        image_hitbox -> y;
    
    SDL_RenderCopy(
        renderer,
        msg_texture,
        NULL,
        &text_body_box
        );
    
    SDL_FreeSurface(msg_surface);
    SDL_DestroyTexture(msg_texture);

}

void welcome_graphics(
        Textures *textures, SDL_Window *window, SDL_Renderer *renderer)
{
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("res/font/font.ttf", 144);

    background_graphics(renderer, window, 62, 137, 72);
        
    // To be able to resize each texture acording to the size of window.
    int screen_width, screen_height;
    SDL_GetWindowSize(window, &screen_width, &screen_height);

    int16_t text_padding = fmin(screen_width / 3, screen_height / 3) / 2;

    int16_t logo_width = screen_width - text_padding * 2;
    int16_t logo_height = screen_height - text_padding * 2;
    
    char *top_message = "HANDSHAKE";

    // Image boundaries and where it will start
    SDL_Rect hitbox =
    {
        text_padding,                       // Start at this X coord
        0,                                  // Start at this Y coord
        logo_width,                         // Ends in this hor. lenght
        text_padding                        // Ends in this ver. lenght
    };
    
    // Create the surface that constains the text
    SDL_Color Text_Color = {254, 174, 53, 0};
    SDL_Surface* msg_surface = TTF_RenderText_Solid(font, top_message, Text_Color); 

    // Make it a texture.
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(
            renderer, msg_surface);

    SDL_RenderCopy(
                renderer,
                text_texture,
                NULL,
                &hitbox
                );

    hitbox.y = text_padding;
    hitbox.h = logo_height;

    SDL_RenderCopy(
                renderer,
                textures -> welcome_logo_texture,
                NULL,
                &hitbox
                );

    hitbox.y += logo_height;
    hitbox.h = text_padding;

    SDL_FreeSurface(msg_surface);
    SDL_DestroyTexture(text_texture);

    msg_surface = TTF_RenderText_Solid(font,
            "[Q]uit or [ENTER] to play the game", Text_Color); 

    // Make it a texture.
    text_texture = SDL_CreateTextureFromSurface(renderer, msg_surface);

    SDL_RenderCopy(
                renderer,
                text_texture,
                NULL,
                &hitbox
                );
    
    SDL_RenderPresent(renderer);

    // Frees from memory the pointers.
    SDL_FreeSurface(msg_surface);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    TTF_Quit();

}
void background_graphics(
        SDL_Renderer *renderer, SDL_Window *window,
        uint8_t r, uint8_t g, uint8_t b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
        
    // To be able to resize each texture acording to the size of window.
    int screen_width, screen_height;
    SDL_GetWindowSize(window, &screen_width, &screen_height);

    // Image boundaries and where it will start
    // X: Dots_X or Dots_Y
    // P1: side_lenght of each object on x and y measured in pixels
    // P2: padding between objects measured in pixels
    // W: Window width or window_height
    //
    // 1) X * (P1 + P2) = W
    // 2) X * P_1 + X * P_2 = W
    // 3) X * P_1 = W - X * P_2
    // 4) P_1 = (W - X * P_2) / X
    // 5) P_1 = (W/X) - (X * P_2) / X
    // 6) P_1 = (W / X) - P_2
    //
    // Let P_2 be e.q to: W / (N * X) where N >= 2
    // We use P_2 as a proportion instead of an arbitrary value because we want
    // the padding to depend on the window size and the dots (ver, hor.)
    //
    // 7) P_1 = (W / X) - (W / (N * X))
    // 8) P_1 = ((N * X * W) - (W * X)) / (N * X^3)
    // 9) P_1 = ((N-1) * X * W) / (N * X^3)
    // 10) P_1 = ((N-1) * W) / (N * X)
    //
    // note: N >= 2 because if we do N = 1, P_1 size will be 0.

    const int16_t N = 4;
    const int16_t side_lenght = fmin(
            ((N-1) * screen_width) / (N * bg_dots.columns),
            ((N-1) * screen_height) / (N * bg_dots.rows));
    
    // Now we find P_2
    const int16_t padding_x = screen_width / bg_dots.columns - side_lenght;
    const int16_t padding_y = screen_height / bg_dots.rows - side_lenght;

    SDL_LockSurface(bg_dots.image);
    
    // Darken the dot image
    // by 80% using as RGB the background color.
    uint8_t target_r = 0, target_g = 0, target_b = 0;
    for (int16_t y = 0; y < bg_dots.image -> h; y++)
    {
        for (int16_t x = 0; x < bg_dots.image -> w; x++)
        {
             uint32_t *target_pixel = (uint32_t *) (bg_dots.image -> pixels +
                y * bg_dots.image -> pitch + 
                x * bg_dots.image -> format->BytesPerPixel);

             SDL_GetRGB(*target_pixel, bg_dots.image -> format,
                     &target_r, &target_g, &target_b);

             // Probably not the best way but, I decided that the color black
             // is the color that is not going to change.
             // so in that way I can change the color of the dot image.
             // without knowing the shape.
             // check res/Background.
             if (target_r == 0 && target_g == 0 && target_b == 0)
                 continue;

             *target_pixel = SDL_MapRGB(bg_dots.image -> format,
                     r * 0.8, g * 0.8, b * 0.8);
        }
    }

    SDL_UnlockSurface(bg_dots.image);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(
            renderer, bg_dots.image);
    // Image boundaries and where it will start

    // Add some copies that are before the window (negative coordinates)
    // fmax / fmin >= 1
    const int16_t adding_negative = 
        ceil(fmax(padding_x, padding_y) / fmin(padding_x, padding_y));

    SDL_Rect image_hitbox =
    {
        bg_dots.increment - (padding_x + side_lenght) * adding_negative,
        bg_dots.increment - (padding_y + side_lenght) * adding_negative,
        side_lenght,                // Ends in this hor. lenght
        side_lenght                 // Ends in this ver. lenght
    };

    for (int16_t y = 0; y < bg_dots.rows + adding_negative; y++)
    {
        for (int16_t x = 0; x < bg_dots.columns + adding_negative; x++)
        {
            SDL_RenderCopy(
                    renderer,
                    image_texture,
                    NULL,
                    &image_hitbox);
           
            // Draw the next field column right after the column before.
            image_hitbox.x += image_hitbox.w + padding_x;
        }
        
        image_hitbox.x = bg_dots.increment - (padding_x + side_lenght) *
            adding_negative;
        
        // Draw the next field row right after the row before
        image_hitbox.y += image_hitbox.h + padding_y;
    }

    bg_dots.increment++;
    if (bg_dots.increment >= fmax(padding_y, padding_x) + side_lenght)
    {
        bg_dots.increment = 0;
    }
    SDL_DestroyTexture(image_texture);
}

bool _belongs_to_player(Player *player, int16_t Y, int16_t X)
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
