#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "main_game.h"
#include "game_state.h"
#include "common.h"
#include "play.h"


SDL_Surface *screen = NULL;
stack s;

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
	game_state_t tmp;
	set_state_main_game(&tmp);
	push(&s, tmp);
	if (!env_init()) 
		return;
	
	top(s).init();
}	

void game_handle_input()
{
	top(s).handle_input();
}

void game_update()
{
	int currentTime = 0, oldTime = 0,
		timeDiff; /* time difference calculations */	
	
	/*setting fps to 30 */
		currentTime = SDL_GetTicks();
		if ((timeDiff = currentTime - oldTime) > FRAME_RATE ) 
			oldTime = currentTime;
		else 
			SDL_Delay(FRAME_RATE - timeDiff);
	if (!stack_empty(s))		
		top(s).update();
}

void game_render() 
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68));
	
	if(!stack_empty(s))
		top(s).render(screen);
}

void game_free() 
{
	SDL_FreeSurface(screen);
	SDL_Quit();
}
