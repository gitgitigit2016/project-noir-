#include "enemy_manager.h"

#include "game_state.h"
#include "utils.h"

#include "raylib.h"
#include <math.h>



void update_enemies(float dt)
{
    float safe_x =
        G.grid_ox +
        G.cell_w * 0.5f;



    for(int i = 0; i < MAX_ENEMIES; i++)
    {

        Enemy *e = &G.enemies[i];


        if(!e->active)
            continue;



        e->anim_timer += dt;



        float target_y =
            G.grid_oy +
            e->lane * G.cell_h +
            G.cell_h * 0.5f;



        e->y +=
            (target_y - e->y)
            * 6.f
            * dt;



        e->x -=
            e->speed *
            dt;



        if(e->x < safe_x)
        {

            e->active = false;


            G.safe_hp--;



            spawn_particles(
                safe_x,
                e->y,
                RED,
                10
            );



            set_message(
                "The safe was breached!",
                2.f
            );



            if(G.safe_hp <= 0)
            {
                G.phase = PHASE_GAMEOVER;
                return;
            }

        }



        if(e->hp <= 0.f)
        {

            e->active = false;


            G.gold++;



            spawn_particles(
                e->x,
                e->y,
                (Color){255,220,50,255},
                6
            );

        }

    }

}