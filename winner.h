#ifndef WINNER_H
#define WINNER_H

#include <SDL/SDL.h>
#include "define.h"
#include "game_state.h"

/* 
 * Initializes the winner state environnement 
 */
void winner_init();

/*
 * handles user input
 */ 
void winner_handle_input();

/*
 * updates state data
 */ 
void winner_update();

/*
 * rendering on the screen
 */ 
void winner_render(SDL_Surface *screen);

/*
 * frees memory held by the game state
 */ 
void winner_free();

/*
 * sets the winner state as the current state
 */ 
game_state_t* set_state_winner(); 
#endif
