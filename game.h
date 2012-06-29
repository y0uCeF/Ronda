#ifndef GAME_H
#define GAME_H

#include "player.h"

	#define WINDOW_WIDTH 800
	#define WINDOW_HEIGHT 600
	#define FPS 30
	#define FRAME_RATE 1000/FPS
    
    
typedef struct {
	volatile shortint running;
	SDL_Surface *screen;	
	SDL_Event event;
	player *user;
	player *comp;
	card table[MAX_NB_CARDS_TABLE];
	card_num cardList[NB_CARDS];
	short selectedTable;
	short selectedHand;
	shortint nbCardsRemaining;
	type_t currentPlayer;
	card_num droppedCard;
} game_t;



game_t* game_init();

bool game_run(game_t *g);

void game_free(game_t *g);

#endif
