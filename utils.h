#ifndef UTILS_H
#define UTILS_H
#include "globals.h"

float randf01(void);
int randi(int lo, int hi);
bool point_in_rect(Vector2 p, Rectangle r);
void set_message(const char *text, float seconds);
Vector2 grid_center(int col, int row);
bool pixel_to_grid(Vector2 p, int *col, int *row);
bool cell_has_unit(int col, int row);
int evidence_at_cell(int col, int row);
bool evidence_unlocked(int id);
const char *threat_name(void);
int unit_cost(UnitType type);
Rectangle guard_button_rect(void);
Rectangle cook_button_rect(void);
Rectangle case_button_rect(void);
Rectangle accuse_button_rect(void);
bool button(Rectangle r, const char *label, bool enabled);
void draw_wrapped_text(const char *text, int x, int y, int max_width, int font_size, int line_gap, Color color);

#endif
