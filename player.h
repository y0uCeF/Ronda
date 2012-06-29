#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

typedef enum {USER, COMPUTER} type_t;

typedef struct {
	//SDL_Surface* box;
	shortint gainedCards;
	shortint points;
} score_t;


typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score_t score;
    SDL_Surface *scoreBox;
    SDL_Rect posScoreBox;
    int nbCardsInHand;
    type_t type;
    int ronda_tringla;
} player;


player* player_init(type_t t);

bool player_distribute(card_num cardList[],player *p, shortint* nbCardsRemaining);

#endif
