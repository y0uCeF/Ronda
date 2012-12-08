#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "winner.h"
#include "common.h"
#include "game_state.h"
#include "game.h"
#include "main_game.h"
#include "define.h"

#define SELECTOR_NEW_GAME_X 220
#define SELECTOR_NEW_GAME_Y 270
#define SELECTOR_EXIT_GAME_X 220
#define SELECTOR_EXIT_GAME_Y 320
#define WINNER_MSG_Y 90


static SDL_Surface *winner_surf = NULL;
static SDL_Surface *selector = NULL;
static SDL_Rect selector_pos;
static enum {NEW_GAME, EXIT_GAME} entry;
static bool new_game ;

extern stack s;
extern type_t winner;

void winner_init()
{
	winner_surf = IMG_Load("data/winner.png");
        selector = IMG_Load("data/selector.png");
        
        TTF_Init();
        new_game = 0;
        entry = NEW_GAME;
        selector_pos.x = SELECTOR_NEW_GAME_X;
        selector_pos.y = SELECTOR_NEW_GAME_Y;
}

void winner_handle_input()
{
	SDL_Event event;
        
        
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
                        game_exit();
		break;	
		
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
                        
                        case SDLK_UP:
                                entry = NEW_GAME;
                        break;
                        
                        case SDLK_DOWN:
                                entry = EXIT_GAME;
                        break;
                        
                        case SDLK_RETURN:
                              	if (entry == EXIT_GAME)
                                        game_exit();
                                else
                                        new_game = 1;
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

void winner_update()
{
        if (new_game) {
		/*remove winner state*/
                top(s).free(); 
                pop(&s);
		/*re-init main_game state*/
                top(s).free();
                top(s).init();
        }
        
	if (entry == EXIT_GAME) {
                selector_pos.x = SELECTOR_EXIT_GAME_X;
                selector_pos.y = SELECTOR_EXIT_GAME_Y;
        } else {
                selector_pos.x = SELECTOR_NEW_GAME_X;
                selector_pos.y = SELECTOR_NEW_GAME_Y;
        }
}

void show_winner_msg(SDL_Surface *scr)
{
	char buf[27] = "";
	sprintf(buf, (winner == USER)? "Congratulations, You Win" : "Sorry You Loose");
	SDL_Surface *surf = set_text_surf("data/urw-bookman-l.ttf", 36, buf, 255, 255, 255);
	SDL_Rect pos = {MIDDLE_X(surf->w), WINNER_MSG_Y};
	SDL_BlitSurface(surf, NULL, scr, &pos);
        SDL_FreeSurface(surf);
}

bool winner_render(SDL_Surface *screen)
{
	if(SDL_BlitSurface(winner_surf, NULL, screen, NULL) == -1)
		return 0;
	if(SDL_BlitSurface(selector, NULL, screen, &selector_pos) == -1)
		return 0;
	
	show_winner_msg(screen);
	
	SDL_Flip(screen);
	
	return 1;
}

void winner_free()
{
	TTF_Quit();
	SDL_FreeSurface(winner_surf);
	SDL_FreeSurface(selector);
}

game_state_t* set_state_winner()
{
	game_state_t *gs = malloc(sizeof(game_state_t));
	gs->init = winner_init;
	gs->handle_input = winner_handle_input;
	gs->update = winner_update;
	gs->render = winner_render;
	gs->free = winner_free;
	
	return gs;
}
