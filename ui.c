#include "ui.h"

#include "game_state.h"

#include "raylib.h"
#include <math.h>



void draw_gameover(void)
{
    ClearBackground((Color){15,5,5,255});

    DrawText(
        "THE SAFE WAS BREACHED",
        SCREEN_W/2 - MeasureText("THE SAFE WAS BREACHED",44)/2,
        220,
        44,
        RED
    );

    DrawText(
        "Evidence destroyed. The killer walks free.",
        SCREEN_W/2 - MeasureText("Evidence destroyed. The killer walks free.",22)/2,
        300,
        22,
        (Color){200,150,150,255}
    );

    DrawText(
        "[R] to try again",
        SCREEN_W/2 - MeasureText("[R] to try again",22)/2,
        520,
        22,
        (Color){200,160,160,255}
    );
}


void draw_win(void)
{
    ClearBackground((Color){5,15,5,255});

    DrawText(
        "KITCHEN DEFENDED!",
        SCREEN_W/2 - MeasureText("KITCHEN DEFENDED!",52)/2,
        220,
        52,
        (Color){100,255,120,255}
    );

    DrawText(
        "All waves repelled. Clues secured.",
        SCREEN_W/2 - MeasureText("All waves repelled. Clues secured.",22)/2,
        300,
        22,
        (Color){160,220,160,255}
    );

    DrawText(
        TextFormat("Gold remaining: %d", G.gold),
        SCREEN_W/2 - 80,
        370,
        22,
        (Color){220,180,50,255}
    );

    DrawText(
        "[R] to play again",
        SCREEN_W/2 - MeasureText("[R] to play again",22)/2,
        520,
        22,
        (Color){150,200,150,255}
    );
}