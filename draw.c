#include "draw.h"
#include "utils.h"

static Texture2D guard_texture;
static Texture2D cook_texture;
static Texture2D walker_texture;
static Texture2D brute_texture;
static Texture2D clue_texture;
static Texture2D grid_texture;

void load_draw_assets(void) {
    guard_texture = LoadTexture("assets/guard.png");
    cook_texture = LoadTexture("assets/cook.png");
    walker_texture = LoadTexture("assets/walker.png");
    brute_texture = LoadTexture("assets/brute.png");
    clue_texture = LoadTexture("assets/clue.png");
    grid_texture = LoadTexture("assets/grid.png");
}

void unload_draw_assets(void) {
    UnloadTexture(guard_texture);
    UnloadTexture(cook_texture);
    UnloadTexture(walker_texture);
    UnloadTexture(brute_texture);
    UnloadTexture(clue_texture);
    UnloadTexture(grid_texture);
}

static void draw_unit_shape(Unit *u) {
    Texture2D tex = (u->type == UNIT_GUARD) ? guard_texture : cook_texture;

    float w = (u->type == UNIT_GUARD) ? 96.0f : 62.0f;
    float h = (u->type == UNIT_GUARD) ? 104.0f : 78.0f;

    Rectangle source = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dest = {u->x - w * 0.5f, u->y - h * 0.5f, w, h};

    DrawTexturePro(tex, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

static void draw_enemy_shape(Enemy *e) {
    Texture2D tex = (e->type == ENEMY_BRUTE) ? brute_texture : walker_texture;

    float w = (e->type == ENEMY_BRUTE) ? 82.0f : 64.0f;
    float h = (e->type == ENEMY_BRUTE) ? 90.0f : 70.0f;

    Rectangle source = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dest = {e->x - w * 0.5f, e->y - h * 0.5f, w, h};

    DrawTexturePro(tex, source, dest, (Vector2){0, 0}, 0.0f, WHITE);

    float pct = e->hp / e->max_hp;
    if (pct < 0.0f) pct = 0.0f;

    DrawRectangle((int)e->x - 22, (int)e->y - 42, 44, 5, DARKGRAY);
    DrawRectangle((int)e->x - 22, (int)e->y - 42, (int)(44 * pct), 5, LIME);
}

static void draw_grid(void) {
    Rectangle source = {0, 0, (float)grid_texture.width, (float)grid_texture.height};
    Rectangle dest = {
        G.grid_x,
        G.grid_y,
        G.cell_w * GRID_COLS,
        G.cell_h * GRID_ROWS
    };

    DrawTexturePro(grid_texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);

    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            float x = G.grid_x + col * G.cell_w;
            float y = G.grid_y + row * G.cell_h;

            if (col == 0) {
                DrawRectangle((int)x + 1, (int)y + 1,
                              (int)G.cell_w - 2, (int)G.cell_h - 2,
                              (Color){65, 55, 18, 170});
            } else if (col == GRID_COLS - 1) {
                DrawRectangle((int)x + 1, (int)y + 1,
                              (int)G.cell_w - 2, (int)G.cell_h - 2,
                              (Color){52, 25, 25, 170});
            }

            DrawRectangleLinesEx((Rectangle){x, y, G.cell_w, G.cell_h},
                                 1, (Color){120, 170, 220, 170});
        }
    }

    DrawText("EVIDENCE", (int)G.grid_x + 6, (int)G.grid_y + 8, 14, GOLD);
    DrawText("ARCHIVE", (int)G.grid_x + 10, (int)G.grid_y + 26, 14, GOLD);
    DrawText("ENTRY", (int)(G.grid_x + (GRID_COLS - 1) * G.cell_w + 18),
             (int)G.grid_y + 8, 14, RAYWHITE);
}

