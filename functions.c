
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
        if (equal(tab[i].number, value)) return 1;
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
    table[0].position->y=160;
    
    for (i=1; i<MAX_NB_CARDS_TABLE ; i++) {
		while (exist(table, i, cardList[nbCardsRemaining-j-1])) {
			swap(&cardList[nbCardsRemaining-j-1], &cardList[nbCardsRemaining-j-k-1]);
			k++;
		}
		if ((i==2) || (i==4) || (i==6)) {
		file[0]='\0';
		table[i].number=cardList[nbCardsRemaining-j-1];
		getFile(table[i].number,file);
		table[i].surf=IMG_Load(file);
		}
		else {
			table[i].number=EMPTY;
			table[i].surf=IMG_Load("cards/blank.gif");	
		}
		table[i].position=malloc(sizeof(SDL_Rect));
		table[i].position->x= (90+15)*(i%5) + 40;
		table[i].position->y= (i<=4)? 160:305; 
		j++;
	}	
    nbCardsRemaining -= j;
}

void distributePlayer(card_num cardList[],player *pl) {
    shortint i;
    for(i=0; i<MAX_NB_CARDS_HAND;i++) {
        pl->hand[i].number=cardList[nbCardsRemaining-1-i];
        pl->hand[i].position=malloc(sizeof(SDL_Rect));
        pl->hand[i].position->x=(90+40)*i+150;
        if (pl->type == USER) {
			char file[13]="";
			getFile(pl->hand[i].number,file);		
			pl->hand[i].surf=IMG_Load(file);		
			pl->hand[i].position->y=450;
		}
		else {
			pl->hand[i].surf=IMG_Load("cards/back.gif");
			pl->hand[i].position->y=15;
		}
        pl->nbCardsInHand++;
    }
    nbCardsRemaining -= i;
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
