#include "utils.h"
float randf(void) { return (float)rand() / (float)RAND_MAX; }
int   randi(int lo, int hi) { return lo + rand() % (hi - lo + 1); }
Vector2 grid_to_pixel(int col, int row) {
    return (Vector2){
        G.grid_ox + col * G.cell_w + G.cell_w * 0.5f,
        G.grid_oy + row * G.cell_h + G.cell_h * 0.5f
    };
}
bool pixel_to_grid(Vector2 p, int *col, int *row) {
    float lx = p.x - G.grid_ox;
    float ly = p.y - G.grid_oy;
    if (lx < 0 || ly < 0) return false;
    *col = (int)(lx / G.cell_w);
    *row = (int)(ly / G.cell_h);
    if (*col >= GRID_COLS || *row >= GRID_ROWS) return false;
    return true;
}

bool cell_has_unit(int col, int row) {
    for (int i = 0; i < G.unit_count; i++)
        if (G.units[i].active && G.units[i].col == col && G.units[i].row == row)
            return true;
    return false;
}

void set_message(const char *msg, float dur) {
    strncpy(G.message, msg, 127);
    G.message_timer = dur;
}

