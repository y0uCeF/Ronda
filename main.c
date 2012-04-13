#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "define.h"
#include "random.h"

int main(int argc,char** argv) {
	int running=1;
	player user,computer;
	card* table[MAX_NB_CARDS_TABLE],selected[11];
	card_num cardList[NB_CARDS]={ARGENT1,ARGENT2,ARGENT3,ARGENT4,ARGENT5,ARGENT6,ARGENT7,ARGENT10,ARGENT11,ARGENT12,
                    BATON1,BATON2,BATON3,BATON4,BATON5,BATON6,BATON7,BATON10,BATON11,BATON12,
                    EPEE1,EPEE2,EPEE3,EPEE4,EPEE5,EPEE6,EPEE7,EPEE10,EPEE11,EPEE12,
                    COUPE1,COUPE2,COUPE3,COUPE4,COUPE5,COUPE6,COUPE7,COUPE10,COUPE11,COUPE12};
    int nbCardsInTable=0, nbCardsRemaining=NB_CARDS, droppedCard=50;                
	SDL_Event event;
	SDL_Surface *screen=NULL;
	
	srand(time(NULL));
	
	//mixing the cards
	mix(cardList, NB_CARDS);
	
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
			}
		
		}
		
		//loop();
	}	
	
	
	return 0;
}
