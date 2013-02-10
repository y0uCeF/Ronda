#include <SDL/SDL.h>

#include "controller.h"
#include "common.h"


static inline void set_default_values(controller_data *c_data)
{
	c_data->exit = 0;
	c_data->selected_card_hand = -1;
	c_data->selected_card_table = -1;
}

controller_data* controller_data_init()
{
	controller_data *c_data = try_malloc(sizeof(controller_data));

	set_default_values(c_data);

	return c_data;
}

static bool valid_card_hand(int x, int y)
{
	bool res = 0;
	if (between(y, 450, 585))
		res = between(x,150, 240) || between(x, 280,370) ||
				between(x,410, 500);
	return res;
}

static bool valid_card_table(int x, int y)
{
	bool res = 0;
	if (between(y, 160, 295) || between(y, 305, 440))
		res = between(x, 40, 130) || between(x, 145, 235) ||
		between(x, 250, 340) || between(x, 355, 445) ||
		between(x, 460, 550); 
	return res;
}

/*
 * returns index of the selected card in the screen (user hand)
 */ 
static short get_selected_hand(int x) 
{
	if (between(x,150, 240)) 
		return 0;
	else if (between(x, 280,370)) 
		return 1;
	else if (between(x,410, 500)) 
		return 2;
	else 
		return -1;
}

/*
 * returns index of the selected card in the screen (table)
 */ 
static short get_selected_table(int x, int y) 
{
	short temp=-1, c=0;
	if (between(x, 40, 130)) 
		temp = 0;
	else if (between(x, 145, 235)) 
		temp = 1;
	else if (between(x, 250, 340)) 
		temp = 2;
	else if (between(x, 355, 445)) 
		temp = 3;
	else if (between(x, 460, 550)) 
		temp = 4;

	if (temp != -1) { 
		if (between(y, 160, 295))
			c = temp;
		else if (between(y, 305, 440)) 
			c = temp+5;
		else 
			c = -1;
		return c;
	}
	else return -1;
}

static void treat_mouse_click_event(SDL_Event event, controller_data* c_data)
{
	int x = event.button.x;
	int y = event.button.y;

	switch (event.button.button) {
	case SDL_BUTTON_LEFT:
		if (valid_card_hand(x, y)) 
			c_data->selected_card_hand = get_selected_hand(x);
		else if (valid_card_table(x, y))
			c_data->selected_card_table = get_selected_table(x, y);

	break;

	default:
	break;
	}
}

void controller_data_update(controller_data *c_data)
{
	SDL_Event event;

	set_default_values(c_data);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			c_data->exit = 1;
		else if (event.type == SDL_MOUSEBUTTONUP)
			treat_mouse_click_event(event, c_data);
	}
}
