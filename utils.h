#ifndef UTILS_H
#define UTILS_H
#include "globals.h"
float   randf(void);
int     randi(int lo, int hi);
Vector2 grid_to_pixel(int col, int row);
bool    pixel_to_grid(Vector2 p, int *col, int *row);
bool    cell_has_unit(int col, int row);
void    set_message(const char *msg, float dur);
#endif
