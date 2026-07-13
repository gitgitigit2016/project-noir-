#include "hud.h"

#include "game_state.h"

#include "raylib.h"

#include <math.h>


void draw_hud(void)
{
    DrawRectangle(
        0,
        0,
        SCREEN_W,
        50,
        (Color){14,10,8,245}
    );


    DrawText(
        TextFormat(
            "NOIR  |  The Kitchen  |  Gold: %d  |  Wave: %d/%d",
            G.gold,
            G.wave,
            WAVE_COUNT
        ),
        14,
        14,
        18,
        (Color){220,200,160,255}
    );


    DrawText(
        "Safe:",
        SCREEN_W - 200,
        14,
        18,
        (Color){180,160,130,255}
    );


    for(int i = 0; i < 3; i++)
    {
        Color hc =
            (i < G.safe_hp)
            ?
            RED
            :
            (Color){60,30,30,255};


        DrawRectangle(
            SCREEN_W - 130 + i * 36,
            14,
            26,
            22,
            hc
        );
    }


    if(!G.wave_active && G.wave < WAVE_COUNT)
    {
        DrawText(
            TextFormat(
                "Next wave in: %.1f",
                G.wave_timer
            ),
            SCREEN_W - 320,
            58,
            15,
            ORANGE
        );


        DrawRectangle(
            SCREEN_W-320,
            76,
            280,
            7,
            (Color){50,30,10,255}
        );


        DrawRectangle(
            SCREEN_W-320,
            76,
            (int)(280 * (1.f - G.wave_timer / 5.f)),
            7,
            ORANGE
        );
    }


    else if(G.wave_active)
    {
        DrawText(
            TextFormat(
                "WAVE %d - ACTIVE",
                G.wave
            ),
            SCREEN_W - 280,
            58,
            15,
            RED
        );
    }


    if(G.message_timer > 0.f)
    {
        float alpha = fminf(
            1.f,
            G.message_timer
        );


        int tw = MeasureText(
            G.message,
            20
        );


        DrawRectangle(
            (SCREEN_W-tw)/2 - 12,
            55,
            tw+24,
            30,
            (Color)
            {
                0,
                0,
                0,
                (unsigned char)(180*alpha)
            }
        );


        DrawText(
            G.message,
            (SCREEN_W-tw)/2,
            60,
            20,
            (Color)
            {
                255,
                240,
                180,
                (unsigned char)(255*alpha)
            }
        );
    }
}