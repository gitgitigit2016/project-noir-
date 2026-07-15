#include "init.h"
#include "update.h"
#include "draw.h"

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "NOIR - Victorian Manor Mystery (Progress Report 1)");
    SetTargetFPS(FPS);
    init_game();

    while (!WindowShouldClose()) {

        if (G.phase == PHASE_MAP) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mp = GetMousePosition();
                if (mp.x>440 && mp.x<840 && mp.y>260 && mp.y<370) {
                    init_kitchen();
                    G.phase = PHASE_DEFENSE;
                }
            }
        } else if (G.phase == PHASE_DEFENSE) {
            update_defense();
        } else if (G.phase == PHASE_GAMEOVER || G.phase == PHASE_WIN) {
            if (IsKeyPressed(KEY_R)) init_game();
        }
        BeginDrawing();
        ClearBackground((Color){12,10,8,255});

        if      (G.phase == PHASE_MAP)      draw_map();
        else if (G.phase == PHASE_DEFENSE) {
            draw_grid();
            draw_clue_tiles();
            draw_units_grid();
            draw_enemies_grid();
            draw_hud();
            draw_shop();
        }
        else if (G.phase == PHASE_GAMEOVER) draw_gameover();
        else if (G.phase == PHASE_WIN)      draw_win();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
