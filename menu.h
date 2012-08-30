#ifndef MENU_H
#define MENU_H

#include <SDL/SDL.h>
#include "define.h"
#include "game_state.h"

void menu_init();

void menu_handle_input();

void menu_update();

bool menu_render(SDL_Surface *screen);

void menu_free();

game_state_t* set_state_menu(); 
#endif
