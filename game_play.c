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

static bool pause(short max_frames)
{
        if (nb_frames < max_frames) {
		nb_frames++;
                return 1;
        } else {
                nb_frames = 0;
                return 0;
        }
}

static bool take_card(player *p, card table[]) 
{	
	int index = exist(table, MAX_NB_CARDS_TABLE, current_card);
	if(index != -1) {
                if(!pause(PAUSE_FRAMES_CARDS)) {
                        /*the cards exists and it's time*/
                        SDL_FreeSurface(table[index].surf);
                        set_card(&table[index], EMPTY, -1, -1, 0);
                        p->score.gained_cards++;
                        if (current_card != 9)
                                current_card++;
                }
                return 1;
	} else {
                /*no cards to take*/
                if (empty(table, MAX_NB_CARDS_TABLE)) 
                        p->score.points++;
                        
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
        
	short y = PLAYER_YPOS(p->type);	
        
        /*what will the player do, depending on the states*/
        switch (state) {
        case NO_VALID_INPUT:       
                if ((p->sel_hand != -1) && (p->sel_table != -1))
                        update_state(*p, table);
        break;
        
        case PUT_CARD:
                if (pause(PAUSE_FRAMES_PLAYERS) && (p->type == COMPUTER))
                        break;
                        
                /*moving the card from hand to table*/
                if (p->type == USER)
			table[p->sel_table].surf = get_sel_hand_surf(*p);
		else
			table[p->sel_table].surf = IMG_Load(get_file(selected_hand));
		
		table[p->sel_table].value = selected_hand;
		dropped_card = selected_hand;
		set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0);
                p->nb_cards_in_hand--;
                
                /*no further actions*/
                state = END_ACTIONS;
        break;
        
        case GET_FIRST_CARD:
                if (pause(PAUSE_FRAMES_PLAYERS) && (p->type == COMPUTER))
                        break;
                if(selected_table == dropped_card) 
			p->score.points++;
                        
		dropped_card = EMPTY;
		last_card_taker = p;
                
                /*removing both cards from hand/table */
                set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0);
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
	unsigned short i = 0, count = 0;
        /*res: the more cards/points the player can gain, 
                the more is the variable's value */
	unsigned short res = 1; 
        
        /*how many cards can the player can get*/
	if (c % 10 != 9) {
		short index = exist(table, MAX_NB_CARDS_TABLE, c);
		while ((index != -1) && (i <= (9 - c%10))) {
			count++;
                        i++;
			index = exist(table, MAX_NB_CARDS_TABLE, c+count);
		}
	}
        
        res += count;
        
        /*we make sure this is a better gain than in taking 2 cards*/
        if (c == dropped_card)
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

void handle_bonus(player *p1, player *p2)
{
	if ((p1->bonus_type == NONE) && (p2->bonus_type == NONE))
		return;
	
        /*check the Readme file for points calculation*/
	if (p1->bonus_type > p2->bonus_type)
		p1->score.points += (p1->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_type < p2->bonus_type)
		p2->score.points += (p2->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_card > p2->bonus_card)
		p1->score.points += (p1->bonus_type == RONDA)? 2:5;
	else if (p1->bonus_card < p2->bonus_card)
		p2->score.points += (p2->bonus_type == RONDA)? 2:5;
		
	p1->bonus_card = -1;
	p1->bonus_type = NONE;
	p2->bonus_card = -1;
	p2->bonus_type = NONE;
	
}

