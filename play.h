#ifndef PLAY_H
#define PLAY_H

#include "player.h"
/* functions definition */

/*
 * returns index of the selected card in the screen (user hand)
 */ 
short get_selected_hand(int x);

/*
 * returns index of the selected card in the screen (table)
 */ 

short get_selected_table(int x, int y);

/*
 * affect changes on cards after user's turn 
 */ 
void user_turn(player *p, card table[], short sel_hand, short sel_table, 
		card_num *dropped_card);

#endif
