#include "shop.h"

#include "game_state.h"

#include "raylib.h"



void draw_shop(void)
{
    int x = 30;
    int y = SCREEN_H - 170;


    DrawRectangle(
        x,
        y,
        260,
        140,
        (Color){20,15,12,230}
    );


    DrawRectangleLines(
        x,
        y,
        260,
        140,
        (Color){120,90,50,255}
    );


    DrawText(
        "Kitchen Helpers",
        x + 20,
        y + 15,
        22,
        (Color){240,220,160,255}
    );


    /*
        Guard button
    */

    DrawRectangle(
        x + 20,
        y + 55,
        100,
        55,
        G.selected == UNIT_GUARD
        ?
        (Color){70,90,180,255}
        :
        (Color){50,50,50,255}
    );


    DrawText(
        "Guard",
        x + 35,
        y + 68,
        18,
        WHITE
    );


    DrawText(
        "3 Gold",
        x + 35,
        y + 90,
        14,
        YELLOW
    );


    /*
        Cook button
    */

    DrawRectangle(
        x + 140,
        y + 55,
        100,
        55,
        G.selected == UNIT_COOK
        ?
        (Color){180,150,70,255}
        :
        (Color){50,50,50,255}
    );


    DrawText(
        "Cook",
        x + 160,
        y + 68,
        18,
        WHITE
    );


    DrawText(
        "2 Gold",
        x + 160,
        y + 90,
        14,
        YELLOW
    );


    /*
        Placement mode indicator
    */

    if(G.place_mode)
    {
        DrawText(
            "Click a tile to place",
            x + 20,
            y + 120,
            15,
            GREEN
        );
    }
}