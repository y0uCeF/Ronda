#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

#define MAX_BONUS 3
#define CARD_WIDTH 90
#define INNER_SPACE_PLAYER 40
#define PLAYER_X 150
#define COMPUTER_Y 15
#define USER_Y 450
#define PLAYER_XPOS(index) (CARD_WIDTH + INNER_SPACE_PLAYER) * index \
		+ PLAYER_X

#define PLAYER_YPOS(type) (type == USER)? USER_Y : COMPUTER_Y

/* types definition */
typedef enum {USER, COMPUTER} type_t;

typedef struct {
	unsigned short gained_cards;
	unsigned short points;
} score_t;


typedef enum {NONE, RONDA, TRINGLA, ESTE, MESA} bonus_type_t;

typedef struct {
	bonus_type_t type;
	SDL_Surface *surf; /* bonus text */
	short bonus_card; /* the card concerned with the Ronda/Tringla */
	bool bonus_shown; /* bonus displayed ? */
	int show_frames;
} bonus_t;

typedef struct player {
	card hand[MAX_NB_CARDS_HAND];
	int nb_cards_in_hand;
	score_t score;
	SDL_Surface *score_box;
	SDL_Rect pos_score_box;
	bonus_t *action_bonus[MAX_BONUS]; /* este/mesa */
	bonus_t card_bonus;
	type_t type;
	short sel_hand; /* selected hand index */
	short sel_table; /* selected table index */   
} player;

/* functions definition */

/*
 * returns card value specified by sel_hand
 */
inline card_num get_sel_hand_val(player p);

/*
 * returns card surface specified by sel_hand
 */
inline SDL_Surface* get_sel_hand_surf(player p);

/*
 * Checks if player has Ronda
 */
bool ronda(player *p, card_num *c);

/*
 * Checks if player has Tringla
 */
bool tringla(player *p, card_num *c);

/*
 * sets card_bonus type to the player Ronda/Tringla
 */
void set_card_bonus(player *p);

/*
 * sets player bonus of any type
 */
void add_bonus(player *p, bonus_type_t type, short c);
 
/*
 * sets player's final points
 */
void set_final_score(player *p);

/*
 * initializes player data
 */
player* player_init(type_t t);

/*
 * distributes cards to player p
 */
void player_distribute(card_num card_list[],player *p, unsigned short *nb_cards_remaining);

/*
 * updates player data
 */
void player_update_bonus(player *p); 
/*
 * renders player cards and score on the screen
 */
void player_render(player *p, SDL_Surface *scr);

/*
 * frees memory taken by player data
 */
void player_free(player *p);
#endif
