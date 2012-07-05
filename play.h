#ifndef PLAY_H
#define PLAY_H

#include "player.h"
short get_selected_hand(int x);
short get_selected_table(int x, int y);
void user_turn(player *p, card table[], short selectedHand, short selectedTable, card_num *droppedCard);

#endif
