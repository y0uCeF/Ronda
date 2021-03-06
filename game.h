#ifndef GAME_H
#define GAME_H

#include "define.h"
#include "game_state.h"

/* Constants */
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MIDDLE_X(w) WINDOW_WIDTH/2 - w/2

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
void game_render();

/*
 * pop all the states from the stack and exit
 */
void game_exit(int status);
  
/*
 * freeing the memory of the current state
 */ 
void game_free();

#endif
