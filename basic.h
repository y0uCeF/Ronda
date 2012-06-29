#ifndef BASIC_H
#define BASIC_H

#include "define.h"

void mix(card_num array[], int size);
void swap(card_num *a, card_num *b);
bool equal(int a, int b);
short exist(card tab[], int size, card_num value);
bool empty(card tab[], shortint size);
shortint nbCards(card tab[], shortint size);
void getFile(card_num n, char* file);
bool between(shortint x, shortint a, shortint b);

#endif
