#include "screens.h"

#include "game_state.h"

#include "raylib.h"



void draw_map(void)
{
    ClearBackground(
        (Color){18,12,8,255}
    );


    DrawText(
        "NOIR",
        SCREEN_W / 2 -
        MeasureText("NOIR",60)/2,
        180,
        60,
        (Color){230,200,120,255}
    );


    DrawText(
        "The Kitchen Mystery",
        SCREEN_W / 2 -
        MeasureText("The Kitchen Mystery",25)/2,
        270,
        25,
        WHITE
    );


    DrawText(
        "Defend the kitchen and protect the clues!",
        SCREEN_W / 2 -
        MeasureText(
            "Defend the kitchen and protect the clues!",
            20
        )/2,
        340,
        20,
        (Color){200,180,150,255}
    );
}



void draw_gameover(void)
{
    ClearBackground(
        (Color){15,5,5,255}
    );


    const char *title =
        "THE SAFE WAS BREACHED";


    DrawText(
        title,
        SCREEN_W / 2 -
        MeasureText(title,44)/2,
        220,
        44,
        RED
    );


    const char *sub =
        "Evidence destroyed. The killer walks free.";


    DrawText(
        sub,
        SCREEN_W / 2 -
        MeasureText(sub,22)/2,
        300,
        22,
        (Color){200,150,150,255}
    );


    DrawText(
        "[R] to try again",
        SCREEN_W / 2 -
        MeasureText("[R] to try again",22)/2,
        520,
        22,
        (Color){200,160,160,255}
    );
}



void draw_win(void)
{
    ClearBackground(
        (Color){5,15,5,255}
    );


    const char *title =
        "KITCHEN DEFENDED!";


    DrawText(
        title,
        SCREEN_W / 2 -
        MeasureText(title,52)/2,
        220,
        52,
        (Color){100,255,120,255}
    );


    const char *sub =
        "All waves repelled. Clues secured.";


    DrawText(
        sub,
        SCREEN_W / 2 -
        MeasureText(sub,22)/2,
        300,
        22,
        (Color){160,220,160,255}
    );


    DrawText(
        TextFormat(
            "Gold remaining: %d",
            G.gold
        ),
        SCREEN_W / 2 - 100,
        370,
        22,
        (Color){220,180,50,255}
    );


    DrawText(
        "[R] to play again",
        SCREEN_W / 2 -
        MeasureText("[R] to play again",22)/2,
        520,
        22,
        (Color){150,200,150,255}
    );
}