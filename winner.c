#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "winner.h"
#include "common.h"
#include "game_state.h"
#include "game.h"
#include "main_game.h"
#include "define.h"

/* constants */
#define SELECTOR_NEW_GAME_X 375
#define SELECTOR_NEW_GAME_Y 370
#define SELECTOR_EXIT_GAME_X 375
#define SELECTOR_EXIT_GAME_Y 420
#define WINNER_MSG_Y 150
#define WINNER_MSG_X(w) (MIDDLE_X(w) - 100)
#define USER_SCORE_X 50
#define COMPUTER_SCORE_X(w) (WINDOW_WIDTH - 50 - w)
#define PLAYER_SCORE_Y 20 

/* static data */
static SDL_Surface *winner_surf = NULL;
static SDL_Surface *selector = NULL;
static SDL_Rect selector_pos;
static enum {NEW_GAME, EXIT_GAME} entry;
static bool new_game ;
static enum{USER_WINS, COMPUTER_WINS, DRAWN} game_result;


void winner_init()
{
	winner_surf = load_image("data/winner.png", __FILE__, __LINE__);
	selector = load_image("data/selector.png", __FILE__, __LINE__);

	if (TTF_Init() == -1)
		sdl_ttf_error("Initialisation failed", __FILE__, __LINE__);
	new_game = 0;
	entry = NEW_GAME;
	selector_pos.x = SELECTOR_NEW_GAME_X;
	selector_pos.y = SELECTOR_NEW_GAME_Y;

	if (user_score.points > computer_score.points)
		game_result = USER_WINS;
	else if (user_score.points < computer_score.points)
		game_result = COMPUTER_WINS;
	else if (user_score.gained_cards > computer_score.gained_cards)
		game_result = USER_WINS;
	else if (user_score.gained_cards < computer_score.gained_cards)
		game_result = COMPUTER_WINS;
	else
		game_result = DRAWN;
}

void winner_handle_input()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game_exit(EXIT_SUCCESS);
		break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP:
				entry = NEW_GAME;
			break;
                       
			case SDLK_DOWN:
				entry = EXIT_GAME;
			break;

			case SDLK_RETURN:
				if (entry == EXIT_GAME)
					game_exit(EXIT_SUCCESS);
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
		SDL_Delay(300);
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

static void show_winner_msg(SDL_Surface *scr)
{
	char buf[27] = "";
	if (game_result == USER_WINS)
		sprintf(buf, "Congratulations, You Win");
	else if (game_result == COMPUTER_WINS)
		sprintf(buf, "Sorry, You Loose");
	else
		sprintf(buf, "Drawn");

	SDL_Surface *surf = set_text_surf("data/urw-bookman-l.ttf", 36, buf, 255, 255, 255);
	SDL_Rect pos = {WINNER_MSG_X(surf->w), WINNER_MSG_Y};
	if (SDL_BlitSurface(surf, NULL, scr, &pos) == -1)
		sdl_error("Blit win message fail", __FILE__, __LINE__);
	free_surface(surf);
}

static void show_final_score(SDL_Surface *scr)
{
	char buf[20] = "";
	SDL_Rect pos;
	sprintf(buf, "Your score : %d", user_score.points);
	show_white_text("data/georgiai.ttf", 24, buf, USER_SCORE_X, PLAYER_SCORE_Y, scr);

	sprintf(buf, "Computer score : %d", computer_score.points);
	SDL_Surface *surf = set_text_surf("data/georgiai.ttf", 24, buf, 255, 255, 255);
	pos.x = COMPUTER_SCORE_X(surf->w);
	pos.y = PLAYER_SCORE_Y;
	if (SDL_BlitSurface(surf, NULL, scr, &pos) == -1)
		sdl_error("Blit final score fail", __FILE__, __LINE__);
	free_surface(surf);
}

void winner_render(SDL_Surface *screen)
{
	if (SDL_BlitSurface(winner_surf, NULL, screen, NULL) == -1)
		sdl_error("Blit winner background fail", __FILE__, __LINE__);
	if (SDL_BlitSurface(selector, NULL, screen, &selector_pos) == -1)
		sdl_error("Blit winner selector fail", __FILE__, __LINE__);

	show_final_score(screen);
	show_winner_msg(screen);

	if (SDL_Flip(screen) == -1)
		sdl_error("SDL_flip fail", __FILE__, __LINE__);
}

void winner_free()
{
	TTF_Quit();
	free_surface(winner_surf);
	free_surface(selector);
}

game_state_t* set_state_winner()
{
	game_state_t *gs = try_malloc(sizeof(game_state_t));
	gs->init = winner_init;
	gs->handle_input = winner_handle_input;
	gs->update = winner_update;
	gs->render = winner_render;
	gs->free = winner_free;

	return gs;
}
