#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "basic.h"
#include "player.h"
#include "play.h"



static bool table_distribute(card_num cardList[],card table[], shortint* nbCardsRemaining) {
	shortint i,k=0,j=1;
	table[0].number=cardList[*nbCardsRemaining-1];
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
		while (exist(table, i, cardList[*nbCardsRemaining-j-1]) != -1) {
			swap(&cardList[*nbCardsRemaining-j-1], &cardList[*nbCardsRemaining-j-k-1]);
			k++;
		}
		file[0]='\0';
		if ((i==2) || (i==4) || (i==6)) {
		table[i].number=cardList[*nbCardsRemaining-j-1];
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
    *nbCardsRemaining -= j; 
    return 1;
}


static bool env_init(game_t *g) {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
       fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
       return 0;
    }
    putenv("SDL_VIDEO_CENTERED=1");
    g->screen=SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (g->screen == NULL) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		return 0;
	}
	if (SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68)) == -1) {
		fprintf(stderr, "Unable to fill screen: %s\n", SDL_GetError());
		return 0;
	}
	SDL_WM_SetCaption("Ronda - alpha", NULL);
	return 1;
}


game_t* game_init() {
	shortint i;
	game_t *g=malloc(sizeof(game_t));
	if (!env_init(g)) return NULL;
	for (i=0; i<MAX_NB_CARDS_TABLE; i++) {
		g->table[i].number=EMPTY;
		g->table[i].surf=NULL;
		g->table[i].position=NULL;
	}
	for(i=0; i<NB_CARDS; i++) {
		g->cardList[i]=i;
	}
	g->running=1;
	g->user=player_init(USER);
	g->comp=player_init(COMPUTER);
	g->selectedHand=-1;
	g->selectedTable=-1;
	g->nbCardsRemaining=NB_CARDS; 
	g->currentPlayer=USER;
	g->droppedCard=EMPTY;
	
	//mixing the cards
	mix(g->cardList, NB_CARDS);
	
	//distributing cards
	if(!table_distribute(g->cardList, g->table, &g->nbCardsRemaining)) return NULL;
	if(!player_distribute(g->cardList, g->user, &g->nbCardsRemaining)) return NULL;
	if(!player_distribute(g->cardList, g->comp, &g->nbCardsRemaining)) return NULL;
	
	return g;
}	


static bool game_render(game_t *g) {
	int i;
	SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68));
	SDL_FillRect(g->user->scoreBox, NULL, SDL_MapRGB(g->user->scoreBox->format, 35, 50, 20));
	SDL_FillRect(g->comp->scoreBox, NULL, SDL_MapRGB(g->comp->scoreBox->format, 35, 50, 20));
	//bliting cards
	
	for (i=0; i<MAX_NB_CARDS_TABLE;i++) 
		if (SDL_BlitSurface(g->table[i].surf, NULL, g->screen, g->table[i].position) == -1) return 0;
			
	for (i=0; i<g->user->nbCardsInHand;i++)
		if (SDL_BlitSurface(g->user->hand[i].surf, NULL, g->screen, g->user->hand[i].position) == -1) return 0; 
		
	for (i=0; i<g->comp->nbCardsInHand;i++)
		if (SDL_BlitSurface(g->comp->hand[i].surf, NULL, g->screen, g->comp->hand[i].position) == -1) return 0;
				
	SDL_BlitSurface(g->user->scoreBox, NULL, g->screen, &g->user->posScoreBox);
	SDL_BlitSurface(g->comp->scoreBox, NULL, g->screen, &g->comp->posScoreBox);
	/*	if (selectionHand != NULL)	{
			if(SDL_BlitSurface(selectionHand, NULL, g->screen, g->user.hand[selectedHand].position) == -1) return 1;
		}*/
	return 1;	
}

void game_free(game_t *g) {
	int i;
	for (i=0;i< MAX_NB_CARDS_TABLE;i++) {
		if(g->table[i].surf != NULL) SDL_FreeSurface(g->table[i].surf);
		if(g->table[i].position != NULL) free(g->table[i].position);
	}
	
	for (i=0; i< g->user->nbCardsInHand;i++) {
		if(g->user->hand[i].surf != NULL)	SDL_FreeSurface(g->user->hand[i].surf); 
		if(g->user->hand[i].position != NULL) free(g->user->hand[i].position);
	}
	
	for (i=0; i< g->comp->nbCardsInHand;i++) {
		if(g->comp->hand[i].surf != NULL)	SDL_FreeSurface(g->comp->hand[i].surf); 
		if(g->comp->hand[i].position != NULL) free(g->comp->hand[i].position);
	}
	if(g->user->scoreBox != NULL) SDL_FreeSurface(g->user->scoreBox);
	if(g->comp->scoreBox != NULL) SDL_FreeSurface(g->comp->scoreBox);
	//if (selectionHand != NULL) SDL_FreeSurface(selectionHand);
	
	free(g->user);
	free(g->comp);
	free(g);
	SDL_Quit();
}


static void game_treatEvent(game_t *g) {
	switch (g->event.type) {
		case SDL_QUIT:
			g->running=0;
		break;	
		case SDL_KEYDOWN:
			switch(g->event.key.keysym.sym) {
				case SDLK_ESCAPE:
					g->running=0;
				break;
				
				case SDLK_RETURN:
					g->running=0;
				break;
					
				default:
				
				break;
			}	
		break;
		
		case SDL_MOUSEBUTTONUP:
			if (g->currentPlayer == USER)
				switch(g->event.button.button) {
					case SDL_BUTTON_LEFT:
						if (g->selectedHand != -1) {
							g->user->hand[g->selectedHand].position->y = 450;	
						}
						if (between(g->event.button.y, 450, 585)) {								
						g->selectedHand = getSelectedHand(g->event.button.x);
						if ((g->selectedHand != -1) && (g->user->hand[g->selectedHand].number != EMPTY)) {		//selectionHand=IMG_Load("cards/selection.png"); 
						g->user->hand[g->selectedHand].position->y = 447;
						}
						g->selectedTable = -1;
						}
						else if (((between(g->event.button.y, 160, 295)) || (between(g->event.button.y, 305, 440)))
							&& (g->selectedHand != -1))  {
							g->selectedTable = getSelectedTable(g->event.button.x, g->event.button.y);
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


bool game_run(game_t *g) {
	int currentTime=0, oldTime=0, timeDiff;
	
	/*for(i=0; i<3;i++) printf("%d  ", user.hand[i].number);
	printf("\n");
	for(i=0; i<10;i++) printf("%d  ", table[i].number);*/
	
	while (g->running) {
		while(SDL_PollEvent(&g->event))	{
			game_treatEvent(g);		
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
		if ((g->selectedHand != -1) && (g->selectedTable != -1)) {					
			play(g->user, g->table, g->selectedHand, g->selectedTable, &g->droppedCard);
			//currentPlayer = COMPUTER;
			g->selectedHand = -1;
			g->selectedTable = -1;
		}

		//rendering
		if (!game_render(g)) return 0;
		
		SDL_Flip(g->screen);	
	}	
	
	return 1; 		
}
