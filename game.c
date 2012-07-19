#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "basic.h"
#include "player.h"
#include "play.h"



static bool table_distribute(card_num cardList[],card table[], 
			unsigned short *nb_cards_remaining) 
{
	unsigned short i, k = 0, j = 0;
	unsigned short xpos, ypos;
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		while (exist(table, i, cardList[*nb_cards_remaining-j-1]) != -1) {
			swap(&cardList[*nb_cards_remaining-j-1], &cardList[*nb_cards_remaining-j-k-1]);
			k++;
		}
		xpos = (90+15)*(i%5) + 40;
		ypos = (i<=4)? 160:305;
		if ((i==0) || (i==2) || (i==4) || (i==6)) {
			set_card(&table[i], cardList[*nb_cards_remaining-j-1], xpos, ypos, 0);
			j++;
		} else {
			set_card(&table[i], EMPTY, xpos, ypos, 0);
		}		
	}	
    *nb_cards_remaining -= j; 
    return 1;
}

static bool env_init(game_t *g) 
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		return sdl_error("Unable to init SDL");
	putenv("SDL_VIDEO_CENTERED=1");
	g->screen=SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  SDL_DOUBLEBUF | SDL_HWSURFACE );
	if (g->screen == NULL) 
		return sdl_error("Video initialization failed");
	if (SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68)) == -1) 
		return sdl_error("Setting screen failed");
		
	SDL_WM_SetCaption("Ronda - alpha", NULL);
	return 1;
}

game_t* game_init() 
{
	unsigned short i;
	game_t *g = malloc(sizeof(game_t));
	if (!env_init(g)) 
		return NULL;
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		g->table[i].number=EMPTY;
		g->table[i].surf=NULL;
		g->table[i].position=NULL;
	}
	
	for(i=0; i < NB_CARDS; i++) 
		g->cardList[i] = i;
	
	g->running = 1;
	g->user = player_init(USER);
	g->comp = player_init(COMPUTER);
	g->selectedHand = -1;
	g->selectedTable = -1;
	g->nb_cards_remaining = NB_CARDS; 
	g->currentPlayer = USER;
	g->droppedCard = EMPTY;
	
	/*mixing the cards*/
	mix(g->cardList, NB_CARDS);
	
	/*distributing cards*/
	if(!table_distribute(g->cardList, g->table, &g->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(g->cardList, g->user, &g->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(g->cardList, g->comp, &g->nb_cards_remaining)) 
		return NULL;
	
	return g;
}	

static bool game_render(game_t *g) 
{
	int i;
	SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68));
	
	if (!player_render(g->user, g->screen)) 
		return 0;
	if (!player_render(g->comp, g->screen))
		return 0;			
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) 
		if (SDL_BlitSurface(g->table[i].surf, NULL, g->screen, g->table[i].position) == -1) 
			return 0;
	
	/*	if (selectionHand != NULL)	
			if(SDL_BlitSurface(selectionHand, NULL, g->screen, g->user.hand[selectedHand].position) == -1) 
			 	return 0;
	*/
	SDL_Flip(g->screen);
	return 1;	
}

void game_free(game_t *g) 
{
	unsigned short i;
	for (i=0;i < MAX_NB_CARDS_TABLE;i++) {
		if(g->table[i].surf != NULL) 
			SDL_FreeSurface(g->table[i].surf);
		if(g->table[i].position != NULL) 
			free(g->table[i].position);
	}
	
	player_free(g->user);
	player_free(g->comp);
	
	
	/*if (selectionHand != NULL) SDL_FreeSurface(selectionHand);*/
	free(g);
	SDL_Quit();
}

static void treat_keyboard_event(SDL_Event event, unsigned short *running)
{
	switch(event.key.keysym.sym) {
		case SDLK_ESCAPE:
			*running = 0;
		break;
			
		case SDLK_RETURN:
			*running = 0;
		break;
					
		default:
			
		break;
	}	
}

static void treat_mouse_down_event(game_t *g)
{
	switch(g->event.button.button) {
	case SDL_BUTTON_LEFT:
		if (g->selectedHand != -1) 
			g->user->hand[g->selectedHand].position->y = 450;	
			
		if (between(g->event.button.y, 450, 585)) {						
			g->selectedHand = get_selected_hand(g->event.button.x);
			if ((g->selectedHand != -1) && (get_card(*g->user, g->selectedHand) != EMPTY)) {		       
				/*selectionHand=IMG_Load("cards/selection.png"); */
				g->user->hand[g->selectedHand].position->y = 447;
			}
			g->selectedTable = -1;
		} else if (((between(g->event.button.y, 160, 295)) || (between(g->event.button.y, 305, 440)))
				&& (g->selectedHand != -1))  {
			g->selectedTable = get_selected_table(g->event.button.x, g->event.button.y);
		}
	break;
						
	default:
	break;
	}
}

static void game_treat_event(game_t *g) 
{
	switch (g->event.type) {
	case SDL_QUIT:
		g->running=0;
	break;	
	
	case SDL_KEYDOWN:
		treat_keyboard_event(g->event, &g->running);
	break;
	
	case SDL_MOUSEBUTTONUP:
		if (g->currentPlayer == USER)
			treat_mouse_down_event(g);
	break;
			
	case SDL_MOUSEMOTION:
				
	break;
				
	default:
		
	break;
	}
}

bool game_run(game_t *g) 
{
	int currentTime = 0, oldTime = 0, timeDiff;
	
	while (g->running) {
		while(SDL_PollEvent(&g->event))	{
			game_treat_event(g);		
		}
		
		/*setting fps to 30 */
		currentTime = SDL_GetTicks();
		if ((timeDiff = currentTime - oldTime) > FRAME_RATE ) 
			oldTime = currentTime;
		else 
			SDL_Delay(FRAME_RATE - timeDiff);
		
		/*if (g->currentPlayer == COMPUTER) 
			computer_turn(g->comp, g->table, &g->droppedCard);
		else if (g->currentPlayer == USER) 
			if ((g->selectedHand != -1) && (g->selectedTable != -1))
				user_turn(g->user, g->table, g->selectedHand, 
						g->selectedTable, &g->droppedCard);
		g->currentPlayer = (g->currentPlayer == USER)? COMPUTER : USER; 
		g->selectedHand = -1;
		g->selectedTable = -1;*/
		if ((g->selectedHand != -1) && (g->selectedTable != -1) &&
			(g->currentPlayer == USER)){
			user_turn(g->user, g->table, g->selectedHand, 
				g->selectedTable, &g->droppedCard);
			g->currentPlayer = COMPUTER;	
			g->selectedHand = -1;
			g->selectedTable = -1;
		}
		else if (g->currentPlayer == COMPUTER) {
			SDL_Delay(2000);
			computer_turn(g->comp, g->table, &g->droppedCard);
			g->currentPlayer = USER;
		}
		
		/*rendering*/
		if (!game_render(g)) return 0;	
	}		
	return 1; 		
}
