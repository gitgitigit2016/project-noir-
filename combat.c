#include "combat.h"

#include "game_state.h"
#include "utils.h"

#include "raylib.h"
#include <math.h>



void update_combat(float dt)
{

    for(int i = 0; i < G.unit_count; i++)
    {

        Unit *u = &G.units[i];


        if(!u->active)
            continue;



        u->attack_timer -= dt;



        if(u->attack_timer > 0.f)
            continue;



        float range;

        int damage;

        float attack_speed;



        if(u->type == UNIT_GUARD)
        {
            range = G.cell_w * 3.5f;
            damage = 2;
            attack_speed = 1.1f;
        }
        else
        {
            range = G.cell_w * 1.8f;
            damage = 3;
            attack_speed = 0.75f;
        }



        float closest_distance = 1000000.f;

        int target_index = -1;



        for(int j = 0; j < MAX_ENEMIES; j++)
        {

            Enemy *e = &G.enemies[j];


            if(!e->active)
                continue;



            if(u->type == UNIT_GUARD &&
               e->lane != u->row)
            {
                continue;
            }



            float dx =
                e->x - u->x;


            float dy =
                e->y - u->y;



            float distance =
                sqrtf(
                    dx * dx +
                    dy * dy
                );



            if(distance < range &&
               distance < closest_distance)
            {
                closest_distance = distance;
                target_index = j;
            }

        }



        if(target_index >= 0)
        {

            G.enemies[target_index].hp -= damage;


            u->attack_timer =
                attack_speed;



            if(u->type == UNIT_COOK)
            {

                for(int j = 0; j < MAX_ENEMIES; j++)
                {

                    if(j == target_index)
                        continue;


                    Enemy *other =
                        &G.enemies[j];


                    if(!other->active)
                        continue;



                    float dx =
                        other->x - u->x;


                    float dy =
                        other->y - u->y;



                    float distance =
                        sqrtf(
                            dx * dx +
                            dy * dy
                        );



                    if(distance < range)
                    {
                        other->hp -= damage * 0.5f;
                    }

                }

            }



            Color particle_color;


            if(u->type == UNIT_GUARD)
            {
                particle_color =
                    (Color){255,255,100,255};
            }
            else
            {
                particle_color =
                    (Color){255,140,50,255};
            }



            spawn_particles(
                G.enemies[target_index].x,
                G.enemies[target_index].y,
                particle_color,
                3
            );

        }

    }

}