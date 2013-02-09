#include <SDL/SDL_image.h>

#include "game_play.h"
#include "player.h"
#include "common.h"
#include "define.h"

#define PAUSE_FRAMES_PLAYERS 30
#define PAUSE_FRAMES_CARDS 20

/*extern data*/
extern type_t current_player;

/*local data*/
static card_num current_card = EMPTY; /*used when player is taking cards*/
static int nb_frames = 0;

/*global data*/
player_state_type state = NO_VALID_INPUT; /*player turn states*/
card_num dropped_card = EMPTY;  /* last card dropped */
player *last_card_taker = NULL;



static bool take_card(player *p, card table[]) 
{	
	int index = exist(table, MAX_NB_CARDS_TABLE, current_card);
	if (index != -1) {
		if (passed(PAUSE_FRAMES_CARDS, &nb_frames)) {
			/*the card exists and it's time*/
			SDL_FreeSurface(table[index].surf);
			set_card(&table[index], EMPTY, -1, -1, 0);
			p->score.gained_cards++;
			if (current_card != 9)
				current_card++;
		}
	return 1;
	} else {
		/*no cards to take*/
		if (empty(table, MAX_NB_CARDS_TABLE)) {
			p->score.points++;
			add_bonus(p, MISSA, -1);
		}

		current_card = EMPTY;
		return 0;        
	}
}

void update_state(player p, card table[])
{
	card_num selected_hand = get_sel_hand_val(p);
	card_num selected_table = table[p.sel_table].value;
	
	if ((selected_table == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, selected_hand) == -1))
		state = PUT_CARD;
	else if ((selected_table != EMPTY) &&
			(equal(selected_hand, selected_table)))
		state = GET_FIRST_CARD;
	else 
		state = NO_VALID_INPUT;
}

void player_turn(player *p, card table[]) 
{
	card_num selected_hand = get_sel_hand_val(*p);
	card_num selected_table = table[p->sel_table].value;

	/*what will the player do, depending on the states*/
	switch (state) {
	case NO_VALID_INPUT:
		if ((p->sel_hand != -1) && (p->sel_table != -1))
			update_state(*p, table);
	break;

	case PUT_CARD:
		if (!passed(PAUSE_FRAMES_PLAYERS, &nb_frames) && (p->type == COMPUTER))
			break;

		/*moving the card from hand to table*/
		if (p->type == USER)
			table[p->sel_table].surf = get_sel_hand_surf(*p);
		else
			table[p->sel_table].surf = IMG_Load(get_file(selected_hand));
		
		table[p->sel_table].value = selected_hand;
		dropped_card = selected_hand;
		set_card(&p->hand[p->sel_hand], EMPTY, -1, -1, 0);
		p->nb_cards_in_hand--;

		/*no further actions*/
		state = END_ACTIONS;
	break;

	case GET_FIRST_CARD:
		if (!passed(PAUSE_FRAMES_PLAYERS, &nb_frames) && (p->type == COMPUTER))
			break;
		
		if (selected_table == dropped_card) {
			p->score.points++;
			add_bonus(p, ESTE, selected_table);
		}

		dropped_card = EMPTY;
		last_card_taker = p;

		/*removing both cards from hand/table */
		set_card(&p->hand[p->sel_hand], EMPTY, -1, -1, 0);
		SDL_FreeSurface(table[p->sel_table].surf);
		set_card(&table[p->sel_table], EMPTY, -1, -1, 0);
		p->score.gained_cards += 2;
		p->nb_cards_in_hand--;

		card_num tmp = selected_hand % 10;
		
		/*take the rest of the cards if possible*/
		if (tmp != 9) {
			state = GET_CARDS;
			current_card = tmp + 1;
		} else {
			if (empty(table, MAX_NB_CARDS_TABLE)) {
				p->score.points++;
				add_bonus(p, MISSA, -1);
			}
			state = END_ACTIONS;
		}
	break;

	case GET_CARDS:
		p->sel_hand = p->sel_table = -1;
		if (!take_card(p, table)) 
			state = END_ACTIONS;
	break;
	
	case END_ACTIONS:
		current_player = !p->type;
		p->sel_hand = p->sel_table = -1;
		state = NO_VALID_INPUT;
	break;
	}
}

