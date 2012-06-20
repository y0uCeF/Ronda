#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "random.h"
#include "functions.h"

int main(int argc,char** argv) {
	shortint running=1;
	player user,computer;
	card table[MAX_NB_CARDS_TABLE];
	card_num cardList[NB_CARDS]={COIN1,COIN2,COIN3,COIN4,COIN5,COIN6,COIN7,COIN10,COIN11,COIN12,
                    CLUB1, CLUB2, CLUB3, CLUB4, CLUB5, CLUB6, CLUB7, CLUB10, CLUB11, CLUB12,
                    SWORD1, SWORD2, SWORD3, SWORD4, SWORD5, SWORD6, SWORD7, SWORD10, SWORD11, SWORD12,
                    CUP1, CUP2, CUP3, CUP4, CUP5, CUP6, CUP7, CUP10, CUP11, CUP12};
    short selectedTable=-1 , selectedHand=-1;
	extern shortint nbCardsRemaining;          
	extern Type currentPlayer;
	extern card_num droppedCard;
	
	
	SDL_Event event;
	SDL_Surface *screen=NULL;  // *selectionHand=NULL,
	int currentTime=0, oldTime=0, timeDiff;
	
	
	//Initialisation
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
       fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
       return 1;
    }
	init(table,&user,&computer);
	
	

	//SDL_WM_SetIcon(icon, NULL);
	putenv("SDL_VIDEO_CENTERED=1");
	screen=SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (screen == NULL) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		return 1;
	}
	if (SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68)) == -1) {
		fprintf(stderr, "Unable to fill screen: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WM_SetCaption("Ronda - alpha", NULL);
		
	//mixing the cards
	mix(cardList, NB_CARDS);
	
	//distributing cards
	if(distributeTable(cardList, table) == 0) return 1;
	if(distributePlayer(cardList, &user) == 0) return 1;
	if(distributePlayer(cardList, &computer) == 0) return 1;

	/*for(i=0; i<3;i++) printf("%d  ", user.hand[i].number);
	printf("\n");
	for(i=0; i<10;i++) printf("%d  ", table[i].number);*/
	
	while (running) {
		while(SDL_PollEvent(&event))	{
			treatEvent(event, &running, &user, &selectedHand, &selectedTable);		
		}
		
		//setting fps to 30 
		currentTime= SDL_GetTicks();
		if ((timeDiff=currentTime - oldTime) > FRAME_RATE ) {
			oldTime=currentTime;
		}
		else {
			SDL_Delay(FRAME_RATE - timeDiff);
		}
		
		//
		if ((selectedHand != -1) && (selectedTable != -1)) {					
			play(&user, table, selectedHand, selectedTable);
			//currentPlayer = COMPUTER;
			selectedHand = -1;
			selectedTable = -1;
		}

		
		if (!render(table, user, computer, screen)) return 1;
		
		SDL_Flip(screen);	
	}	
	
	//freeing memory
	delete(table, user, computer);
	
	SDL_Quit(); 	
	return 0;
}
