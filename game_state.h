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
} game_state_t;


/* functions definition */

/* 
 * Initializes the game state environnement 
 */
game_state_t* game_state_init();

/*
 * handles user input
 */ 
void game_state_handle_input(SDL_Event event, game_state_t *s);

/*
 * processes actions specified by user input 
 */  
void game_state_process_actions(game_state_t *s);

/*
 * moves the turn to the computer
 */ 
void game_state_computer_turn(game_state_t *s);

/*
 * draws everything on the screen
 */ 
bool game_state_render(game_state_t *s, SDL_Surface *screen);

/*
 * frees memory held by the game state
 */ 
void game_state_free(game_state_t *s);


#endif
