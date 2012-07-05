#ifndef GAME_H
#define GAME_H

#include "player.h"

/* Constants definition */
	#define WINDOW_WIDTH 800
	#define WINDOW_HEIGHT 600
	#define FPS 30
	#define FRAME_RATE 1000/FPS
    

/* types definition */    
typedef struct {
	unsigned short running;
	SDL_Surface *screen;	
	SDL_Event event;
	player *user;
	player *comp;
	card table[MAX_NB_CARDS_TABLE];
	card_num cardList[NB_CARDS];
	short selectedTable;
	short selectedHand;
	unsigned short nb_cards_remaining;
	type_t currentPlayer;
	card_num droppedCard;
} game_t;

/* functions definition */


/* 
 * Initializes the game environnement 
 */
game_t* game_init();

/*
 * the main loop 
 */ 
bool game_run(game_t *g);

/*
 * freeing the memory
 */ 
void game_free(game_t *g);

#endif
