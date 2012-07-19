#include <time.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "game.h"
#include "basic.h"
#include "player.h"
#include "play.h"



static bool table_distribute(card_num card_list[],card table[], 
			unsigned short *nb_cards_remaining) 
{
	unsigned short i, k = 0, j = 0;    /* itérators */
	unsigned short xpos, ypos;    /* x, y positions */
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		while (exist(table, i, card_list[*nb_cards_remaining-j-1]) != -1) {
			swap(&card_list[*nb_cards_remaining-j-1], &card_list[*nb_cards_remaining-j-k-1]);
			k++;
		}
		xpos = (90+15)*(i%5) + 40;
		ypos = (i<=4)? 160:305;
		if ((i==0) || (i==2) || (i==4) || (i==6)) {
			set_card(&table[i], card_list[*nb_cards_remaining-j-1], xpos, ypos, 0);
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
	g->screen=SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32,  
				SDL_DOUBLEBUF | SDL_HWSURFACE );
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
		g->table[i].value=EMPTY;
		g->table[i].surf=NULL;
		g->table[i].position=NULL;
	}
	
	for(i=0; i < NB_CARDS; i++) 
		g->card_list[i] = i;
	
	g->running = 1;
	g->user = player_init(USER);
	g->comp = player_init(COMPUTER);
	g->sel_hand = -1;
	g->sel_table = -1;
	g->nb_cards_remaining = NB_CARDS; 
	g->current_player = USER;
	g->dropped_card = EMPTY;
	
	/*mixing the cards*/
	mix(g->card_list, NB_CARDS);
	
	/*distributing cards*/
	if(!table_distribute(g->card_list, g->table, &g->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(g->card_list, g->user, &g->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(g->card_list, g->comp, &g->nb_cards_remaining)) 
		return NULL;
	
	return g;
}	

static bool game_render(game_t *g) 
{
	unsigned short i;
	SDL_FillRect(g->screen, NULL, SDL_MapRGB(g->screen->format, 53, 131, 68));
	
	if (!player_render(g->user, g->screen)) 
		return 0;
	if (!player_render(g->comp, g->screen))
		return 0;			
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) 
		if (SDL_BlitSurface(g->table[i].surf, NULL, g->screen, g->table[i].position) == -1) 
			return 0;
	
	/*	if (selectionHand != NULL)	
			if(SDL_BlitSurface(selectionHand, NULL, g->screen, g->user.hand[sel_hand].position) == -1) 
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
		if (g->sel_hand != -1) 
			g->user->hand[g->sel_hand].position->y = 450;	
			
		if (between(g->event.button.y, 450, 585)) {						
			g->sel_hand = get_selected_hand(g->event.button.x);
			if ((g->sel_hand != -1) && (get_card_val(*g->user, g->sel_hand) != EMPTY)) {		       
				/*selectionHand=IMG_Load("cards/selection.png"); */
				g->user->hand[g->sel_hand].position->y = 447;
			}
			g->sel_table = -1;
		} else if (((between(g->event.button.y, 160, 295)) || (between(g->event.button.y, 305, 440)))
				&& (g->sel_hand != -1))  {
			g->sel_table = get_selected_table(g->event.button.x, g->event.button.y);
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
		if (g->current_player == USER)
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
	int currentTime = 0, oldTime = 0, 
		timeDiff; /* time difference calculations */
	
	/* main loop */
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
		
		
		if ((g->sel_hand != -1) && (g->sel_table != -1) &&
			(g->current_player == USER)){
			user_turn(g->user, g->table, g->sel_hand, 
				g->sel_table, &g->dropped_card);
			g->current_player = COMPUTER;	
			g->sel_hand = -1;
			g->sel_table = -1;
		}
		else if (g->current_player == COMPUTER) {
			SDL_Delay(2000);
			computer_turn(g->comp, g->table, &g->dropped_card);
			g->current_player = USER;
		}
		
		/*rendering*/
		if (!game_render(g)) 
			return 0;	
	}		
	return 1; 		
}
