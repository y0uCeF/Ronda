#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string.h>
#include "define.h"

void swap(card_num *a, card_num *b);
shortint equal(shortint a, shortint b);
shortint exist(card tab[], int size, int value);
void getFile(card_num n, char* file);
void init(card table[],player *p1, player *p2);
void distributeTable(card_num cardList[],card table[]);
void distributePlayer(card_num cardList[],player *pl);


#endif