static void draw_evidence_tiles(void) {
    for (int i = 0; i < TOTAL_EVIDENCE; i++) {
        Evidence *e = &EVIDENCE[i];
        float x = G.grid_x + e->col * G.cell_w;
        float y = G.grid_y + e->row * G.cell_h;

        if (e->collected) {
            DrawRectangle((int)x + 6, (int)y + 6,
                          (int)G.cell_w - 12, (int)G.cell_h - 12,
                          (Color){35, 78, 45, 220});
            DrawText("OK", (int)x + 28, (int)y + 25, 22, RAYWHITE);
            continue;
        }

        if (!evidence_unlocked(i)) {
            DrawRectangle((int)x + 6, (int)y + 6,
                          (int)G.cell_w - 12, (int)G.cell_h - 12,
                          (Color){48, 48, 48, 220});
            DrawText("LOCK", (int)x + 16, (int)y + 24, 16, GRAY);
            continue;
        }

        Color tile = e->major
                     ? (Color){105, 74, 22, 240}
                     : (Color){60, 62, 70, 240};

        DrawRectangle((int)x + 6, (int)y + 6,
                      (int)G.cell_w - 12, (int)G.cell_h - 12,
                      tile);

        DrawRectangleLines((int)x + 6, (int)y + 6,
                           (int)G.cell_w - 12, (int)G.cell_h - 12,
                           e->major ? GOLD : RAYWHITE);

        Rectangle clue_source = {0, 0, (float)clue_texture.width, (float)clue_texture.height};
        Rectangle clue_dest = {
            x + G.cell_w * 0.5f - 20.0f,
            y + 8.0f,
            40.0f,
            40.0f
        };
        DrawTexturePro(clue_texture, clue_source, clue_dest,
                       (Vector2){0, 0}, 0.0f, e->major ? GOLD : WHITE);

        DrawText(TextFormat("%d CR", e->cost),
                 (int)x + 19,
                 (int)y + 49,
                 14,
                 RAYWHITE);
    }
}

static void draw_units(void) {
    for (int i = 0; i < G.unit_count; i++) {
        if (G.units[i].active) draw_unit_shape(&G.units[i]);
    }
}

static void draw_enemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (G.enemies[i].active) draw_enemy_shape(&G.enemies[i]);
    }
}

static void draw_hud(void) {
    DrawRectangle(0, 0, SCREEN_W, 92, (Color){15, 15, 17, 255});

    DrawText("NOIR", 20, 14, 28, GOLD);

    DrawText(TextFormat("Credits: %d", G.credits), 275, 18, 20, RAYWHITE);
    DrawText(TextFormat("Kills: %d", G.kills), 415, 18, 20, RAYWHITE);
    DrawText(TextFormat("Rumors: %d/%d", G.rumor_count, TOTAL_RUMORS),
             525, 18, 20, RAYWHITE);
    DrawText(TextFormat("Evidence: %d/%d", G.evidence_count, TOTAL_EVIDENCE),
             680, 18, 20, GOLD);
    DrawText(TextFormat("Threat: %s", threat_name()), 850, 18, 20, RAYWHITE);

    DrawText("Archive:", 1020, 18, 20, RAYWHITE);
    for (int i = 0; i < ARCHIVE_MAX_HP; i++) {
        Color c = (i < G.archive_hp) ? GOLD : DARKGRAY;
        DrawRectangle(1110 + i * 35, 15, 25, 23, c);
    }

    DrawText("Kill 3 enemies = Rumor | Click clue tiles to buy evidence | Right-click cancels placement",
             20, 57, 16, LIGHTGRAY);

    if (G.message_timer > 0.0f) {
        int tw = MeasureText(G.message, 18);
        DrawRectangle(SCREEN_W / 2 - tw / 2 - 12, 96, tw + 24, 30,
                      (Color){0, 0, 0, 210});
        DrawText(G.message, SCREEN_W / 2 - tw / 2, 102, 18, GOLD);
    }
}

