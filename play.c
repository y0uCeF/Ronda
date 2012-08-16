#include <SDL/SDL_image.h>

#include "play.h"
#include "common.h"


static void take_cards(player *p, card table[]) 
{
	int i = 0, index = 0;

	card_num played_card = get_sel_hand_val(*p);
	short y = (p->type == USER)? 450 : 15;
	set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0);
	p->score.gained_cards++;
	
	index = exist(table, MAX_NB_CARDS_TABLE, played_card);
	while((index != -1) && (i <= 9-played_card%10)) {  
		SDL_FreeSurface(table[index].surf);
		set_card(&table[index], EMPTY, -1, -1, 0);
		p->score.gained_cards++;
		i++;
		index=exist(table, MAX_NB_CARDS_TABLE, played_card+i);
	}
	
	if (empty(table, MAX_NB_CARDS_TABLE)) 
		p->score.points++;	
}

static unsigned short get_gain(card_num c, card table[], card_num dropped_card)
{
	unsigned short i = 0, count = 0;
	unsigned short res = 1;
	if (c%10 != 9) {
		short index = exist(table, MAX_NB_CARDS_TABLE, c);
		while ((index != -1) && (i<= 9-c%10)) {
			count++;
			index = exist(table, MAX_NB_CARDS_TABLE, c+count);
		}
	}
	if (c == dropped_card) 
		res++;
        if (count >= 3)
		res++;
        
        return res;
}

static short get_index_ai(player *p, card table[], card_num dropped_card, 
		short *index_tab)
{
	unsigned short i, j;
	unsigned short tmp_gain, gain = 0;
	short best_card_index = -1;
	*index_tab = -1;
	for (i=0; i < MAX_NB_CARDS_HAND; i++) {
		card_num tmp = p->hand[i].value;
		if (tmp != EMPTY) 
			for (j=0; j < MAX_NB_CARDS_TABLE; j++) {
				if (equal(tmp, table[j].value)) {
					tmp_gain = get_gain(tmp, table, dropped_card);
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

bool valid_move(player p, card table[])
{
	card_num selected_hand = get_sel_hand_val(p);
	card_num selected_table = table[p.sel_table].value;
	if ((selected_table == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, selected_hand) != -1))
		return 0;
	else if ((selected_table != EMPTY) &&
		(!equal(selected_hand, selected_table)))
		return 0;
	else return 1;		
}

void user_turn(player *p, card table[], card_num *dropped_card) 
{
	if (p->nb_cards_in_hand == 3) 
		*dropped_card = EMPTY;
	
	card_num selected_hand = get_sel_hand_val(*p);
	card_num selected_table = table[p->sel_table].value;
	short y = (p->type == USER)? 450 : 15;	
	if ((selected_table == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, selected_hand) == -1)) {
		SDL_FreeSurface(table[p->sel_table].surf);
		if (p->type == USER)
			table[p->sel_table].surf = get_sel_hand_surf(*p);
		else
			table[p->sel_table].surf = IMG_Load(get_file(selected_hand));
		
		table[p->sel_table].value = selected_hand;
		*dropped_card = selected_hand;
		set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0); 
	} else if (selected_table != EMPTY) {
		if (equal(selected_hand, selected_table)) {
			if(selected_table == *dropped_card) 
				p->score.points++;
			*dropped_card=EMPTY;
			take_cards(p, table);
		} else {
			p->hand[p->sel_hand].position->y = y;
		}
	} else {
		p->hand[p->sel_hand].position->y = y;
	}
	p->nb_cards_in_hand--;	
}

void computer_turn(player *p, card table[], card_num *dropped_card)
{
	short index_tab;
	short index_hand = get_index_ai(p, table, *dropped_card, &index_tab);
	
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
	
	user_turn(p, table, dropped_card);	
}
