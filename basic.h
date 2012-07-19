#ifndef BASIC_H
#define BASIC_H

#include "define.h"

inline int rand_a_b(int a, int b);
bool between(unsigned short x, unsigned short a, unsigned short b);
bool equal(int a, int b);
void mix(card_num array[], int size);
void swap(card_num *a, card_num *b);
short exist(card tab[], int size, card_num value);
bool empty(card tab[], unsigned short size);
unsigned short nb_cards(card tab[], unsigned short size);
char* get_file(card_num n);
bool sdl_error();
bool set_card(card *c, card_num num, short posx, short posy, bool back);
#endif