static void draw_bottom_bar(void) {
    DrawRectangle(0, SCREEN_H - 104, SCREEN_W, 104, (Color){15, 15, 17, 255});
    DrawLine(0, SCREEN_H - 104, SCREEN_W, SCREEN_H - 104, GRAY);

    Rectangle gr = guard_button_rect();
    Rectangle cr = cook_button_rect();
    Rectangle cf = case_button_rect();
    Rectangle ar = accuse_button_rect();

    Color gfill = (G.placing_unit && G.selected_unit == UNIT_GUARD)
                  ? GOLD : (Color){90, 78, 25, 255};
    Color cfill = (G.placing_unit && G.selected_unit == UNIT_COOK)
                  ? LIGHTGRAY : (Color){70, 70, 75, 255};

    DrawRectangleRec(gr, gfill);
    DrawRectangleLinesEx(gr, 2, RAYWHITE);
    DrawText("GUARD", (int)gr.x + 10, (int)gr.y + 8, 18, BLACK);
    DrawText("3 CR | ranged", (int)gr.x + 10, (int)gr.y + 33, 15, BLACK);

    DrawRectangleRec(cr, cfill);
    DrawRectangleLinesEx(cr, 2, RAYWHITE);
    DrawText("COOK", (int)cr.x + 10, (int)cr.y + 8, 18, BLACK);
    DrawText("2 CR | close", (int)cr.x + 10, (int)cr.y + 33, 15, BLACK);

    DrawText("Credits buy BOTH defense and evidence.", 360, SCREEN_H - 77, 18, RAYWHITE);
    DrawText("That trade-off is the main strategy.", 360, SCREEN_H - 50, 16, LIGHTGRAY);

    bool case_hover = point_in_rect(GetMousePosition(), cf);
    DrawRectangleRec(cf, case_hover ? (Color){85, 85, 90, 255} : (Color){60, 60, 65, 255});
    DrawRectangleLinesEx(cf, 2, RAYWHITE);
    DrawText("CASE FILE", (int)cf.x + 31, (int)cf.y + 19, 19, RAYWHITE);

    bool unlocked = G.evidence_count >= ACCUSE_REQUIRED;
    bool acc_hover = unlocked && point_in_rect(GetMousePosition(), ar);
    Color afill = !unlocked
                  ? (Color){45, 45, 45, 255}
                  : acc_hover
                    ? (Color){135, 45, 45, 255}
                    : (Color){95, 35, 35, 255};

    DrawRectangleRec(ar, afill);
    DrawRectangleLinesEx(ar, 2, unlocked ? GOLD : GRAY);

    if (unlocked) {
        DrawText("ACCUSE", (int)ar.x + 65, (int)ar.y + 8, 20, GOLD);
        DrawText("Unlocked", (int)ar.x + 67, (int)ar.y + 33, 14, RAYWHITE);
    } else {
        DrawText("ACCUSE", (int)ar.x + 65, (int)ar.y + 8, 20, GRAY);
        DrawText(TextFormat("Need %d/%d evidence", ACCUSE_REQUIRED, TOTAL_EVIDENCE),
                 (int)ar.x + 35, (int)ar.y + 33, 14, GRAY);
    }
}

static void draw_play(void) {
    ClearBackground((Color){20, 20, 22, 255});
    draw_hud();
    draw_grid();
    draw_evidence_tiles();
    draw_units();
    draw_enemies();
    draw_bottom_bar();

    if (G.placing_unit) {
        DrawText("PLACEMENT MODE - click a free grid cell",
                 20, SCREEN_H - 125, 16, GOLD);
    }
}

/* ------------------------------------------------------------
   PANELS
   ------------------------------------------------------------ */

static void draw_panel_background(const char *title) {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, (Color){0, 0, 0, 175});

    Rectangle box = {150, 95, 980, 610};
    DrawRectangleRec(box, (Color){28, 28, 31, 255});
    DrawRectangleLinesEx(box, 3, GOLD);

    DrawText(title, 185, 125, 30, GOLD);
    DrawText("Defense is paused while this screen is open.", 185, 168, 16, LIGHTGRAY);
}

static void draw_info_panel(void) {
    draw_panel_background(G.popup_title ? G.popup_title : "INFORMATION");

    draw_wrapped_text(G.popup_text ? G.popup_text : "",
                      195, 235, 880, 24, 10, RAYWHITE);

    Rectangle close = {470, 605, 340, 60};
    if (button(close, "CONTINUE", true)) {
        G.panel = PANEL_NONE;
    }
}

