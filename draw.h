#ifndef DRAW_H
#define DRAW_H
#include "globals.h"
void draw_guard(float cx, float cy, float scale);
void draw_cook(float cx, float cy, float scale);
void draw_suspect(float cx, float cy, float scale, float bob);
void draw_brute(float cx, float cy, float scale, float bob);
void draw_grid(void);
void draw_clue_tiles(void);
void draw_units_grid(void);
void draw_enemies_grid(void);
void draw_hud(void);
void draw_shop(void);
void draw_map(void);
void draw_gameover(void);
void draw_win(void);

#endif
