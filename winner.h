#ifndef WINNER_H
#define WINNER_H

#include <SDL/SDL.h>
#include "define.h"
#include "game_state.h"

void winner_init();

void winner_handle_input();

void winner_update();

bool winner_render(SDL_Surface *screen);

void winner_free();

game_state_t* set_state_winner(); 
#endif
