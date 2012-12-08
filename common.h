#ifndef COMMON_H
#define COMMON_H

#include "define.h"


/*
 * checks if  a <= x <= b
 */
inline bool between(unsigned short x, unsigned short a, unsigned short b);

/*
 * checks of a == b
 */ 
inline bool equal(int a, int b);

inline int rand_a_b(int a, int b);

/*
 * mixes cards in array
 */ 
void mix(card_num array[], unsigned short size);

/*
 * swaps contents of a and b
 */ 

void swap(card_num *a, card_num *b);

/*
 * checks if value exists in tab
 */ 
short exist(card tab[], unsigned short size, card_num value);

/*
 * checks if tab is empty
 */ 
bool empty(card tab[], unsigned short size);

/*
 * counts cards in tab
 */ 
unsigned short nb_cards(card tab[], unsigned short size);

/*
 * returns path to file representing the card n
 */  
char* get_file(card_num n);

/*
 * writes error messages in stderr
 */ 
bool sdl_error();

/*
 * sets card attributes given in parameters
 */ 
bool set_card(card *c, card_num num, short posx, short posy, bool back);

/*
 * checks if max_frames has passed
 */
bool passed(short max_frames, int *nb_frames);  

/*
 * returns text surface
 */
SDL_Surface* set_text_surf(char *font_name, int size, char* text, short r,
                                short g, short b);
#endif
