#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string.h>
#include "define.h"

void swap(card_num *a, card_num *b);
unsigned short int equal(int a, int b);
unsigned short int exist(card tab[], int size, int value);
void getFile(card_num n, char* file);
void init(card table[],player *p1, player *p2);
void distributeTable(card_num cardList[],card table[], unsigned short int* nbCardsRemaining);
void distributePlayer(card_num cardList[],player *pl, unsigned short int* nbCardsRemaining);


#endif
