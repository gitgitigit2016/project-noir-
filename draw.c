#include "draw.h"
void draw_guard(float cx, float cy, float scale) {
    int x = (int)cx, y = (int)cy, s = (int)scale;
    DrawRectangle(x - s, y - s, s * 2, s * 3, (Color){50, 80, 200, 255});
    DrawCircle(x, y - s * 2, s * 1.2f, (Color){220, 180, 140, 255});
    DrawText("G", x - 5, y - s * 3 + 2, 14, WHITE);
}
void draw_cook(float cx, float cy, float scale) {
    int x = (int)cx, y = (int)cy, s = (int)scale;
    DrawRectangle(x - s, y - s, s * 2, s * 3, (Color){240, 240, 220, 255});
    DrawCircle(x, y - s * 2, s * 1.2f, (Color){220, 180, 140, 255});
    DrawText("C", x - 5, y - s * 3 + 2, 14, (Color){40, 40, 40, 255});
}
void draw_suspect(float cx, float cy, float scale, float bob) {
    int x = (int)cx, y = (int)(cy + bob), s = (int)scale;
    DrawRectangle(x - s, y - s, s * 2, s * 3, (Color){60, 40, 30, 255});
    DrawCircle(x, y - s * 2, s * 1.2f, (Color){180, 80, 80, 255});
    DrawCircle(x - s / 2, y - s * 2, s * 0.3f, RED);
    DrawCircle(x + s / 2, y - s * 2, s * 0.3f, RED);
}
void draw_brute(float cx, float cy, float scale, float bob) {
    int x = (int)cx, y = (int)(cy + bob), s = (int)(scale * 1.4f);
    DrawRectangle(x - s, y - s, s * 2, s * 3, (Color){80, 30, 20, 255});
    DrawCircle(x, y - s * 2, s * 1.3f, (Color){140, 60, 40, 255});
    DrawCircle(x - s / 2, y - s * 2, s * 0.35f, ORANGE);
    DrawCircle(x + s / 2, y - s * 2, s * 0.35f, ORANGE);
}
void draw_grid(void) {
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            float cx = G.grid_ox + col * G.cell_w;
            float cy = G.grid_oy + row * G.cell_h;

            Color tile_color = (row % 2 == 0) ? (Color){34,20,12,255}
                                              : (Color){28,16,9,255};
            if (col == 0) tile_color = (Color){18,50,18,255};

            DrawRectangle((int)cx+1, (int)cy+1,
                          (int)G.cell_w-2, (int)G.cell_h-2, tile_color);
            DrawRectangleLinesEx((Rectangle){cx, cy, G.cell_w, G.cell_h},
                                 1, (Color){55,40,28,180});
        }
    }

    float sh = GRID_ROWS * G.cell_h;
    DrawRectangleLinesEx((Rectangle){G.grid_ox, G.grid_oy, G.cell_w, sh},
                         3, (Color){60,180,60,255});
    DrawText("SAFE",
             (int)(G.grid_ox + G.cell_w * 0.5f - 20),
             (int)(G.grid_oy + sh * 0.5f - 8),
             14, (Color){80,220,80,255});

    for (int row = 0; row < GRID_ROWS; row++) {
        float dy = G.grid_oy + row * G.cell_h;
        float dx = G.grid_ox + (GRID_COLS - 1) * G.cell_w;
        DrawRectangle((int)(dx + G.cell_w - 6), (int)dy,
                      8, (int)G.cell_h, (Color){100,55,25,255});
    }
}

void draw_clue_tiles(void) {
    float pulse = 1.0f;

    for (int i = 0; i < G.clue_tile_count; i++) {
        float cx = G.grid_ox + G.clue_tiles[i].col * G.cell_w;
        float cy = G.grid_oy + G.clue_tiles[i].row * G.cell_h;

        DrawRectangle((int)cx+2, (int)cy+2,
                      (int)G.cell_w-4, (int)G.cell_h-4,
                      (Color){30, 30, 80, 160});
        DrawText("?",
                 (int)(cx + G.cell_w/2 - 7),
                 (int)(cy + G.cell_h/2 - 10),
                 22,
                 (Color){100, 120, 255, (unsigned char)(160 + 95 * pulse)});
    }
}

