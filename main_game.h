#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SDL/SDL.h>
#include "game_state.h"
#include "player.h"
#include "define.h"


extern score_t user_score;
extern score_t computer_score;

/* functions definition */

/* 
 * Initializes the game state environnement 
 */
void main_game_init();

/*
 * handles user input
 */ 
void main_game_handle_input();

/*
 * updates game information
 */ 
void main_game_update();

/*
 * draws everything on the screen
 */ 
bool main_game_render(SDL_Surface *screen);

/*
 * frees memory held by the game state
 */ 
void main_game_free();

/*
 * sets the main game as the current state
 */ 
game_state_t* set_state_main_game();
#endif
