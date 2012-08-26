#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL/SDL.h>
#include "define.h"
typedef struct {
	void (*init)();
	void (*handle_input)();
	void (*update)();
	bool (*render)(SDL_Surface *screen);
	void (*free)();
} game_state_t;	

typedef struct elm {
	game_state_t state;
	struct elm *prev;
} elm;
typedef elm* stack;

short stack_empty(stack s);

void push(stack *s, game_state_t g_s);

void pop(stack *s);

game_state_t top(stack s);

#endif