void draw_units_grid(void) {
    float scale = fminf(G.cell_w, G.cell_h) * 0.17f;
    if (scale < 4) scale = 4;

    for (int i = 0; i < G.unit_count; i++) {
        if (!G.units[i].active) continue;
        if (G.units[i].type == UNIT_GUARD)
            draw_guard(G.units[i].x, G.units[i].y, scale);
        else
            draw_cook(G.units[i].x, G.units[i].y, scale);
    }
}

void draw_enemies_grid(void) {
    float scale = fminf(G.cell_w, G.cell_h) * 0.15f;
    if (scale < 4) scale = 4;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &G.enemies[i];
        if (!e->active) continue;

        float bob = 0;

        if (e->type == ENEMY_SUSPECT) draw_suspect(e->x, e->y, scale, bob);
        else                          draw_brute  (e->x, e->y, scale, bob);

        float pct  = e->hp / e->max_hp;
        float barw = fminf(G.cell_w, G.cell_h) * 0.85f;
        float bary = e->y - scale * 6.5f + bob;

        DrawRectangle((int)(e->x - barw/2), (int)bary, (int)barw, 5,
                      (Color){40,20,20,200});
        DrawRectangle((int)(e->x - barw/2), (int)bary, (int)(barw * pct), 5,
                      pct > 0.5f ? GREEN :
                      pct > 0.25f ? (Color){255,200,0,255} : RED);
    }
}
void draw_hud(void) {
    DrawRectangle(0, 0, SCREEN_W, 50, (Color){14,10,8,245});

    DrawText(TextFormat("NOIR  |  The Kitchen  |  Gold: %d  |  Wave: %d/%d",
             G.gold, G.wave, WAVE_COUNT),
             14, 14, 18, (Color){220,200,160,255});

    DrawText("Safe:", SCREEN_W - 200, 14, 18, (Color){180,160,130,255});
    for (int i = 0; i < 3; i++) {
        Color hc = (i < G.safe_hp) ? RED : (Color){60,30,30,255};
        DrawRectangle(SCREEN_W - 130 + i * 36, 14, 26, 22, hc);
    }

    if (!G.wave_active && G.wave < WAVE_COUNT) {
        DrawText(TextFormat("Next wave in: %.1f", G.wave_timer),
                 SCREEN_W - 320, 58, 15, ORANGE);
        DrawRectangle(SCREEN_W-320, 76, 280, 7, (Color){50,30,10,255});
        DrawRectangle(SCREEN_W-320, 76,
                      (int)(280 * (1.f - G.wave_timer / 5.f)), 7, ORANGE);
    } else if (G.wave_active) {
        DrawText(TextFormat("WAVE %d  -  ACTIVE", G.wave),
                 SCREEN_W - 280, 58, 15, RED);
    }

    if (G.message_timer > 0.f) {
        float alpha = fminf(1.f, G.message_timer);
        int tw = MeasureText(G.message, 20);
        DrawRectangle((SCREEN_W-tw)/2 - 12, 55, tw+24, 30,
                      (Color){0,0,0,(unsigned char)(180*alpha)});
        DrawText(G.message, (SCREEN_W-tw)/2, 60, 20,
                 (Color){255,240,180,(unsigned char)(255*alpha)});
    }
}

