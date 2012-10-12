#ifndef GAME_H
#define GAME_H

#include "define.h"
#include "game_state.h"

/* functions definition */

/* 
 * Initializes the game environnement 
 */
void game_init();

/*
 * handles current state's input
 */ 
void game_handle_input();

/*
 * updates the game
 */ 
void game_update();

/*
 * renders the current state
 */ 
bool game_render();

/*
 * pop all the states from the stack 
 */
void game_exit();
  
/*
 * freeing the memory of the current state
 */ 
void game_free();

#endif
