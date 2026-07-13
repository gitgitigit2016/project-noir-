#include "characters.h"
#include "raylib.h"


void draw_guard(float cx,float cy,float scale)
{
    int x=(int)cx;
    int y=(int)cy;
    int s=(int)scale;


    DrawRectangle(
        x-s,
        y-s,
        s*2,
        s*3,
        (Color){50,80,200,255}
    );


    DrawCircle(
        x,
        y-s*2,
        s*1.2f,
        (Color){220,180,140,255}
    );


    DrawText(
        "G",
        x-5,
        y-s*3+2,
        14,
        WHITE
    );
}