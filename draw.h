#ifndef DRAW_H
#define DRAW_H

/* =========================================================
   draw.h
   ------
   Declarations for every draw function in the game.
   Character drawing, grid, HUD, shop, screens — all here.
   ========================================================= */

#include "globals.h"

/* Character drawing */
void draw_guard(float cx, float cy, float scale);
void draw_cook(float cx, float cy, float scale);
void draw_suspect(float cx, float cy, float scale, float bob);
void draw_brute(float cx, float cy, float scale, float bob);

/* Grid and game elements */
void draw_grid(void);
void draw_clue_tiles(void);
void draw_units_grid(void);
void draw_enemies_grid(void);
void draw_particles_all(void);

/* HUD and UI */
void draw_hud(void);
void draw_shop(void);

/* Screens */
void draw_map(void);
void draw_gameover(void);
void draw_win(void);

#endif
