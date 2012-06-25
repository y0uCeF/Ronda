#include <stdio.h>
#include <string.h>
#include <SDL/SDL_image.h>


#include "basic.h"
#include "functions.h"


shortint nbCardsRemaining=NB_CARDS;
Type currentPlayer=USER;
card_num droppedCard=EMPTY;


void init(card table[],player *p1, player *p2) {
	shortint i;
	for (i=0; i<MAX_NB_CARDS_TABLE; i++) {
		table[i].number=EMPTY;
		table[i].surf=NULL;
		table[i].position=NULL;
	}
	
	for (i=0; i<MAX_NB_CARDS_HAND; i++) {
		p1->hand[i].number=EMPTY;
		p1->hand[i].surf=NULL;
		p1->hand[i].position=NULL;
		
		p2->hand[i].number=EMPTY;
		p2->hand[i].surf=NULL;
		p2->hand[i].position=NULL;
	}	
	
	p1->nbCardsInHand=0;
	p1->score_player.gainedCards=0;
	p1->score_player.points=0;
	p1->type=USER;
	p1->posScoreBox.x=600;
	p1->posScoreBox.y=40;
	p1->scoreBox=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	
	p2->nbCardsInHand=0;
	p2->score_player.gainedCards=0;
	p2->score_player.points=0;
	p2->type=COMPUTER;
	p2->posScoreBox.x=600;
	p2->posScoreBox.y=470;	
	p2->scoreBox=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	
}	

shortint distributeTable(card_num cardList[],card table[]) {
	shortint i,k=0,j=1;
	table[0].number=cardList[nbCardsRemaining-1];
    char file[16]="";
    getFile(table[0].number,file);
    table[0].surf=IMG_Load(file);
    if(table[0].surf == NULL) {
		fprintf(stderr, "couldn't locate : %s", file);
		return 0;
	}
    table[0].position=malloc(sizeof(SDL_Rect));
    table[0].position->x=40;
    table[0].position->y=160;
    
    for (i=1; i<MAX_NB_CARDS_TABLE ; i++) {
		while (exist(table, i, cardList[nbCardsRemaining-j-1]) != -1) {
			swap(&cardList[nbCardsRemaining-j-1], &cardList[nbCardsRemaining-j-k-1]);
			k++;
		}
		file[0]='\0';
		if ((i==2) || (i==4) || (i==6)) {
		table[i].number=cardList[nbCardsRemaining-j-1];
		getFile(table[i].number,file);
		table[i].surf=IMG_Load(file);
		j++;
		}
		else {
			strcat(file, "cards/blank.gif");
			table[i].number=EMPTY;
			table[i].surf=IMG_Load(file);	
		}
		if(table[i].surf == NULL) {
		fprintf(stderr, "couldn't locate : %s", file);
		return 0;
		}
		table[i].position=malloc(sizeof(SDL_Rect));
		table[i].position->x= (90+15)*(i%5) + 40;
		table[i].position->y= (i<=4)? 160:305; 		
	}	
    nbCardsRemaining -= j; 
    return 1;
}

shortint distributePlayer(card_num cardList[],player *pl) {
    shortint i;   
    for(i=0; i<MAX_NB_CARDS_HAND;i++) {
		char file[15]="";
        pl->hand[i].number=cardList[nbCardsRemaining-1-i];
        pl->hand[i].position=malloc(sizeof(SDL_Rect));
        pl->hand[i].position->x=(90+40)*i+150;
        if (pl->type == USER) {			
			getFile(pl->hand[i].number,file);		
			pl->hand[i].surf=IMG_Load(file);	
			pl->hand[i].position->y=450;
		}
		else {
			strcat(file, "cards/back.gif");
			pl->hand[i].surf=IMG_Load(file);
			pl->hand[i].position->y=15;
		}
		if(pl->hand[i].surf	== NULL) {
			fprintf(stderr, "can't find : %s", file);
			return 0;
		}
        pl->nbCardsInHand++;
    }
    nbCardsRemaining -= i;
    return 1;
}

short getSelectedHand(int x) {
	if ((x >150) && (x<240)) {
		return 0;
	}
	else if ((x >280) && (x<370)) {
		return 1;
	}
	else if ((x >410) && (x<500)) {
		return 2;
	}
	else return -1;	
}

short getSelectedTable(int x, int y) {
	shortint temp=5, c=0;
	if ((x>40) && (x<130)) temp=0;
	else if((x>145) && (x<235)) temp=1;
	else if ((x>250) && (x<340)) temp=2;
	else if((x>355) && (x<445)) temp=3;
	else if ((x>460) && (x<550)) temp=4;
	
	if (temp!=5) { 
		if ((y>160) && (y<295))	c=temp;
		else if ((y>305) && (y<440)) c=temp+5;
		return c;
	}
	else return -1;	
}

static void eat(player *p, card table[], short selectedHand, short selectedTable) {
	int i=0, indice=0;
	card_num playedCard=p->hand[selectedHand].number;
	p->hand[selectedHand].surf=IMG_Load("cards/blank.gif");
	p->hand[selectedHand].number=EMPTY;
	p->hand[selectedHand].position->y=450;
	p->score_player.gainedCards++;
	indice=exist(table, MAX_NB_CARDS_TABLE, playedCard);
	while((indice != -1) && (i<= 9-playedCard%10)) {  
	SDL_FreeSurface(table[indice].surf);
	table[indice].surf=IMG_Load("cards/blank.gif");	
	table[indice].number = EMPTY;
	p->score_player.gainedCards++;
	i++;
	indice=exist(table, MAX_NB_CARDS_TABLE, playedCard+i);
	}
	if (empty(table, MAX_NB_CARDS_TABLE)) p->score_player.points++;
	
}


