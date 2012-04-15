#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "random.h"
#include "functions.h"

int main(int argc,char** argv) {
	int running=1,i;
	player user,computer;
	card table[MAX_NB_CARDS_TABLE],selected[11];
	card_num cardList[NB_CARDS]={ARGENT1,ARGENT2,ARGENT3,ARGENT4,ARGENT5,ARGENT6,ARGENT7,ARGENT10,ARGENT11,ARGENT12,
                    BATON1,BATON2,BATON3,BATON4,BATON5,BATON6,BATON7,BATON10,BATON11,BATON12,
                    EPEE1,EPEE2,EPEE3,EPEE4,EPEE5,EPEE6,EPEE7,EPEE10,EPEE11,EPEE12,
                    COUPE1,COUPE2,COUPE3,COUPE4,COUPE5,COUPE6,COUPE7,COUPE10,COUPE11,COUPE12};
    unsigned short int nbCardsRemaining=NB_CARDS,droppedCard=50;                
	SDL_Event event;
	SDL_Surface *screen=NULL;
	
	//Initialisation
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	init(table,&user,&computer);
	
	//SDL_WM_SetIcon(icon, NULL);
	screen=SDL_SetVideoMode(440, 460, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 30, 170, 45));
		
	//mixing the cards
	mix(cardList, NB_CARDS);
	
	//distributing cards
	distributeTable(cardList, table, &nbCardsRemaining);
	distributePlayer(cardList, &user, &nbCardsRemaining);
	distributePlayer(cardList, &computer, &nbCardsRemaining);
	
	while (running) {
		while(SDL_PollEvent(&event))	{
			switch (event.type) {
				case SDL_QUIT:
					running=0;
				break;	
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running=0;
						break;
						default:
						
						break;
					}	
				break;
		
				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button) {
						default:
				
						break;
					}
				break; 
				
				default:
				
				break;
			}
		
		}
		for (i=0; i<MAX_NB_CARDS_TABLE;i++) 
			if (table[i].number != EMPTY)
				if (SDL_BlitSurface(table[i].surf, NULL, screen, table[i].position) == -1) return 1;
				
		for (i=0; i<user.nbCardsInHand;i++)
			if (SDL_BlitSurface(user.hand[i].surf, NULL, screen, user.hand[i].position) == -1) return 1; 
		SDL_Flip(screen);	
	}	
	
	for (i=0;i<MAX_NB_CARDS_TABLE;i++) {
		if(table[i].surf != NULL) SDL_FreeSurface(table[i].surf);
		if(table[i].position != NULL) free(table[i].position);
	}
	
	for (i=0; i<user.nbCardsInHand;i++) {
		if(user.hand[i].surf != NULL)	SDL_FreeSurface(user.hand[i].surf); 
		if(user.hand[i].position != NULL) free(user.hand[i].position);
	}
	
	SDL_Quit(); 	
	return 0;
}
