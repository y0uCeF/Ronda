#include <SDL/SDL_image.h>
#include "functions.h"


shortint nbCardsRemaining=NB_CARDS;

void swap(card_num *a, card_num *b) {
    int tmp = *a;
    *a= *b;
    *b = tmp;
}

shortint equal(shortint a, shortint b) {
    return ((a%10) == (b%10));
}

shortint exist(card tab[], int size, int value)
{
    shortint i;
    for (i=0; i<size; i++) {
        if (equal(tab[i].number,value)) return 1;
    }
    return 0;
}


void getFile(card_num n, char* file) {
	char num[3]="";
	num[0]=(char) (n/10+ (char) '0');
	num[1]=(char) (n%10+ (char) '0');
	strcat(file,"cards/");
	strcat(file,num);
	strcat(file,".gif");
}
	

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
	
	p2->nbCardsInHand=0;
	p2->score_player.gainedCards=0;
	p2->score_player.points=0;
	p2->type=COMPUTER;
}	

void distributeTable(card_num cardList[],card table[]) {
	shortint i,k=0,j=1;
	table[0].number=cardList[nbCardsRemaining-1];
    char file[13]="";
    getFile(table[0].number,file);
    table[0].surf=IMG_Load(file);
    table[0].position=malloc(sizeof(SDL_Rect));
    table[0].position->x=40;
    table[0].position->y=165;
    
    for (i=1; i<MAX_NB_CARDS_TABLE ; i++) {
		if ((i==2) || (i==4) || (i==6)) {
			while (exist(table, i, cardList[nbCardsRemaining-j-1])) {
				swap(&cardList[nbCardsRemaining-j-1], &cardList[nbCardsRemaining-j-k-1]);
				k++;
			}
			file[0]='\0';
			table[i].number=cardList[nbCardsRemaining-j-1];
			getFile(table[i].number,file);
			table[i].surf=IMG_Load(file);
			table[i].position=malloc(sizeof(SDL_Rect));
			switch (i) {
				case 2: 
					table[i].position->x=250;
					table[i].position->y=165;
				break;
			
				case 4: 
					table[i].position->x=460;
					table[i].position->y=165;
				break;
			
				case 6: 
					table[i].position->x=145;
					table[i].position->y=300;
				break;
			}
			j++;
		}  
	}	
    nbCardsRemaining -= j;
}

void distributePlayer(card_num cardList[],player *pl) {
    shortint i;
    for(i=0; i<MAX_NB_CARDS_HAND;i++) {
        pl->hand[i].number=cardList[nbCardsRemaining-1-i];
        pl->hand[i].position=malloc(sizeof(SDL_Rect));
        if (pl->type == USER) {
			char file[13]="";
			getFile(pl->hand[i].number,file);		
			pl->hand[i].surf=IMG_Load(file);
			switch (i) {
				case 0:
					pl->hand[i].position->x=150;
					pl->hand[i].position->y=450;
				break;
				case 1:
					pl->hand[i].position->x=280;
					pl->hand[i].position->y=450;
				break;
					
				case 2:
					pl->hand[i].position->x=410;
					pl->hand[i].position->y=450;
				break;
			}
		}
		else {
			pl->hand[i].surf=IMG_Load("cards/back.gif");
			switch (i) {
				case 0:
					pl->hand[i].position->x=150;
					pl->hand[i].position->y=15;
				break;
				
				case 1:
					pl->hand[i].position->x=280;
					pl->hand[i].position->y=15;
				break;
				
				case 2:
					pl->hand[i].position->x=410;
					pl->hand[i].position->y=15;
				break;
			}
		}
        pl->nbCardsInHand++;
    }
    nbCardsRemaining -= i;
}
