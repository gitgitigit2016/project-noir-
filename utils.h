#ifndef UTILS_H
#define UTILS_H

/* =========================================================
   utils.h
   -------
   Declarations (prototypes) for all utility functions.
   Any file that wants to call randf(), randi() etc
   just includes this header.
   ========================================================= */

#include "globals.h"

float   randf(void);
int     randi(int lo, int hi);
Vector2 grid_to_pixel(int col, int row);
bool    pixel_to_grid(Vector2 p, int *col, int *row);
bool    cell_has_unit(int col, int row);
void    set_message(const char *msg, float dur);
void    spawn_particles(float x, float y, Color c, int n);

#endif
