#include "functions.h"



void swap(card_num *a, card_num *b) {
    int tmp = *a;
    *a= *b;
    *b = tmp;
}

unsigned short int equal(int a, int b) {
    return ((a%10) == (b%10));
}

unsigned short int exist(card tab[], int size, int value)
{
    int i;
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
	strcat(file,".bmp");
}
	

void init(card table[],player *p1, player *p2) {
	int i;
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

void distributeTable(card_num cardList[],card table[], unsigned short int* nbCardsRemaining) {
	int i,k=0,j=1;
	table[0].number=cardList[*nbCardsRemaining-1];
    char file[13]="";
    getFile(table[0].number,file);
    table[0].surf=SDL_LoadBMP(file);
    table[0].position=malloc(sizeof(SDL_Rect));
    table[0].position->x=20;
    table[0].position->y=134;
    
    for (i=1; i<MAX_NB_CARDS_TABLE ; i++) {
		if ((i==2) || (i==4) || (i==6)) {
			while (exist(table, j, cardList[*nbCardsRemaining-j-1])) {
				swap(&cardList[*nbCardsRemaining-j-1], &cardList[*nbCardsRemaining-j-k-1]);
				k++;
			}
			file[0]='\0';
			table[i].number=cardList[*nbCardsRemaining-j-1];
			getFile(table[i].number,file);
			table[i].surf=SDL_LoadBMP(file);
			table[i].position=malloc(sizeof(SDL_Rect));
			switch (i) {
				case 2: 
					table[i].position->x=160;
					table[i].position->y=134;
				break;
			
				case 4: 
					table[i].position->x=300;
					table[i].position->y=134;
				break;
			
				case 6: 
					table[i].position->x=90;
					table[i].position->y=230;
				break;
			}
			j++;
		}  
	}	
    *nbCardsRemaining -= j;
}

void distributePlayer(card_num cardList[],player *pl, unsigned short int* nbCardsRemaining) {
    int i;
    for(i=0; i<MAX_NB_CARDS_HAND;i++) {
        pl->hand[i].number=cardList[*nbCardsRemaining-1-i];
        
        if (pl->type == USER) {
			char file[13]="";
			getFile(pl->hand[i].number,file);
			pl->hand[i].position=malloc(sizeof(SDL_Rect));
			pl->hand[i].surf=SDL_LoadBMP(file);
			switch (i) {
				case 0:
					pl->hand[i].position->x=80;
					pl->hand[i].position->y=346;
				break;
				case 1:
					pl->hand[i].position->x=180;
					pl->hand[i].position->y=346;
				break;
					
				case 2:
					pl->hand[i].position->x=280;
					pl->hand[i].position->y=346;
				break;
			}
		}
        pl->nbCardsInHand++;
    }
    *nbCardsRemaining -= i;
}
