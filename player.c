#include <SDL/SDL_image.h>

#include "player.h"
#include "basic.h"

player* player_init(type_t t) 
{
	unsigned short i;
	player *p=malloc(sizeof(player));
	for (i=0; i<MAX_NB_CARDS_HAND; i++) {
		p->hand[i].number=EMPTY;
		p->hand[i].surf=NULL;
		p->hand[i].position=NULL;
	}	
	p->nb_cards_in_hand=0;
	p->score.gainedCards=0;
	p->score.points=0;
	p->type=t;
	p->posScoreBox.x=600;
	p->posScoreBox.y= (t == USER)? 470:40;
	p->scoreBox=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	return p;	
}


bool player_distribute(card_num cardList[],player *p, unsigned short *nb_cards_remaining) 
{
	unsigned short i;   
	unsigned short posx, posy;
	bool back;

	for(i=0; i<MAX_NB_CARDS_HAND;i++) {
		posx = (90+40)*i+150;
		

		if (p->type == USER) {			
			posy = 450;
			back=0;
		} else {
			posy = 15;
			back=1;
		}
		
		set_card(&p->hand[i], cardList[*nb_cards_remaining-1-i], posx, posy, back);

		p->nb_cards_in_hand++;
	}
	*nb_cards_remaining -= i;
	return 1;
}

bool player_render(player *p, SDL_Surface *scr)
{
	unsigned short i;
	SDL_FillRect(p->scoreBox, NULL, SDL_MapRGB(p->scoreBox->format, 35, 50, 20));
	
	for (i=0; i < MAX_NB_CARDS_HAND; i++)
		if (SDL_BlitSurface(p->hand[i].surf, NULL, scr, p->hand[i].position) == -1) 
			return 0; 
	if(SDL_BlitSurface(p->scoreBox, NULL, scr, &p->posScoreBox) == -1) 
		return 0;	
	return 1;
}

void player_free(player *p)
{
	unsigned short i;
	for (i=0; i < p->nb_cards_in_hand;i++) {
		if(p->hand[i].surf != NULL)	
			SDL_FreeSurface(p->hand[i].surf); 
		if(p->hand[i].position != NULL) 
			free(p->hand[i].position);
	}
	if(p->scoreBox != NULL) 
		SDL_FreeSurface(p->scoreBox);
	free(p);	
}

inline card_num get_card(player p, short index)
{
	return p.hand[index].number;
}
