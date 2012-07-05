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

static void eat(player *p, card table[], short selectedHand, short selectedTable) 
{
	int i=0, index=0;
	card_num playedCard=get_card(*p, selectedHand);
	set_card(&p->hand[selectedHand], EMPTY, -1, 450, 0);
	p->score.gainedCards++;
	
	index=exist(table, MAX_NB_CARDS_TABLE, playedCard);
	while((index != -1) && (i<= 9-playedCard%10)) {  
	SDL_FreeSurface(table[index].surf);
	set_card(&table[index], EMPTY, -1, -1, 0);
	p->score.gainedCards++;
	i++;
	index=exist(table, MAX_NB_CARDS_TABLE, playedCard+i);
	}
	
	if (empty(table, MAX_NB_CARDS_TABLE)) p->score.points++;	
}


void user_turn(player *p, card table[], short selectedHand, short selectedTable, card_num *droppedCard) 
{
	if (p->nb_cards_in_hand == 3) 
		*droppedCard=EMPTY;
	if ((table[selectedTable].number == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, p->hand[selectedHand].number) == -1)) {
		SDL_FreeSurface(table[selectedTable].surf);
		table[selectedTable].surf=p->hand[selectedHand].surf;
		table[selectedTable].number=p->hand[selectedHand].number;
		*droppedCard=get_card(*p, selectedHand);
		set_card(&p->hand[selectedHand], EMPTY, -1, 450, 0); 
		
	} else if (table[selectedTable].number != EMPTY) {
		if (equal(p->hand[selectedHand].number, table[selectedTable].number)) {
			if(table[selectedTable].number == *droppedCard) 
				p->score.points++;
			*droppedCard=EMPTY;
			eat(p, table, selectedHand, selectedTable);
		} else {
			p->hand[selectedHand].position->y=450;
		}
	} else {
		p->hand[selectedHand].position->y=450;
	}
	p->nb_cards_in_hand--;	
}
