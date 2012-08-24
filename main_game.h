#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL/SDL.h>
#include "player.h"
#include "define.h"

typedef struct {
	player* user;    
	player* comp;
	card table[MAX_NB_CARDS_TABLE];
	card_num card_list[NB_CARDS];
	unsigned short nb_cards_remaining;
	type_t current_player;
	card_num dropped_card;  /* last card dropped */
} main_game_t;


/* functions definition */

/* 
 * Initializes the game state environnement 
 */
main_game_t* main_game_init();

/*
 * handles user input
 */ 
void main_game_handle_input(SDL_Event event, main_game_t *s);

/*
 * processes actions specified by user input 
 */  
void main_game_process_actions(main_game_t *s);

/*
 * moves the turn to the computer
 */ 
void main_game_computer_turn(main_game_t *s);

/*
 * draws everything on the screen
 */ 
bool main_game_render(main_game_t *s, SDL_Surface *screen);

/*
 * frees memory held by the game state
 */ 
void main_game_free(main_game_t *s);


#endif
