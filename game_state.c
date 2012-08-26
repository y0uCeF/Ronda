#include <stdlib.h>

#include "game_state.h"


short stack_empty(stack s)
{
	return (s == NULL);
}

void push(stack *s, game_state_t g_state)
{
	stack tmp = malloc(sizeof(elm));
	tmp->state = g_state;
	tmp->prev = *s;
	*s = tmp;
}

void pop(stack *s)
{
	stack tmp = *s;
	*s = (*s)->prev;
	free(tmp);
}

game_state_t top(stack s)
{
	return s->state;
}