static void draw_casefile_panel(void) {
    draw_panel_background("CASE FILE");

    DrawText(TextFormat("Evidence Collected: %d/%d", G.evidence_count, TOTAL_EVIDENCE),
             185, 205, 22, GOLD);
    DrawText(TextFormat("Rumors Collected: %d/%d", G.rumor_count, TOTAL_RUMORS),
             485, 205, 22, RAYWHITE);

    DrawText("SUSPECTS", 185, 252, 22, GOLD);

    int sy = 285;
    for (int i = 0; i < TOTAL_SUSPECTS; i++) {
        DrawText(TextFormat("%d. %s - %s", i + 1, SUSPECTS[i].name, SUSPECTS[i].role),
                 195, sy, 18, RAYWHITE);
        draw_wrapped_text(SUSPECTS[i].note, 215, sy + 24, 400, 14, 4, LIGHTGRAY);
        sy += 73;
    }

    DrawText("COLLECTED EVIDENCE", 650, 252, 22, GOLD);

    int ey = 285;
    bool any_evidence = false;
    for (int i = 0; i < TOTAL_EVIDENCE; i++) {
        if (!EVIDENCE[i].collected) continue;
        any_evidence = true;

        DrawText(TextFormat("%s%s", EVIDENCE[i].major ? "[MAJOR] " : "", EVIDENCE[i].title),
                 660, ey, 16, EVIDENCE[i].major ? GOLD : RAYWHITE);
        ey += 26;
    }

    if (!any_evidence) {
        DrawText("No evidence collected yet.", 660, ey, 16, GRAY);
    }

    DrawText("RUMORS", 650, 480, 22, GOLD);
    if (G.rumor_count == 0) {
        DrawText("No rumors collected yet.", 660, 515, 16, GRAY);
    } else {
        int ry = 515;
        for (int i = 0; i < G.rumor_count; i++) {
            DrawText(TextFormat("Rumor %d", i + 1), 660, ry, 16, RAYWHITE);
            ry += 24;
        }
    }

    Rectangle close = {880, 620, 200, 52};
    if (button(close, "CLOSE", true)) {
        G.panel = PANEL_NONE;
    }
}

static void draw_accuse_panel(void) {
    draw_panel_background("FINAL ACCUSATION");

    DrawText("Who killed Mr. Blackwood?", 185, 215, 26, RAYWHITE);
    DrawText("Choose carefully. A wrong accusation ends the case.",
             185, 255, 18, LIGHTGRAY);

    for (int i = 0; i < TOTAL_SUSPECTS; i++) {
        Rectangle r = {
            260,
            315 + i * 68,
            760,
            50
        };

        char label[128];
        snprintf(label, sizeof(label), "%s - %s", SUSPECTS[i].name, SUSPECTS[i].role);

        if (button(r, label, true)) {
            if (i == 3) {
                G.state = STATE_WIN;
                G.panel = PANEL_NONE;
            } else {
                G.state = STATE_LOSE;
                G.panel = PANEL_NONE;
                G.wrong_accusation = true;
            }
        }
    }

    Rectangle cancel = {500, 610, 280, 50};
    if (button(cancel, "BACK TO CASE", true)) {
        G.panel = PANEL_NONE;
    }
}

/* ------------------------------------------------------------
   TITLE / END SCREENS
   ------------------------------------------------------------ */

