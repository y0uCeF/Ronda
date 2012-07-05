#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

typedef enum {USER, COMPUTER} type_t;

typedef struct {
	/* SDL_Surface* box; */
	unsigned short gainedCards;
	unsigned short points;
} score_t;


typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score_t score;
    SDL_Surface *scoreBox;
    SDL_Rect posScoreBox;
    int nb_cards_in_hand;
    type_t type;
    int ronda_tringla;
} player;

inline card_num get_card(player p, short index);

player* player_init(type_t t);

bool player_distribute(card_num cardList[],player *p, unsigned short *nbCardsRemaining);

bool player_render(player *p, SDL_Surface *scr);

void player_free(player *p);
#endif