void play(player *p, card table[], short selectedHand, short selectedTable) {
	if (nbCards(p->hand, MAX_NB_CARDS_HAND) ==3) droppedCard=EMPTY;
	if ((table[selectedTable].number == EMPTY) && (exist(table, MAX_NB_CARDS_TABLE, p->hand[selectedHand].number) == -1)) {
		SDL_FreeSurface(table[selectedTable].surf);
		table[selectedTable].surf=p->hand[selectedHand].surf;
		table[selectedTable].number=p->hand[selectedHand].number;
		droppedCard=p->hand[selectedHand].number;
		p->hand[selectedHand].surf=IMG_Load("cards/blank.gif");
		p->hand[selectedHand].number=EMPTY;
		p->hand[selectedHand].position->y=450; 
	}
	else if (table[selectedTable].number != EMPTY) {
		if (equal(p->hand[selectedHand].number, table[selectedTable].number)) {
			if(table[selectedTable].number == droppedCard) p->score_player.points++;
			droppedCard=EMPTY;
			eat(p, table, selectedHand, selectedTable);
		}
		else {
			p->hand[selectedHand].position->y=450;
		}
	}
	else {
		p->hand[selectedHand].position->y=450;
	}
		
}


shortint render(card table[], player user, player computer, SDL_Surface *screen) {
	int i;
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68));
	SDL_FillRect(user.scoreBox, NULL, SDL_MapRGB(user.scoreBox->format, 35, 50, 20));
	SDL_FillRect(computer.scoreBox, NULL, SDL_MapRGB(computer.scoreBox->format, 35, 50, 20));
	//bliting cards
	
	for (i=0; i<MAX_NB_CARDS_TABLE;i++) 
		if (SDL_BlitSurface(table[i].surf, NULL, screen, table[i].position) == -1) return 0;
			
	for (i=0; i<user.nbCardsInHand;i++)
		if (SDL_BlitSurface(user.hand[i].surf, NULL, screen, user.hand[i].position) == -1) return 0; 
		
	for (i=0; i<computer.nbCardsInHand;i++)
		if (SDL_BlitSurface(computer.hand[i].surf, NULL, screen, computer.hand[i].position) == -1) return 0;
				
	SDL_BlitSurface(user.scoreBox, NULL, screen, &user.posScoreBox);
	SDL_BlitSurface(computer.scoreBox, NULL, screen, &computer.posScoreBox);
	/*	if (selectionHand != NULL)	{
			if(SDL_BlitSurface(selectionHand, NULL, screen, user.hand[selectedHand].position) == -1) return 1;
		}*/
	return 1;	
}

void delete(card table[], player user, player computer) {
	int i;
	for (i=0;i<MAX_NB_CARDS_TABLE;i++) {
		if(table[i].surf != NULL) SDL_FreeSurface(table[i].surf);
		if(table[i].position != NULL) free(table[i].position);
	}
	
	for (i=0; i<user.nbCardsInHand;i++) {
		if(user.hand[i].surf != NULL)	SDL_FreeSurface(user.hand[i].surf); 
		if(user.hand[i].position != NULL) free(user.hand[i].position);
	}
	
	for (i=0; i<computer.nbCardsInHand;i++) {
		if(computer.hand[i].surf != NULL)	SDL_FreeSurface(computer.hand[i].surf); 
		if(computer.hand[i].position != NULL) free(computer.hand[i].position);
	}
	SDL_FreeSurface(user.scoreBox);
	SDL_FreeSurface(computer.scoreBox);
	//if (selectionHand != NULL) SDL_FreeSurface(selectionHand);
}


void treatEvent(SDL_Event event, shortint *running, player *user, short *selectedHand,short *selectedTable) {
	switch (event.type) {
		case SDL_QUIT:
			*running=0;
		break;	
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					*running=0;
				break;
				
				case SDLK_RETURN:
					*running=0;
				break;
					
				default:
				
				break;
			}	
		break;
		
		case SDL_MOUSEBUTTONUP:
			if (currentPlayer == USER)
				switch(event.button.button) {
					case SDL_BUTTON_LEFT:
						if (*selectedHand != -1) {
							user->hand[*selectedHand].position->y = 450;	
						}
						if (between(event.button.y, 450, 585)) {								
						*selectedHand = getSelectedHand(event.button.x);
						if ((*selectedHand != -1) && (user->hand[*selectedHand].number != EMPTY)) {		//selectionHand=IMG_Load("cards/selection.png"); 
						user->hand[*selectedHand].position->y = 447;
						}
						*selectedTable = -1;
						}
						else if (((between(event.button.y, 160, 295)) || (between(event.button.y, 305, 440)))
							&& (*selectedHand != -1))  {
							*selectedTable = getSelectedTable(event.button.x, event.button.y);
						}
						break;
						
						default:
				
						break;
				}
		break; 
				
		case SDL_MOUSEMOTION:
				
		break;
				
		default:
			
		break;
	}
}
