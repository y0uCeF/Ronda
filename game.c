#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

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
static SDL_Surface *screen = NULL;
stack s = NULL;
static int oldTime = 0;

static void env_init() 
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		sdl_error("Unable to init SDL", __FILE__, __LINE__);
	putenv("SDL_VIDEO_CENTERED=1");

	SDL_Surface *icon = SDL_LoadBMP(ICON_FILE);
	if(!icon)
		fprintf(stderr, "Unable to load icon : %s\n", SDL_GetError());
	else
		SDL_WM_SetIcon(icon, NULL);

	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  
				SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (screen == NULL) 
		sdl_error("Video initialization failed", __FILE__, __LINE__);
	if (SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68)) == -1) 
		sdl_error("Setting screen failed", __FILE__, __LINE__);

	SDL_WM_SetCaption("Ronda", NULL);
}

void game_init() 
{
	s = NULL;
	game_state_t *tmp = set_state_main_game();
	push(&s, *tmp);

	env_init();

	top(s).init();
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
		top(s).render(screen);
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
	free_surface(screen);
	try_free(s);
	SDL_Quit();
}
