#ifndef BASIC_H
#define BASIC_H

#include "define.h"

void swap(card_num *a, card_num *b);
shortint equal(int a, int b);
short exist(card tab[], int size, card_num value);
shortint empty(card tab[], shortint size);
shortint nbCards(card tab[], shortint size);
void getFile(card_num n, char* file);
shortint between(shortint x, shortint a, shortint b);

#endif
