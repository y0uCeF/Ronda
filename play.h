#ifndef PLAY_H
#define PLAY_H

#include "player.h"
/* functions definition */

bool valid_move(player p, card table[]);

/*
 * affect changes on cards after user's turn 
 */ 
void user_turn(player *p, card table[]);

/*
 * computer turn with automatic card choosing
 */ 
void computer_turn(player *p, card table[]);

#endif
