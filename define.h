#ifndef DEFINE_H
#define DEFINE_H

#include <SDL/SDL.h>

/* 
 * some constants definitions
 */
	#define NB_CARDS 40
	#define NB_CARDS_TABLE 4
	#define MAX_NB_CARDS_TABLE 10
	#define MAX_NB_CARDS_HAND 3
    
/*
 * Definition of common data structures 
 */  

typedef enum { EMPTY=-1,
	COIN1, COIN2, COIN3, COIN4, COIN5, COIN6, COIN7, COIN10, COIN11, COIN12,
		CLUB1, CLUB2, CLUB3, CLUB4, CLUB5, CLUB6, CLUB7, CLUB10, CLUB11, CLUB12,
		SWORD1, SWORD2, SWORD3, SWORD4, SWORD5, SWORD6, SWORD7, SWORD10, SWORD11, SWORD12,
		CUP1, CUP2, CUP3, CUP4, CUP5, CUP6, CUP7, CUP10, CUP11, CUP12
} card_num;
      

typedef struct {
	SDL_Surface* surf;  /* sprite of the card */
	SDL_Rect* position; /* position on the screen */
	card_num value; /* value of the card */
} card;      

typedef unsigned short bool;  /* boolean */

#endif
