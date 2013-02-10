#include <stdlib.h>

#include "game_state.h"


bool stack_empty(stack s)
{
	return (s == NULL);
}

void push(stack *s, game_state_t g_state)
{
	stack tmp = try_malloc(sizeof(elm));
	tmp->state = g_state;
	tmp->prev = *s;
	*s = tmp;
}

void pop(stack *s)
{
	stack tmp = *s;
	*s = (*s)->prev;
	try_free(tmp);
}

game_state_t top(stack s)
{
	return s->state;
}