void draw_shop(void) {
    float sy = SCREEN_H - 95.f;

    DrawRectangle(0, (int)sy - 5, SCREEN_W, 100, (Color){14,12,10,235});
    DrawLine(0, (int)sy - 5, SCREEN_W, (int)sy - 5, (Color){70,55,35,200});

    Vector2 mp = GetMousePosition();
    float scale = fminf(G.cell_w, G.cell_h) * 0.17f;
    if (scale < 4) scale = 4;

    bool hov = (mp.x>20 && mp.x<140 && mp.y>sy && mp.y<sy+75);
    Color bc  = (G.selected==UNIT_GUARD && G.place_mode) ? (Color){80,110,200,255} :
                hov ? (Color){55,70,130,255} : (Color){35,48,100,255};
    DrawRectangle(20, (int)sy, 120, 75, bc);
    DrawRectangleLinesEx((Rectangle){20,sy,120,75}, 2, (Color){100,140,255,255});
    draw_guard(80, sy + 52, scale);
    DrawText("GUARD", 28, (int)sy+6,  14, (Color){180,210,255,255});
    DrawText(TextFormat("%dg", COST_GUARD), 28, (int)sy+57, 13, (Color){220,180,50,255});

    hov = (mp.x>150 && mp.x<270 && mp.y>sy && mp.y<sy+75);
    bc  = (G.selected==UNIT_COOK && G.place_mode) ? (Color){200,160,60,255} :
          hov ? (Color){130,95,30,255} : (Color){90,65,20,255};
    DrawRectangle(150, (int)sy, 120, 75, bc);
    DrawRectangleLinesEx((Rectangle){150,sy,120,75}, 2, (Color){255,190,80,255});
    draw_cook(210, sy + 52, scale);
    DrawText("COOK", 158, (int)sy+6,  14, (Color){255,210,130,255});
    DrawText(TextFormat("%dg", COST_COOK), 158, (int)sy+57, 13, (Color){220,180,50,255});

    DrawText("Left-click grid to place   |   Right-click to cancel",
             310, (int)sy+15, 14, (Color){140,130,110,255});
    DrawText("Guard: hits same lane only     Cook: hits all nearby enemies",
             310, (int)sy+36, 13, (Color){120,110,90,255});
    DrawText("? = Clue tile   (Investigator coming next build!)",
             310, (int)sy+58, 13, (Color){90,100,160,255});
}
void draw_map(void) {
    ClearBackground((Color){12,10,8,255});

    DrawText("NOIR",
             SCREEN_W/2 - MeasureText("NOIR",64)/2, 40, 64,
             (Color){200,180,140,255});
    DrawText("~ Investigation Board ~",
             SCREEN_W/2 - MeasureText("~ Investigation Board ~",22)/2,
             118, 22, (Color){140,120,100,255});
    DrawText("Select a room to begin.",
             SCREEN_W/2 - MeasureText("Select a room to begin.",16)/2,
             155, 16, (Color){160,140,120,255});

    Vector2 mp = GetMousePosition();
    bool hov = (mp.x>440 && mp.x<840 && mp.y>260 && mp.y<370);
    DrawRectangle(440, 260, 400, 110,
                  hov ? (Color){80,50,30,255} : (Color){45,32,20,255});
    DrawRectangleLinesEx((Rectangle){440,260,400,110}, 3, (Color){180,100,60,255});
    DrawText("The Kitchen",              460, 278, 28, WHITE);
    DrawText("Grid: 12 x 3  -  3 Waves",460, 318, 16, (Color){180,160,130,255});
    if (hov) DrawText("Click to enter", 460, 346, 14, (Color){255,200,140,255});

    const char *locked[] = {"The Ballroom","The Library","The Cellar","The Study"};
    for (int i = 0; i < 4; i++) {
        DrawRectangle(440, 390+i*68, 400, 54, (Color){22,18,14,255});
        DrawRectangleLinesEx((Rectangle){440,390+i*68.f,400,54},
                             2, (Color){40,34,26,255});
        DrawText(locked[i], 460, 404+i*68, 20, (Color){65,55,45,255});
        DrawText("LOCKED",  740, 408+i*68, 15, (Color){60,50,42,255});
    }
}

void draw_gameover(void) {
    ClearBackground((Color){15,5,5,255});
    DrawText("THE SAFE WAS BREACHED",
             SCREEN_W/2 - MeasureText("THE SAFE WAS BREACHED",44)/2,
             220, 44, RED);
    DrawText("Evidence destroyed. The killer walks free.",
             SCREEN_W/2 - MeasureText("Evidence destroyed. The killer walks free.",22)/2,
             300, 22, (Color){200,150,150,255});
    DrawText("[R] to try again",
             SCREEN_W/2 - MeasureText("[R] to try again",22)/2,
             520, 22, (Color){200,160,160,255});
}

void draw_win(void) {
    ClearBackground((Color){5,15,5,255});
    DrawText("KITCHEN DEFENDED!",
             SCREEN_W/2 - MeasureText("KITCHEN DEFENDED!",52)/2,
             220, 52, (Color){100,255,120,255});
    DrawText("All waves repelled. Clues secured.",
             SCREEN_W/2 - MeasureText("All waves repelled. Clues secured.",22)/2,
             300, 22, (Color){160,220,160,255});
    DrawText(TextFormat("Gold remaining: %d", G.gold),
             SCREEN_W/2 - 80, 370, 22, (Color){220,180,50,255});
    DrawText("[R] to play again",
             SCREEN_W/2 - MeasureText("[R] to play again",22)/2,
             520, 22, (Color){150,200,150,255});
}

