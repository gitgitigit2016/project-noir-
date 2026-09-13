#ifndef UPDATE_H
#define UPDATE_H
#include "globals.h"

void update_game(void);
void update_units(float dt);
void update_enemies(float dt);
void update_play_input(void);
void collect_evidence(int id);
bool place_unit(UnitType type, int col, int row);

#endif
