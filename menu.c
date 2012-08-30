#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "menu.h"
#include "game_state.h"
#include "define.h"

#define FONT_SIZE 20
#define FONT_NAME "georgiai.ttf"


static SDL_Surface *question;
static SDL_Surface *stay;
static SDL_Surface *leave;
static SDL_Rect question_pos;
static SDL_Rect stay_pos;
static SDL_Rect leave_pos;

extern stack s;

static void set_text_surf(SDL_Surface **surf, char* text)
{
	TTF_Font* font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
	SDL_Color foreground = {255, 255, 255};
	SDL_Color background = {0, 0, 0};
	
	*surf = TTF_RenderText_Shaded(font, text, foreground, background);
	if(*surf == NULL) printf("surf is the problem\n");
	TTF_CloseFont(font);	
}

void menu_init()
{
	TTF_Init();
	
	set_text_surf(&question, "What are you willing to do?");
	set_text_surf(&stay, "(S)tay.");
	set_text_surf(&leave, "(L)eave.");
	
	if(question == NULL) printf("question is the problem\n");
	question_pos.x = 230;
	question_pos.y = 170;
	stay_pos.x = 320;
	stay_pos.y = 210;
	leave_pos.x = 320;
	leave_pos.y = 240;
}

void menu_handle_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			while(!stack_empty(s)) {
				top(s).free();
				pop(&s);
			}
		break;	
		
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
			case SDLK_l:
				while(!stack_empty(s)) {
				top(s).free();
				pop(&s);
				}
			break;
			
			case SDLK_s:
				top(s).free();
				pop(&s);
			break;	
			
			case SDLK_ESCAPE:
				top(s).free();
				pop(&s);
			break;	
			
			default:
			break;
			}	
		break;
		
		default:
		break;
		}
	}		
}

void menu_update()
{
	
}

bool menu_render(SDL_Surface *screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	if(SDL_BlitSurface(question, NULL, screen, &question_pos) == -1)
		return 0;
	if(SDL_BlitSurface(stay, NULL, screen, &stay_pos) == -1)
		return 0;
	if(SDL_BlitSurface(leave, NULL, screen, &leave_pos) == -1)
		return 0;
	
	SDL_Flip(screen);
	
	return 1;
}

void menu_free()
{
	SDL_FreeSurface(question);
	SDL_FreeSurface(stay);
	SDL_FreeSurface(leave);
	
	TTF_Quit();
}

game_state_t* set_state_menu()
{
	game_state_t *gs = malloc(sizeof(game_state_t));
	gs->init = menu_init;
	gs->handle_input = menu_handle_input;
	gs->update = menu_update;
	gs->render = menu_render;
	gs->free = menu_free;
	
	return gs;
}
