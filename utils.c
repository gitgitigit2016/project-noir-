#include "utils.h"

float randf01(void) {
    return (float)rand() / (float)RAND_MAX;
}

int randi(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

bool point_in_rect(Vector2 p, Rectangle r) {
    return CheckCollisionPointRec(p, r);
}

void set_message(const char *text, float seconds) {
    strncpy(G.message, text, sizeof(G.message) - 1);
    G.message[sizeof(G.message) - 1] = '\0';
    G.message_timer = seconds;
}

Vector2 grid_center(int col, int row) {
    Vector2 p = {
        G.grid_x + col * G.cell_w + G.cell_w * 0.5f,
        G.grid_y + row * G.cell_h + G.cell_h * 0.5f
    };
    return p;
}

bool pixel_to_grid(Vector2 p, int *col, int *row) {
    float lx = p.x - G.grid_x;
    float ly = p.y - G.grid_y;

    if (lx < 0 || ly < 0) return false;

    *col = (int)(lx / G.cell_w);
    *row = (int)(ly / G.cell_h);

    if (*col < 0 || *col >= GRID_COLS) return false;
    if (*row < 0 || *row >= GRID_ROWS) return false;

    return true;
}

bool cell_has_unit(int col, int row) {
    for (int i = 0; i < G.unit_count; i++) {
        if (!G.units[i].active) continue;
        if (G.units[i].col == col && G.units[i].row == row) return true;
    }
    return false;
}

int evidence_at_cell(int col, int row) {
    for (int i = 0; i < TOTAL_EVIDENCE; i++) {
        if (EVIDENCE[i].col == col && EVIDENCE[i].row == row) return i;
    }
    return -1;
}

bool evidence_unlocked(int id) {
    if (id < 0 || id >= TOTAL_EVIDENCE) return false;
    if (id <= 2) return true;
    if (id <= 4) return G.evidence_count >= 2;
    return G.evidence_count >= 4;
}

const char *threat_name(void) {
    if (G.evidence_count < 3) return "LOW";
    if (G.evidence_count < 5) return "MEDIUM";
    return "HIGH";
}

int unit_cost(UnitType type) {
    return (type == UNIT_GUARD) ? GUARD_COST : COOK_COST;
}

Rectangle guard_button_rect(void) {
    return (Rectangle){20, SCREEN_H - 82, 150, 58};
}

Rectangle cook_button_rect(void) {
    return (Rectangle){180, SCREEN_H - 82, 150, 58};
}

Rectangle case_button_rect(void) {
    return (Rectangle){870, SCREEN_H - 82, 170, 58};
}

Rectangle accuse_button_rect(void) {
    return (Rectangle){1050, SCREEN_H - 82, 205, 58};
}

bool button(Rectangle r, const char *label, bool enabled) {
    Vector2 mouse = GetMousePosition();
    bool hover = enabled && point_in_rect(mouse, r);

    Color fill;
    if (!enabled) fill = (Color){55, 55, 55, 255};
    else if (hover) fill = (Color){145, 125, 35, 255};
    else fill = (Color){90, 78, 25, 255};

    DrawRectangleRec(r, fill);
    DrawRectangleLinesEx(r, 2, enabled ? RAYWHITE : GRAY);

    int size = 18;
    int tw = MeasureText(label, size);
    DrawText(label,
             (int)(r.x + r.width * 0.5f - tw * 0.5f),
             (int)(r.y + r.height * 0.5f - size * 0.5f),
             size,
             enabled ? RAYWHITE : GRAY);

    return enabled && hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void draw_wrapped_text(const char *text,
                       int x,
                       int y,
                       int max_width,
                       int font_size,
                       int line_gap,
                       Color color) {
    char copy[1024];
    strncpy(copy, text, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char line[512] = "";
    char test[512] = "";

    char *word = strtok(copy, " ");
    int cy = y;

    while (word != NULL) {
        if (line[0] == '\0') snprintf(test, sizeof(test), "%s", word);
        else snprintf(test, sizeof(test), "%s %s", line, word);

        if (MeasureText(test, font_size) > max_width && line[0] != '\0') {
            DrawText(line, x, cy, font_size, color);
            cy += font_size + line_gap;
            snprintf(line, sizeof(line), "%s", word);
        } else {
            snprintf(line, sizeof(line), "%s", test);
        }

        word = strtok(NULL, " ");
    }

    if (line[0] != '\0') DrawText(line, x, cy, font_size, color);
}
