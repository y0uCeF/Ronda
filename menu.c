#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "menu.h"
#include "game_state.h"
#include "main_game.h"
#include "define.h"

#define SELECTOR_YES_X 225
#define SELECTOR_YES_Y 270
#define SELECTOR_NO_X 225
#define SELECTOR_NO_Y 310

static SDL_Surface *menu = NULL;
static SDL_Surface *selector = NULL;
static SDL_Rect selector_pos;
static enum {NO, YES} entry;
static bool call_menu ;

extern stack s;

void menu_init()
{
	menu = IMG_Load("menu.png");
        selector = IMG_Load("selector.png");
        
        call_menu = 0;
        entry = YES;
        selector_pos.x = SELECTOR_YES_X;
        selector_pos.y = SELECTOR_YES_Y;
}

void menu_handle_input()
{
	SDL_Event event;
        
        
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
                        top(s).free();
			pop(&s);
		break;	
		
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
                        
                        case SDLK_UP:
                                entry = YES;
                        break;
                        
                        case SDLK_DOWN:
                                entry = NO;
                        break;
                        
                        case SDLK_RETURN:
                              	if (entry == YES) {
                                        top(s).free();
                                        pop(&s);
                                } else {
                                        call_menu = 1;
                                }
			break;
                                
			case SDLK_ESCAPE:
                                call_menu = 1;
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
        if (call_menu) {
                game_state_t *tmp = set_state_main_game();
                push(&s, *tmp);
        }
        
	if (entry == NO) {
                selector_pos.x = SELECTOR_NO_X;
                selector_pos.y = SELECTOR_NO_Y;
        } else {
                selector_pos.x = SELECTOR_YES_X;
                selector_pos.y = SELECTOR_YES_Y;
        }
}

bool menu_render(SDL_Surface *screen)
{
	if(SDL_BlitSurface(menu, NULL, screen, NULL) == -1)
		return 0;
	if(SDL_BlitSurface(selector, NULL, screen, &selector_pos) == -1)
		return 0;
	
	SDL_Flip(screen);
	
	return 1;
}

void menu_free()
{
	SDL_FreeSurface(menu);
	SDL_FreeSurface(selector);
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
