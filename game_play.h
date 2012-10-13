#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "player.h"

typedef enum {
        NO_VALID_INPUT, PUT_CARD, GET_FIRST_CARD, GET_CARDS, END_ACTIONS
} player_state_type;

/*
 * executes a player turn
 */ 
void player_turn(player *p, card table[]);

/*
 * uses AI to pick cards
 */
void set_computer_choice(player *p, card table[]);

/*
 * takes all the cards in the last round
 */ 
void take_all_cards(player *p, card table[]);

/*
 * calculates points regarding bonus
 */ 
void handle_bonus(player *p1, player *p2);
#endif
