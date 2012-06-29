#include <SDL/SDL_image.h>


#include "player.h"
#include "basic.h"


player* player_init(type_t t) {
	shortint i;
	player *p=malloc(sizeof(player));
	for (i=0; i<MAX_NB_CARDS_HAND; i++) {
		p->hand[i].number=EMPTY;
		p->hand[i].surf=NULL;
		p->hand[i].position=NULL;
	}	
	p->nbCardsInHand=0;
	p->score.gainedCards=0;
	p->score.points=0;
	p->type=t;
	p->posScoreBox.x=600;
	p->posScoreBox.y= (t == USER)? 470:40;
	p->scoreBox=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	return p;	
}


bool player_distribute(card_num cardList[],player *p, shortint* nbCardsRemaining) {
	shortint i;   
    for(i=0; i<MAX_NB_CARDS_HAND;i++) {
		char file[15]="";
        p->hand[i].number=cardList[*nbCardsRemaining-1-i];
        p->hand[i].position=malloc(sizeof(SDL_Rect));
        p->hand[i].position->x=(90+40)*i+150;
        if (p->type == USER) {			
			getFile(p->hand[i].number,file);		
			p->hand[i].surf=IMG_Load(file);	
			p->hand[i].position->y=450;
		}
		else {
			strcat(file, "cards/back.gif");
			p->hand[i].surf=IMG_Load(file);
			p->hand[i].position->y=15;
		}
		if(p->hand[i].surf	== NULL) {
			fprintf(stderr, "couldn't load : %s", file);
			return 0;
		}
        p->nbCardsInHand++;
    }
    *nbCardsRemaining -= i;
    return 1;
	
}
