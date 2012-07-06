#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

/* types definition */
typedef enum {USER, COMPUTER} type_t;

typedef struct {
	/* SDL_Surface* box; */
	unsigned short gained_cards;
	unsigned short points;
} score_t;


typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score_t score;
    SDL_Surface *score_box;
    SDL_Rect pos_score_box;
    int nb_cards_in_hand;
    type_t type;
    int ronda_tringla;
} player;

/* functions definition */

/*
 * returns card value specified by index
 */  
inline card_num get_card_val(player p, short index);

/*
 * initializes player data
 */ 
player* player_init(type_t t);

/*
 * distributes cards to player p
 */  
bool player_distribute(card_num card_list[],player *p, unsigned short *nb_cards_remaining);

/*
 * renders player cards and score on the screen
 */ 
bool player_render(player *p, SDL_Surface *scr);

/*
 * frees memory taken by player data
 */ 
void player_free(player *p);
#endif
