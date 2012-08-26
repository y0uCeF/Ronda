#ifndef GAME_H
#define GAME_H

#include "define.h"
#include "game_state.h"
/* Constants definition */
	#define WINDOW_WIDTH 800
	#define WINDOW_HEIGHT 600
	#define FPS 30
	#define FRAME_RATE 1000/FPS
    

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
 * freeing the memory of the current state
 */ 
void game_free();

#endif
