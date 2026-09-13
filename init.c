#include "init.h"
#include "utils.h"

static void reset_evidence(void) {
    for (int i = 0; i < TOTAL_EVIDENCE; i++) EVIDENCE[i].collected = false;
}

void init_game(void) {
    srand((unsigned int)time(NULL));
    memset(&G, 0, sizeof(G));
    G.state = STATE_TITLE;
}

void start_game(void) {
    memset(&G, 0, sizeof(G));

    G.state = STATE_PLAY;
    G.panel = PANEL_NONE;
    G.grid_x = 110.0f;
    G.grid_y = 125.0f;
    G.cell_w = 88.0f;
    G.cell_h = 78.0f;
    G.credits = START_CREDITS;
    G.archive_hp = ARCHIVE_MAX_HP;
    G.selected_unit = UNIT_GUARD;
    G.placing_unit = false;
    G.spawn_timer = 2.0f;

    reset_evidence();
    set_message("Defend the Evidence Archive. Spend Credits carefully.", 4.0f);
}
