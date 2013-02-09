#ifndef COMMON_H
#define COMMON_H

#include "define.h"


/*
 * some useful macros
 */ 	

#define rand_a_b(a, b) (rand() % ((b) - (a)) + (a))
#define between(x, a, b) (((x) >= (a)) && ((x) <= (b)))
#define equal(a, b) (((a) % 10) == ((b) % 10))


extern SDL_Surface *back_card;// = NULL;
extern SDL_Surface *empty_card;// = NULL;

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

/*
 * displays text "txt" on the screen, position specified by posx, posy
 */
void show_text(char *font, short size, char *txt, short posx, short posy, 
			short r, short g, short b, SDL_Surface *scr); 

/*
 * displays text set in "txt" in white color using show_text()
 */ 
inline void show_white_text(char *font, short size, char *txt, short posx, short posy, 
		SDL_Surface *scr);
#endif
