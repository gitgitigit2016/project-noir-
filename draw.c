#include "draw.h"

void draw_guard(float cx, float cy, float scale) {
    int x=(int)cx,y=(int)cy,s=(int)scale;
    DrawRectangle(x-s,y-s,s*2,s*3,YELLOW);
    DrawCircle(x,y-s*2,s*1.2f,WHITE);
    DrawText("G",x-5,y-s*3+2,14,BLACK);
}

void draw_cook(float cx,float cy,float scale){
    int x=(int)cx,y=(int)cy,s=(int)scale;
    DrawRectangle(x-s,y-s,s*2,s*3,WHITE);
    DrawCircle(x,y-s*2,s*1.2f,WHITE);
    DrawText("C",x-5,y-s*3+2,14,BLACK);
}

void draw_suspect(float cx,float cy,float scale,float bob){
    int x=(int)cx,y=(int)(cy+bob),s=(int)scale;
    DrawRectangle(x-s,y-s,s*2,s*3,WHITE);
    DrawCircle(x,y-s*2,s*1.2f,YELLOW);
    DrawCircle(x-s/2,y-s*2,s*0.3f,YELLOW);
    DrawCircle(x+s/2,y-s*2,s*0.3f,YELLOW);
}

void draw_brute(float cx,float cy,float scale,float bob){
    int x=(int)cx,y=(int)(cy+bob),s=(int)(scale*1.4f);
    DrawRectangle(x-s,y-s,s*2,s*3,YELLOW);
    DrawCircle(x,y-s*2,s*1.3f,WHITE);
    DrawCircle(x-s/2,y-s*2,s*0.35f,YELLOW);
    DrawCircle(x+s/2,y-s*2,s*0.35f,YELLOW);
}


void draw_grid(void){

    for(int row=0;row<GRID_ROWS;row++){
        for(int col=0;col<GRID_COLS;col++){

            float cx=G.grid_ox+col*G.cell_w;
            float cy=G.grid_oy+row*G.cell_h;

            Color tile_color=(row%2==0)?
            (Color){15,15,15,255}:
            (Color){25,25,25,255};

            if(col==0)
                tile_color=(Color){30,30,10,255};

            DrawRectangle(
                (int)cx+1,
                (int)cy+1,
                (int)G.cell_w-2,
                (int)G.cell_h-2,
                tile_color
            );

            DrawRectangleLinesEx(
                (Rectangle){cx,cy,G.cell_w,G.cell_h},
                1,
                (Color){255,255,255,100}
            );
        }
    }

    float sh=GRID_ROWS*G.cell_h;

    DrawRectangleLinesEx(
        (Rectangle){G.grid_ox,G.grid_oy,G.cell_w,sh},
        3,
        YELLOW
    );

    DrawText(
        "SAFE",
        (int)(G.grid_ox+G.cell_w*0.5f-20),
        (int)(G.grid_oy+sh*0.5f-8),
        14,
        YELLOW
    );

    for(int row=0;row<GRID_ROWS;row++){

        float dy=G.grid_oy+row*G.cell_h;
        float dx=G.grid_ox+(GRID_COLS-1)*G.cell_w;

        DrawRectangle(
            (int)(dx+G.cell_w-6),
            (int)dy,
            8,
            (int)G.cell_h,
            WHITE
        );
    }
}


void draw_clue_tiles(void){

    float pulse=1.0f;

    for(int i=0;i<G.clue_tile_count;i++){

        float cx=G.grid_ox+G.clue_tiles[i].col*G.cell_w;
        float cy=G.grid_oy+G.clue_tiles[i].row*G.cell_h;

        DrawRectangle(
            (int)cx+2,
            (int)cy+2,
            (int)G.cell_w-4,
            (int)G.cell_h-4,
            (Color){20,20,20,160}
        );

        DrawText(
            "?",
            (int)(cx+G.cell_w/2-7),
            (int)(cy+G.cell_h/2-10),
            22,
            (Color){255,220,50,(unsigned char)(160+95*pulse)}
        );
    }
}








void draw_units_grid(void){

    float scale=fminf(G.cell_w,G.cell_h)*0.17f;
    if(scale<4) scale=4;

    for(int i=0;i<G.unit_count;i++){

        if(!G.units[i].active) continue;

        if(G.units[i].type==UNIT_GUARD)
            draw_guard(G.units[i].x,G.units[i].y,scale);
        else
            draw_cook(G.units[i].x,G.units[i].y,scale);
    }
}


void draw_enemies_grid(void){

    float scale=fminf(G.cell_w,G.cell_h)*0.15f;
    if(scale<4) scale=4;

    for(int i=0;i<MAX_ENEMIES;i++){

        Enemy *e=&G.enemies[i];

        if(!e->active) continue;

        float bob=0;

        if(e->type==ENEMY_SUSPECT)
            draw_suspect(e->x,e->y,scale,bob);
        else
            draw_brute(e->x,e->y,scale,bob);


        float pct=e->hp/e->max_hp;
        float barw=fminf(G.cell_w,G.cell_h)*0.85f;
        float bary=e->y-scale*6.5f+bob;

        DrawRectangle(
            (int)(e->x-barw/2),
            (int)bary,
            (int)barw,
            5,
            (Color){40,40,40,200}
        );

        DrawRectangle(
            (int)(e->x-barw/2),
            (int)bary,
            (int)(barw*pct),
            5,
            YELLOW
        );
    }
}