static void draw_title(void) {
    ClearBackground((Color){18, 18, 20, 255});

    const char *title = "NOIR";
    int tw = MeasureText(title, 54);
    DrawText(title, SCREEN_W / 2 - tw / 2, 95, 54, GOLD);

    const char *subtitle = "Tower Defense + Murder Investigation";
    int sw = MeasureText(subtitle, 24);
    DrawText(subtitle, SCREEN_W / 2 - sw / 2, 165, 24, RAYWHITE);

    DrawRectangle(230, 235, 820, 330, (Color){32, 32, 35, 255});
    DrawRectangleLines(230, 235, 820, 330, GRAY);

    DrawText("THE IDEA", 265, 268, 25, GOLD);
    DrawText("1. Defend the Evidence Archive from attackers.", 265, 315, 20, RAYWHITE);
    DrawText("2. Every enemy kill gives +1 Credit.", 265, 350, 20, RAYWHITE);
    DrawText("3. Every 3 kills reveals one Rumor.", 265, 385, 20, RAYWHITE);
    DrawText("4. Spend Credits on defenders OR evidence tiles.", 265, 420, 20, RAYWHITE);
    DrawText("5. At 4/7 Evidence, ACCUSE becomes available.", 265, 455, 20, RAYWHITE);
    DrawText("6. Identify the murderer before the Archive is destroyed.", 265, 490, 20, RAYWHITE);

    const char *start = "PRESS ENTER OR CLICK TO START";
    int stw = MeasureText(start, 24);
    DrawText(start, SCREEN_W / 2 - stw / 2, 630, 24, GOLD);

    DrawText("No external art or assets required.", 20, SCREEN_H - 35, 16, GRAY);
}

static void draw_win(void) {
    ClearBackground((Color){17, 31, 20, 255});

    const char *title = "CASE SOLVED";
    int tw = MeasureText(title, 56);
    DrawText(title, SCREEN_W / 2 - tw / 2, 120, 56, GOLD);

    DrawText("Victor Crowe killed Mr. Blackwood.", 380, 230, 28, RAYWHITE);

    DrawRectangle(260, 295, 760, 230, (Color){28, 38, 30, 255});
    DrawRectangleLines(260, 295, 760, 230, GOLD);

    draw_wrapped_text(
        "The victim discovered Victor was stealing from the manor accounts. "
        "The torn letter arranged a 10 PM meeting. The broken watch and autopsy placed the death "
        "around 10:15. Arthur's ticket cleared the butler. Finally, Victor's broken cufflink was "
        "found beside fresh blood under the desk.",
        300, 330, 680, 20, 8, RAYWHITE);

    DrawText(TextFormat("Evidence collected: %d/%d | Kills: %d | Credits left: %d",
                        G.evidence_count, TOTAL_EVIDENCE, G.kills, G.credits),
             330, 560, 20, LIGHTGRAY);

    DrawText("Press R to play again", 500, 650, 22, GOLD);
}

static void draw_lose(void) {
    ClearBackground((Color){38, 18, 18, 255});

    const char *title = G.wrong_accusation ? "WRONG ACCUSATION" : "ARCHIVE DESTROYED";
    int tw = MeasureText(title, 50);
    DrawText(title, SCREEN_W / 2 - tw / 2, 140, 50, GOLD);

    if (G.wrong_accusation) {
        DrawText("The real killer escaped because the case was closed on the wrong suspect.",
                 260, 255, 22, RAYWHITE);
    } else {
        DrawText("The attackers destroyed the Evidence Archive before you solved the case.",
                 280, 255, 22, RAYWHITE);
    }

    DrawText(TextFormat("Evidence: %d/%d | Kills: %d | Credits: %d",
                        G.evidence_count, TOTAL_EVIDENCE, G.kills, G.credits),
             420, 330, 22, LIGHTGRAY);

    DrawText("Press R to try again", 505, 520, 22, GOLD);
}

/* ------------------------------------------------------------
   DRAW DISPATCH
   ------------------------------------------------------------ */

void draw_game(void) {
    if (G.state == STATE_TITLE) {
        draw_title();
        return;
    }

    if (G.state == STATE_WIN) {
        draw_win();
        return;
    }

    if (G.state == STATE_LOSE) {
        draw_lose();
        return;
    }

    draw_play();

    if (G.panel == PANEL_INFO) draw_info_panel();
    else if (G.panel == PANEL_CASEFILE) draw_casefile_panel();
    else if (G.panel == PANEL_ACCUSE) draw_accuse_panel();
}

/* ------------------------------------------------------------
   MAIN
   ------------------------------------------------------------ */
