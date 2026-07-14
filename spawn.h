#ifndef SPAWN_H
#define SPAWN_H

/* =========================================================
   spawn.h
   -------
   Declarations for placing units on the grid and
   spawning enemy waves.
   ========================================================= */

#include "globals.h"
#include "utils.h"

bool place_unit(UnitType type, int col, int row);
void spawn_wave(int wave_num);
void spawn_enemy(int lane);

#endif