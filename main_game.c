#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "main_game.h"
#include "player.h"
#include "common.h"
#include "play.h"
#include "game_state.h"
#include "menu.h"

#define PAUSE_FRAMES 75
extern SDL_Surface *empty_card;
extern SDL_Surface *back_card;
extern stack s;
extern player *last_card_taker;

static player *user, *comp;
static card table[MAX_NB_CARDS_TABLE];
static card_num card_list[NB_CARDS];
static unsigned short nb_cards_remaining;
static type_t current_player;
static int nb_frames_round = 0;


static inline void game_exit()
{
	while(!stack_empty(s)) {
		top(s).free();
		pop(&s);
	}	
}

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

static void set_bonus(player *p1, player *p2)
{
	if(tringla(p1))
		p1->bonus_type = TRINGLA;
	else if (ronda(p1))
		p1->bonus_type = RONDA;
	
	if(tringla(p2))
		p2->bonus_type = TRINGLA;
	else if (ronda(p2))
		p2->bonus_type = RONDA;
}

void main_game_init()
{
	unsigned short i;
	
	empty_card = IMG_Load("cards/blank.gif");
	back_card = IMG_Load("cards/back.gif");
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		table[i].value=EMPTY;
		table[i].surf=NULL;
		table[i].position=NULL;
	}
	
	for(i=0; i < NB_CARDS; i++) 
		card_list[i] = i;
	
	
	user = player_init(USER);
	comp = player_init(COMPUTER);
	nb_cards_remaining = NB_CARDS; 
	current_player = USER;
	
	/*mixing the cards*/
	mix(card_list, NB_CARDS);
	
	/*distributing cards*/
	if(!table_distribute(card_list, table, &nb_cards_remaining)) 
		return;
	if(!player_distribute(card_list, user, &nb_cards_remaining)) 
		return;
	if(!player_distribute(card_list, comp, &nb_cards_remaining)) 
		return;
	
	set_bonus(user, comp);
}

static void call_menu_state()
{
	game_state_t *tmp  = NULL;
	tmp = set_state_menu();
	push(&s, *tmp);
	top(s).init();
	free(tmp);	
}

static void treat_keyboard_event(SDL_Event event)
{
	
	switch(event.key.keysym.sym) {
	case SDLK_ESCAPE:
		call_menu_state();
	break;
		
	default:
		
	break;
	}	
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

static void treat_mouse_down_event(SDL_Event event)
{
	switch(event.button.button) {
	case SDL_BUTTON_LEFT:
		if(current_player != USER) 
		return;
	int x = event.button.x;
	int y = event.button.y;	
	if (valid_card_hand(x, y))
		user->sel_hand = get_selected_hand(x);	
	else if (valid_card_table(x, y) && (user->sel_hand != -1)) 
		user->sel_table = get_selected_table(x, y);
	break;
						
	default:
	break;
	}
}

void main_game_handle_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			game_exit();
		else if (event.type == SDL_KEYDOWN)
			treat_keyboard_event(event);
		else if (event.type == SDL_MOUSEBUTTONUP)
			treat_mouse_down_event(event);
	}		
}

static void main_game_user_turn()
{
	if (current_player != USER)
		return;
	
	if ((user->sel_hand == -1) || (user->sel_table == -1))
		return;
	
	if (valid_move(*user, table)) {
		user_turn(user, table);
		current_player = COMPUTER;
	}
	user->sel_hand = -1;
	user->sel_table = -1;
}

static void main_game_computer_turn()
{
	computer_turn(comp, table);
	current_player = USER;
}

static inline bool round_end()
{
	return (user->nb_cards_in_hand == 0) &&
		(comp->nb_cards_in_hand == 0);
}

static inline bool game_end()
{
	return (nb_cards_remaining == 0) && round_end();
}

static void take_all_cards(player *p, card table[])
{
	unsigned short i;
	for (i = 0; i < MAX_NB_CARDS_TABLE; ++i) 
		if (table[i].value != EMPTY){
			SDL_FreeSurface(table[i].surf);
			set_card(&table[i], EMPTY, -1, -1, 0);
			p->score.gained_cards++;
		}	
}

static void handle_bonus(player *p1, player *p2)
{
	if ((p1->bonus_type == NONE) && (p2->bonus_type == NONE))
		return;
	
	if (p1->bonus_type > p2->bonus_type)
		p1->score.points += (p1->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_type < p2->bonus_type)
		p2->score.points += (p2->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_card > p2->bonus_card)
		p1->score.points += (p1->bonus_type == RONDA)? 2:5;
	else if (p1->bonus_card > p2->bonus_card)
		p2->score.points += (p2->bonus_type == RONDA)? 2:5;
		
	p1->bonus_card = -1;
	p1->bonus_type = NONE;
	p2->bonus_card = -1;
	p2->bonus_type = NONE;
	
}

void main_game_update()
{
	if (game_end()) {
		if (nb_cards(table, MAX_NB_CARDS_TABLE) != 0)
			take_all_cards(last_card_taker, table);
		handle_bonus(user, comp);
		SDL_Delay(1500);		
				
		game_exit();
	
	} else {
		if (round_end()) {
			SDL_Delay(800);
			handle_bonus(user, comp);
			player_distribute(card_list, user, &nb_cards_remaining);
			player_distribute(card_list, comp, &nb_cards_remaining);
		
			set_bonus(user, comp);
		}
	
		if (current_player == USER) {
			main_game_user_turn();
		} else if (nb_frames_round < PAUSE_FRAMES) {  /*pause for 75 frames*/
			nb_frames_round++;
		} else {
			nb_frames_round = 0;		
			main_game_computer_turn();
		}
	}
}
		
bool main_game_render(SDL_Surface *screen)
{
	unsigned short i;
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 53, 131, 68));
	
	if (!player_render(user, screen)) 
		return 0;
	if (!player_render(comp, screen))
		return 0;			
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) 
		if (SDL_BlitSurface(table[i].surf, NULL, screen, table[i].position) == -1) 
			return 0;
	
	SDL_Flip(screen);
	return 1;
}

void main_game_free()
{
	unsigned short i;
	
	SDL_FreeSurface(empty_card);
	SDL_FreeSurface(back_card);
	for (i=0;i < MAX_NB_CARDS_TABLE;i++) {
		if((table[i].surf != NULL) && (table[i].value != EMPTY))
			SDL_FreeSurface(table[i].surf);
		if(table[i].position != NULL) 
			free(table[i].position);
	}
	
	player_free(user);
	player_free(comp);
}

game_state_t* set_state_main_game()
{
	game_state_t *gs = malloc(sizeof(game_state_t));
	gs->init = main_game_init;
	gs->handle_input = main_game_handle_input;
	gs->update = main_game_update;
	gs->render = main_game_render;
	gs->free = main_game_free;
	
	return gs;
}
