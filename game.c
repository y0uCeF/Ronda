#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "main_game.h"
#include "menu.h"
#include "game_state.h"
#include "common.h"


/* Constants definition */
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 30
#define FRAME_RATE 1000/FPS
        
static SDL_Surface *screen = NULL;
stack s;
static int oldTime = 0;

static bool env_init() 
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		return sdl_error("Unable to init SDL");
	putenv("SDL_VIDEO_CENTERED=1");
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  
				SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (screen == NULL) 
		return sdl_error("Video initialization failed");
	if (SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68)) == -1) 
		return sdl_error("Setting screen failed");
		
	SDL_WM_SetCaption("Ronda - alpha", NULL);
	return 1;
}

void game_init() 
{
	s = NULL;
        game_state_t *tmp = set_state_menu();
	push(&s, *tmp);
        free(tmp);
	tmp = set_state_main_game();
	push(&s, *tmp);
	if (!env_init()) 
		return;
	
	top(s).init();
        free(tmp);
}	

void game_handle_input()
{
	top(s).handle_input();
}

void game_update()
{
	int timeDiff; /* time difference calculations */	
	
	/*setting fps to 30 */
	int currentTime = SDL_GetTicks();
	if ((timeDiff = currentTime - oldTime) > FRAME_RATE ) 
		oldTime = currentTime;
	else 
		SDL_Delay(FRAME_RATE - timeDiff);
	
	if (!stack_empty(s))		
		top(s).update();
}

bool game_render() 
{
	if(!stack_empty(s))
		if(!top(s).render(screen))
			return 0;
	return 1;		
}

void game_exit()
{
	while(!stack_empty(s)) {
		top(s).free();
		pop(&s);
	}	
}

void game_free() 
{
	SDL_FreeSurface(screen);
	SDL_Quit();
}
