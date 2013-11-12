#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include "define.h"
#include "common.h"

/* game_state data structure */
typedef struct {
	void (*init)();
	void (*handle_input)();
	void (*update)();
	void (*render)(SDL_Renderer *renderer);
	void (*free)();
} game_state_t;

/* game_state stack structure */
typedef struct elm {
	game_state_t state;
	struct elm *prev;
} elm;
typedef elm* stack;

/* stack to hold game states */
extern stack s;

/*
 * is the stack "s" empty?
 */ 
bool stack_empty(stack s);

/*
 * pushes "g_s" to the stack
 */ 
void push(stack *s, game_state_t g_s);

/*
 * pops from the stack
 */ 
void pop(stack *s);

/*
 * returns the top element
 */ 
game_state_t top(stack s);

#endif