void draw_hud(void){

    DrawRectangle(
        0,
        0,
        SCREEN_W,
        50,
        (Color){10,10,10,245}
    );


    DrawText(
        TextFormat("NOIR  |  The Kitchen  |  Gold: %d  |  Wave: %d/%d",
        G.gold,G.wave,WAVE_COUNT),
        14,
        14,
        18,
        WHITE
    );


    DrawText(
        "Safe:",
        SCREEN_W-200,
        14,
        18,
        WHITE
    );


    for(int i=0;i<3;i++){

        Color hc=(i<G.safe_hp)?YELLOW:(Color){50,50,50,255};

        DrawRectangle(
            SCREEN_W-130+i*36,
            14,
            26,
            22,
            hc
        );
    }


    if(!G.wave_active&&G.wave<WAVE_COUNT){

        DrawText(
            TextFormat("Next wave in: %.1f",G.wave_timer),
            SCREEN_W-320,
            58,
            15,
            YELLOW
        );

        DrawRectangle(
            SCREEN_W-320,
            76,
            280,
            7,
            (Color){40,40,40,255}
        );

        DrawRectangle(
            SCREEN_W-320,
            76,
            (int)(280*(1.f-G.wave_timer/5.f)),
            7,
            YELLOW
        );

    }else if(G.wave_active){

        DrawText(
            TextFormat("WAVE %d  -  ACTIVE",G.wave),
            SCREEN_W-280,
            58,
            15,
            YELLOW
        );
    }


    if(G.message_timer>0.f){

        float alpha=fminf(1.f,G.message_timer);
        int tw=MeasureText(G.message,20);

        DrawRectangle(
            (SCREEN_W-tw)/2-12,
            55,
            tw+24,
            30,
            (Color){0,0,0,(unsigned char)(180*alpha)}
        );

        DrawText(
            G.message,
            (SCREEN_W-tw)/2,
            60,
            20,
            (Color){255,220,80,(unsigned char)(255*alpha)}
        );
    }
}


void draw_shop(void){

    float sy=SCREEN_H-95.f;

    DrawRectangle(
        0,
        (int)sy-5,
        SCREEN_W,
        100,
        (Color){10,10,10,235}
    );


    DrawLine(
        0,
        (int)sy-5,
        SCREEN_W,
        (int)sy-5,
        WHITE
    );


    Vector2 mp=GetMousePosition();

    bool hov=(mp.x>20&&mp.x<140&&mp.y>sy&&mp.y<sy+75);

    Color bc=(G.selected==UNIT_GUARD&&G.place_mode)?
    YELLOW:
    hov?
    (Color){120,120,20,255}:
    (Color){60,60,20,255};


    DrawRectangle(
        20,
        (int)sy,
        120,
        75,
        bc
    );

    draw_guard(80,sy+52,10);

    DrawText(
        "GUARD",
        28,
        (int)sy+6,
        14,
        BLACK
    );


    hov=(mp.x>150&&mp.x<270&&mp.y>sy&&mp.y<sy+75);

    bc=(G.selected==UNIT_COOK&&G.place_mode)?
    YELLOW:
    hov?
    (Color){120,120,120,255}:
    WHITE;


    DrawRectangle(
        150,
        (int)sy,
        120,
        75,
        bc
    );

    draw_cook(210,sy+52,10);

    DrawText(
        "COOK",
        158,
        (int)sy+6,
        14,
        BLACK
    );


    DrawText(
        "Left-click grid to place | Right-click cancel",
        310,
        (int)sy+15,
        14,
        WHITE
    );
}


void draw_map(void){

    ClearBackground(BLACK);

    DrawText(
        "NOIR",
        SCREEN_W/2-MeasureText("NOIR",64)/2,
        40,
        64,
        YELLOW
    );


    DrawText(
        "~ Investigation Board ~",
        SCREEN_W/2-MeasureText("~ Investigation Board ~",22)/2,
        118,
        22,
        WHITE
    );
}


void draw_gameover(void){

    ClearBackground(BLACK);

    DrawText(
        "THE SAFE WAS BREACHED",
        SCREEN_W/2-MeasureText("THE SAFE WAS BREACHED",44)/2,
        220,
        44,
        YELLOW
    );

    DrawText(
        "[R] TO TRY AGAIN",
        SCREEN_W/2-MeasureText("[R] TO TRY AGAIN",22)/2,
        520,
        22,
        WHITE
    );
}


void draw_win(void){

    ClearBackground(BLACK);

    DrawText(
        "KITCHEN DEFENDED!",
        SCREEN_W/2-MeasureText("KITCHEN DEFENDED!",52)/2,
        220,
        52,
        YELLOW
    );

    DrawText(
        "[R] TO PLAY AGAIN",
        SCREEN_W/2-MeasureText("[R] TO PLAY AGAIN",22)/2,
        520,
        22,
        WHITE
    );
}