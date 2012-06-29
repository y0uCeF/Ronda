#ifndef PLAY_H
#define PLAY_H

#include "player.h"
short getSelectedHand(int x);
short getSelectedTable(int x, int y);
void play(player *p, card table[], short selectedHand, short selectedTable, card_num *droppedCard);

#endif
