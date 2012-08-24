#include <SDL/SDL_image.h>
#include "main_game.h"
#include "common.h"
#include "play.h"

extern SDL_Surface *empty_card;
extern SDL_Surface *back_card;

static bool table_distribute(card_num card_list[],card table[], 
			unsigned short *nb_cards_remaining) 
{
	unsigned short i, k = 0, j = 0;    /* iterators */
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

static bool valid_card_hand(int x, int y)
{
	bool res = 0;
	if (between(y, 450, 585))
		res = between(x,150, 240) || between(x, 280,370) ||
			between(x,410, 500);	
	return res;
}

static bool valid_card_table(int x, int y)
{
	bool res = 0;
	if (between(y, 160, 295) || between(y, 305, 440))
		res = between(x, 40, 130) || between(x, 145, 235) ||
		between(x, 250, 340) || between(x, 355, 445) ||
		between(x, 460, 550); 
	return res;
}

/*
 * returns index of the selected card in the screen (user hand)
 */ 
static short get_selected_hand(int x) 
{
	if (between(x,150, 240)) 
		return 0;
	else if (between(x, 280,370)) 
		return 1;
	else if (between(x,410, 500)) 
		return 2;
	else return -1;
}

/*
 * returns index of the selected card in the screen (table)
 */ 
static short get_selected_table(int x, int y) 
{
	short temp=-1, c=0;
	if (between(x, 40, 130)) 
		temp=0;
	else if(between(x, 145, 235)) 
		temp=1;
	else if (between(x, 250, 340)) 
		temp=2;
	else if(between(x, 355, 445)) 
		temp=3;
	else if (between(x, 460, 550)) 
		temp=4;
	
	if (temp != -1) { 
		if (between(y, 160, 295))	
			c=temp;
		else if (between(y, 305, 440)) 
			c=temp+5;
		else c = -1;	
		return c;
	}
	else return -1;	
}

main_game_t* main_game_init()
{
	main_game_t *s = malloc(sizeof(main_game_t));
	unsigned short i;
	
	empty_card = IMG_Load("cards/blank.gif");
	back_card = IMG_Load("cards/back.gif");
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		s->table[i].value=EMPTY;
		s->table[i].surf=NULL;
		s->table[i].position=NULL;
	}
	
	for(i=0; i < NB_CARDS; i++) 
		s->card_list[i] = i;
	
	
	s->user = player_init(USER);
	s->comp = player_init(COMPUTER);
	s->nb_cards_remaining = NB_CARDS; 
	s->current_player = USER;
	s->dropped_card = EMPTY;
	
	/*mixing the cards*/
	mix(s->card_list, NB_CARDS);
	
	/*distributing cards*/
	if(!table_distribute(s->card_list, s->table, &s->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(s->card_list, s->user, &s->nb_cards_remaining)) 
		return NULL;
	if(!player_distribute(s->card_list, s->comp, &s->nb_cards_remaining)) 
		return NULL;
	
	return s;
}

void main_game_handle_input(SDL_Event event, main_game_t *s)
{
	if(s->current_player != USER) 
		return;
	int x = event.button.x;
	int y = event.button.y;	
	if (valid_card_hand(x, y))
		s->user->sel_hand = get_selected_hand(x);	
	else if (valid_card_table(x, y) && (s->user->sel_hand != -1)) 
		s->user->sel_table = get_selected_table(x, y);
}

void main_game_process_actions(main_game_t *s)
{
	if (s->current_player != USER)
		return;
	
	if ((s->user->sel_hand == -1) || (s->user->sel_table == -1))
		return;
	
	if (valid_move(*s->user, s->table)) {
		user_turn(s->user, s->table, &s->dropped_card);
		s->current_player = COMPUTER;
	}
	s->user->sel_hand = -1;
	s->user->sel_table = -1;
}

void main_game_computer_turn(main_game_t *s)
{
	SDL_Delay(2000);
	computer_turn(s->comp, s->table, &s->dropped_card);
	s->current_player = USER;
}
	
bool main_game_render(main_game_t *s, SDL_Surface *screen)
{
	unsigned short i;
	if (!player_render(s->user, screen)) 
		return 0;
	if (!player_render(s->comp, screen))
		return 0;			
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) 
		if (SDL_BlitSurface(s->table[i].surf, NULL, screen, s->table[i].position) == -1) 
			return 0;
	
	SDL_Flip(screen);
	return 1;
}

void main_game_free(main_game_t *s)
{
	unsigned short i;
	
	SDL_FreeSurface(empty_card);
	SDL_FreeSurface(back_card);
	for (i=0;i < MAX_NB_CARDS_TABLE;i++) {
		if((s->table[i].surf != NULL) && (s->table[i].value != EMPTY))
			SDL_FreeSurface(s->table[i].surf);
		if(s->table[i].position != NULL) 
			free(s->table[i].position);
	}
	
	player_free(s->user);
	player_free(s->comp);
	
	free(s);
}
