#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "main_game.h"
#include "winner.h"
#include "game_state.h"
#include "common.h"

/* Constants definition */
#define FPS         30
#define FRAME_RATE  1000/FPS
#define ICON_FILE   GFX_DIR "icon.bmp"

/* static Data */
static SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
stack s = NULL;
static int oldTime = 0;

static void env_init() 
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		sdl_error("Unable to init SDL");
	
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
		sdl_error("Unable to init SDL_image");
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.

	window = SDL_CreateWindow("Ronda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE); // | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		sdl_error("CreateWindow");
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
		sdl_error("CreateRenderer");
		
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	

	SDL_Surface *icon = SDL_LoadBMP(ICON_FILE);
	if(!icon) {
		fprintf(stderr, "Unable to load icon : %s\n", SDL_GetError());
	} else {
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	 }
}

void game_init() 
{
	s = NULL;
	game_state_t *tmp = set_state_main_game();
	push(&s, *tmp);

	env_init();

	top(s).init(renderer);
	try_free(tmp);
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

void game_render() 
{
	if (!stack_empty(s))
		top(s).render(renderer);
}

void game_exit(int status)
{
	while (!stack_empty(s)) {
		top(s).free();
		pop(&s);
	}
	exit(status);
}

void game_free() 
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}
