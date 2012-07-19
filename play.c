#include <SDL/SDL_image.h>

#include "play.h"
#include "basic.h"



short get_selected_hand(int x) 
{
	if (between(x,150, 240)) {
		return 0;
	} else if (between(x, 280,370)) {
		return 1;
	} else if (between(x,410, 500)) {
		return 2;
	} else return -1;	
}

short get_selected_table(int x, int y) 
{
	unsigned short temp=5, c=0;
	if (between(x, 40, 130)) temp=0;
	else if(between(x, 145, 235)) temp=1;
	else if (between(x, 250, 340)) temp=2;
	else if(between(x, 355, 445)) temp=3;
	else if (between(x, 460, 550)) temp=4;
	
	if (temp!=5) { 
		if (between(y, 160, 295))	c=temp;
		else if (between(y, 305, 440)) c=temp+5;
		return c;
	}
	else return -1;	
}

static void eat(player *p, card table[], short sel_hand, short sel_table) 
{
	int i = 0, index = 0;

	card_num played_card=get_card_val(*p, sel_hand);
	short y = (p->type == USER)? 450 : 15;
	set_card(&p->hand[sel_hand], EMPTY, -1, y, 0);
	p->score.gained_cards++;
	
	index = exist(table, MAX_NB_CARDS_TABLE, played_card);
	while((index != -1) && (i<= 9-played_card%10)) {  
		SDL_FreeSurface(table[index].surf);
		set_card(&table[index], EMPTY, -1, -1, 0);
		p->score.gained_cards++;
		i++;
		index=exist(table, MAX_NB_CARDS_TABLE, played_card+i);
	}
	
	if (empty(table, MAX_NB_CARDS_TABLE)) 
		p->score.points++;	
}


void user_turn(player *p, card table[], short sel_hand, short sel_table,
		card_num *dropped_card) 
{
	if (p->nb_cards_in_hand == 3) 
		*dropped_card=EMPTY;
	
	short y = (p->type == USER)? 450 : 15;	
	if ((table[sel_table].value == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, p->hand[sel_hand].value) == -1)) {
		SDL_FreeSurface(table[sel_table].surf);
		if (p->type == USER)
			table[sel_table].surf = p->hand[sel_hand].surf;
		else
			table[sel_table].surf = IMG_Load(get_file(p->hand[sel_hand].value));
		
		table[sel_table].value = p->hand[sel_hand].value;
		*dropped_card = get_card_val(*p, sel_hand);
		set_card(&p->hand[sel_hand], EMPTY, -1, y, 0); 
	} else if (table[sel_table].value != EMPTY) {
		if (equal(p->hand[sel_hand].value, table[sel_table].value)) {
			if(table[sel_table].value == *dropped_card) 
				p->score.points++;
			*dropped_card=EMPTY;
			eat(p, table, sel_hand, sel_table);
		} else {
			p->hand[sel_hand].position->y = y;
		}
	} else {
		p->hand[sel_hand].position->y = y;
	}
	p->nb_cards_in_hand--;	
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
		card_num tmp = get_card_val(*p, i);
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
		short i = rand_a_b(0, 3);
		while (table[i].value != EMPTY)
			i = rand_a_b(0, 3);
		index_tab = i;	
	}
	user_turn(p, table, index_hand, index_tab, dropped_card);	
}
