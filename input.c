#include "input.h"

#include "game_state.h"
#include "constants.h"
#include "utils.h"

#include "raylib.h"



void update_input(void)
{

    Vector2 mp =
        GetMousePosition();



    float shop_y =
        SCREEN_H - 95.f;



    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {

        if(mp.x > 20 &&
           mp.x < 140 &&
           mp.y > shop_y &&
           mp.y < shop_y + 75)
        {

            G.selected =
                UNIT_GUARD;


            G.place_mode = true;

        }


        else if(mp.x > 150 &&
                mp.x < 270 &&
                mp.y > shop_y &&
                mp.y < shop_y + 75)
        {

            G.selected =
                UNIT_COOK;


            G.place_mode = true;

        }


        else
        {

            int col;
            int row;


            if(G.place_mode &&
               pixel_to_grid(
                    mp,
                    &col,
                    &row))
            {

                place_unit(
                    G.selected,
                    col,
                    row
                );

            }

        }

    }




    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        G.place_mode = false;
    }

}