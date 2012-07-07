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
	unsigned short running;  /* main loop condition */
	SDL_Surface *screen;	
	SDL_Event event;
	player *user;    
	player *comp;
	card table[MAX_NB_CARDS_TABLE];
	card_num card_list[NB_CARDS];
	short sel_table;
	short sel_hand;
	unsigned short nb_cards_remaining;
	type_t current_player;
	card_num dropped_card;  /* last card dropped */
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
