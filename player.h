#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

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


typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score_t score;
    SDL_Surface *score_box;
    SDL_Rect pos_score_box;
    int nb_cards_in_hand;
    type_t type;
    short sel_hand;
    short sel_table;
    enum {NONE, RONDA, TRINGLA} bonus_type; /*Ronda or Tringla */
    enum {NO_EXTRA , ESTE, MISSA} extra_bonus; /*Este or Missa*/
    SDL_Surface *bonus_surf; /*stores the bonus text*/
    short bonus_card; /*the card concerned with the Ronda/Tringla*/
    bool bonus_shown; /*tells whether the bonus has been displayed or not*/
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
bool ronda(player *p);

/*
 * Checks if player has Tringla
 */ 
bool tringla(player *p);

/*
 * sets bonus type to the player
 */ 
void set_bonus(player *p);

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
