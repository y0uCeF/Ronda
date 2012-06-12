#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "random.h"
#include "functions.h"

int main(int argc,char** argv) {
	int running=1,i;
	player user,computer;
	enum {USER, COMPUTER} currentPlayer=USER;
	card table[MAX_NB_CARDS_TABLE];
	card_num cardList[NB_CARDS]={COIN1,COIN2,COIN3,COIN4,COIN5,COIN6,COIN7,COIN10,COIN11,COIN12,
                    CLUB1, CLUB2, CLUB3, CLUB4, CLUB5, CLUB6, CLUB7, CLUB10, CLUB11, CLUB12,
                    SWORD1, SWORD2, SWORD3, SWORD4, SWORD5, SWORD6, SWORD7, SWORD10, SWORD11, SWORD12,
                    CUP1, CUP2, CUP3, CUP4, CUP5, CUP6, CUP7, CUP10, CUP11, CUP12};
    short selectedTable=-1 , selectedHand=-1;
    shortint droppedCard=50;      
    extern shortint nbCardsRemaining;          
	SDL_Event event;
	SDL_Surface *screen=NULL, *selectionHand=NULL, *scoreBoxUser=NULL, *scoreBoxComputer=NULL;
	SDL_Rect posScoreBoxUser, posScoreBoxComputer;
	int currentTime=0, oldTime=0, timeDiff;
	
	
	//Initialisation
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	init(table,&user,&computer);
	posScoreBoxUser.x=600;
	posScoreBoxUser.y=40;
	posScoreBoxComputer.x=600;
	posScoreBoxComputer.y=470;	
	scoreBoxUser=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	scoreBoxComputer=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	

	//SDL_WM_SetIcon(icon, NULL);
	putenv("SDL_VIDEO_CENTERED=1");
	screen=SDL_SetVideoMode(800, 600, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68));
	SDL_WM_SetCaption("Ronda - alpha", NULL);
		
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
		
				case SDL_MOUSEBUTTONUP:
					if (currentPlayer == USER)
						switch(event.button.button) {
							case SDL_BUTTON_LEFT:
								if ((event.button.y> 450) && (event.button.y < 585)) {
								if (selectedHand != -1)
									user.hand[selectedHand].position->y = 450;	
								selectedHand = getSelectedHand(event.button.x);
								if(selectedHand != -1) //selectionHand=IMG_Load("cards/selection.png"); 
									user.hand[selectedHand].position->y = 447;
								selectedTable = -1;
								}
								else if ((((event.button.y>160) && (event.button.y<295)) || ((event.button.y>305) && (event.button.y<440)))
									&& (selectedHand != -1))  {
									selectedTable = getSelectedTable(event.button.x, event.button.y);
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
		
		//setting fps to 30 - time optimisation
		currentTime= SDL_GetTicks();
		if ((timeDiff=currentTime - oldTime) > 30 ) {
			oldTime=currentTime;
		}
		else {
			SDL_Delay(30 - timeDiff);
		}
		
		/*if (selectedHand != -1) printf("hand:%d.", selectedHand);
		if (selectedTable != -1) printf("table:%d", selectedTable);
		*/
		if ((selectedHand != -1) && (selectedTable != -1)) {
			
		}
		
		
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68));
		SDL_FillRect(scoreBoxUser, NULL, SDL_MapRGB(scoreBoxUser->format, 35, 50, 20));
		SDL_FillRect(scoreBoxComputer, NULL, SDL_MapRGB(scoreBoxComputer->format, 35, 50, 20));
		//bliting cards
		
		for (i=0; i<MAX_NB_CARDS_TABLE;i++) 
			if (SDL_BlitSurface(table[i].surf, NULL, screen, table[i].position) == -1) return 1;
				
		for (i=0; i<user.nbCardsInHand;i++)
			if (SDL_BlitSurface(user.hand[i].surf, NULL, screen, user.hand[i].position) == -1) return 1; 
			
		for (i=0; i<computer.nbCardsInHand;i++)
			if (SDL_BlitSurface(computer.hand[i].surf, NULL, screen, computer.hand[i].position) == -1) return 1;
				
		SDL_BlitSurface(scoreBoxUser, NULL, screen, &posScoreBoxUser);
		SDL_BlitSurface(scoreBoxComputer, NULL, screen, &posScoreBoxComputer);
	
	/*	if (selectionHand != NULL)	{
			if(SDL_BlitSurface(selectionHand, NULL, screen, user.hand[selectedHand].position) == -1) return 1;
		}*/
		
		SDL_Flip(screen);	
	}	
	
	
	//freeing memory
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
	SDL_FreeSurface(scoreBoxUser);
	SDL_FreeSurface(scoreBoxComputer);
	//if (selectionHand != NULL) SDL_FreeSurface(selectionHand);
	SDL_Quit(); 	
	return 0;
}
