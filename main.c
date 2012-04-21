#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "random.h"
#include "functions.h"

int main(int argc,char** argv) {
	int running=1,i;
	player user,computer;
	card table[MAX_NB_CARDS_TABLE],selected[11];
	card_num cardList[NB_CARDS]={COIN1,COIN2,COIN3,COIN4,COIN5,COIN6,COIN7,COIN10,COIN11,COIN12,
                    CLUB1, CLUB2, CLUB3, CLUB4, CLUB5, CLUB6, CLUB7, CLUB10, CLUB11, CLUB12,
                    SWORD1, SWORD2, SWORD3, SWORD4, SWORD5, SWORD6, SWORD7, SWORD10, SWORD11, SWORD12,
                    CUP1, CUP2, CUP3, CUP4, CUP5, CUP6, CUP7, CUP10, CUP11, CUP12};
    shortint droppedCard=50;      
    extern shortint nbCardsRemaining;          
	SDL_Event event;
	SDL_Surface *screen=NULL;
	
	//Initialisation
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	init(table,&user,&computer);
	
	//SDL_WM_SetIcon(icon, NULL);
	screen=SDL_SetVideoMode(800, 600, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 30, 170, 45));
	SDL_WM_SetCaption("Ronda - beta", NULL);
		
	//mixing the cards
	mix(cardList, NB_CARDS);
	
	//distributing cards
	distributeTable(cardList, table);
	distributePlayer(cardList, &user);
	distributePlayer(cardList, &computer);
	
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
			
		for (i=0; i<computer.nbCardsInHand;i++)
			if (SDL_BlitSurface(computer.hand[i].surf, NULL, screen, computer.hand[i].position) == -1) return 1;	
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
	
	for (i=0; i<computer.nbCardsInHand;i++) {
		if(computer.hand[i].surf != NULL)	SDL_FreeSurface(computer.hand[i].surf); 
		if(computer.hand[i].position != NULL) free(computer.hand[i].position);
	}
	
	SDL_Quit(); 	
	return 0;
}