static unsigned short get_gain(card_num c, card table[])
{
	unsigned short count = 0;
	/*res: the more cards/points the player can gain,
		the more is the variable's value */
	unsigned short res = 1;

	/*how many cards can the player can get*/
	if (c % 10 != 9) {
		short index = exist(table, MAX_NB_CARDS_TABLE, c);
		while ((index != -1) && (count <= (9 - c%10))) {
			count++;
			index = exist(table, MAX_NB_CARDS_TABLE, c+count);
		}
	}

	res += count;

	/*we make sure this is a better gain than in taking 2 cards*/
	if (equal(c, dropped_card))
		res += 2; 

	return res;
}

static short get_index_ai(player *p, card table[], short *index_tab)
{
	unsigned short i, j;
	unsigned short tmp_gain, gain = 0;
	short best_card_index = -1;
	*index_tab = -1;

	/*let's choose the best card (the best gain)*/
	for (i=0; i < MAX_NB_CARDS_HAND; i++) {
		card_num tmp = p->hand[i].value;
		if (tmp != EMPTY) 
			for (j=0; j < MAX_NB_CARDS_TABLE; j++) {
				if (equal(tmp, table[j].value)) {
					tmp_gain = get_gain(tmp, table);
					if (gain < tmp_gain) {
						gain = tmp_gain;
						best_card_index = i;
						*index_tab = j;
					}
				}
			}
		
	}
	
	return best_card_index;
}

void set_computer_choice(player *p, card table[])
{
	short index_tab;
	short index_hand = get_index_ai(p, table, &index_tab);
	
	if (index_hand == -1) {
		short i = rand_a_b(0, 3);
		while (p->hand[i].value == EMPTY)
			i = rand_a_b(0, 3);
		index_hand = i;	
	}
	
	if (index_tab == -1) {
		short i = rand_a_b(0, 10);
		while (table[i].value != EMPTY)
			i = rand_a_b(0, 10);
		index_tab = i;	
	}
	p->sel_hand = index_hand;
	p->sel_table = index_tab;
}

void take_all_cards(player *p, card table[])
{
	unsigned short i;
	for (i = 0; i < MAX_NB_CARDS_TABLE; ++i) 
		if (table[i].value != EMPTY){
			SDL_FreeSurface(table[i].surf);
			set_card(&table[i], EMPTY, -1, -1, 0);
			p->score.gained_cards++;
		}	
}

void handle_bonus(score_t *p1_score, bonus_t *p1_bonus, score_t *p2_score, bonus_t *p2_bonus)
{
	if ((p1_bonus->type == NONE) && (p2_bonus->type == NONE))
		return;
	
	/*check the HowToPlay file for points calculation*/
	if (p1_bonus->type > p2_bonus->type)
		p1_score->points += ((p1_bonus->type == RONDA)? 1:5) +
			((p2_bonus->type == NONE)? 0:1);
	else if (p1_bonus->type < p2_bonus->type)
		p2_score->points += ((p2_bonus->type == RONDA)? 1:5) +
			((p1_bonus->type == NONE)? 0:1);
	else if (p1_bonus->bonus_card > p2_bonus->bonus_card)
		p1_score->points += (p1_bonus->type == RONDA)? 2:10;
	else if (p1_bonus->bonus_card < p2_bonus->bonus_card)
		p2_score->points += (p2_bonus->type == RONDA)? 2:10;
	else 
		p1_score->points = p2_score->points = (p1_bonus->type == RONDA)? 1:5;
	
	p1_bonus->bonus_card = -1;
	p1_bonus->type = NONE;
	p2_bonus->bonus_card = -1;
	p2_bonus->type = NONE;
	
}
