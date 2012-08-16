#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "common.h"
#include "play.h"


static bool env_init(game_t *g) 
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		return sdl_error("Unable to init SDL");
	putenv("SDL_VIDEO_CENTERED=1");
	g->screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  
				SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (g->screen == NULL) 
		return sdl_error("Video initialization failed");
	if (SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68)) == -1) 
		return sdl_error("Setting screen failed");
		
	SDL_WM_SetCaption("Ronda - alpha", NULL);
	return 1;
}

game_t* game_init() 
{
	game_t *g = malloc(sizeof(game_t));
	if (!env_init(g)) 
		return NULL;
	
	g->running = 1;
	g->state = game_state_init();
	
	return g;
}	

static bool game_render(game_t *g) 
{
	SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68));
	
	return game_state_render(g->state, g->screen);	
}

void game_free(game_t *g) 
{
	game_state_free(g->state);
	free(g);
	
	SDL_Quit();
}

static void treat_keyboard_event(SDL_Event event, unsigned short *running)
{
	switch(event.key.keysym.sym) {
		case SDLK_ESCAPE:
			*running = 0;
		break;
			
		case SDLK_RETURN:
			*running = 0;
		break;
					
		default:
			
		break;
	}	
}

static void treat_mouse_down_event(game_t *g)
{
	switch(g->event.button.button) {
	case SDL_BUTTON_LEFT:
		game_state_handle_input(g->event, g->state);
	break;
						
	default:
	break;
	}
}

static void game_handle_input(game_t *g)
{
	if (g->event.type == SDL_QUIT) 
		g->running = 0;
	else if (g->event.type == SDL_KEYDOWN)
		treat_keyboard_event(g->event, &g->running);
	else if (g->event.type == SDL_MOUSEBUTTONUP)
		treat_mouse_down_event(g);
}

void game_update(game_t *g)
{
	if (g->state->current_player == USER)
		game_state_process_actions(g->state);
	else	
		game_state_computer_turn(g->state);	
}

bool game_run(game_t *g) 
{
	int currentTime = 0, oldTime = 0,
		timeDiff; /* time difference calculations */
	
	/* main loop */
	while (g->running) {     
		while(SDL_PollEvent(&g->event))	{
			game_handle_input(g);		
		}
		
		/*setting fps to 30 */
		currentTime = SDL_GetTicks();
		if ((timeDiff = currentTime - oldTime) > FRAME_RATE ) 
			oldTime = currentTime;
		else 
			SDL_Delay(FRAME_RATE - timeDiff);
		
		
		game_update(g);
		
		
		/*rendering*/
		if (!game_render(g)) 
			return 0;	
	}
					
	return 1; 		
}
