#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "main_game.h"
#include "game_play.h"
#include "player.h"
#include "common.h"
#include "game_state.h"
#include "controller.h"
#include "winner.h"
#include "game.h"


#define INNER_SPACE_TABLE 15
#define TABLE_X 40
#define TABLE_FIRSTROW 160
#define TABLE_SECONDROW 305

#define TABLE_XPOS(index) (CARD_WIDTH + INNER_SPACE_TABLE) * (index % 5) \
                + TABLE_X
                
#define TABLE_YPOS(index) (index <= 4)? TABLE_FIRSTROW:TABLE_SECONDROW

/*extern data*/
extern SDL_Surface *empty_card;
extern SDL_Surface *back_card;
extern stack s;
extern player_state_type state;
extern player *last_card_taker;
extern card_num dropped_card;
extern score_t user_score;
extern score_t computer_score;

/*local data*/
static player *user, *comp;
static card table[MAX_NB_CARDS_TABLE];  
static card_num card_list[NB_CARDS];
static unsigned short nb_cards_remaining;
static SDL_Surface *selection;  /*highlight the selected card*/
static SDL_Rect *selection_pos = NULL;
static SDL_Surface *bg = NULL;
static controller_data *c_data = NULL;

/*global data*/
type_t current_player;

static bool table_distribute(card_num card_list[],card table[], 
			unsigned short *nb_cards_remaining) 
{
	unsigned short i, k = 0, j = 0;    
	unsigned short xpos, ypos;
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
                /*making sure the card doesn't exist already*/
		while (exist(table, i, card_list[*nb_cards_remaining-j-1]) != -1) {
			swap(&card_list[*nb_cards_remaining-j-1], &card_list[*nb_cards_remaining-j-k-1]);
			k++;
		}
		xpos = TABLE_XPOS(i);
		ypos = TABLE_YPOS(i);
                /*the first 4 cards are in the 1st, 3rd, 5th and 7th position*/
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

void main_game_init()
{
	unsigned short i;
        card_num j;
	
	empty_card = IMG_Load("data/cards/blank.gif");
	back_card = IMG_Load("data/cards/back.gif");
	selection = IMG_Load("data/cards/selection.png");
        bg = IMG_Load("data/bg.png");
        
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) {
		table[i].value=EMPTY;
		table[i].surf=NULL;
		table[i].position=NULL;
	}
	
	/*filling the list with cards*/
        for(i =0, j = COIN1; j <= CUP12; j++, i++) 
		card_list[i] = j;
	
        /*mixing the cards*/
	mix(card_list, NB_CARDS);
	
        /*initializing users*/
	user = player_init(USER);
	comp = player_init(COMPUTER);
        
        c_data = controller_data_init();
	nb_cards_remaining = NB_CARDS; 
	current_player = USER;
	
	/*distributing cards*/
	if(!table_distribute(card_list, table, &nb_cards_remaining)) 
		return;
	if(!player_distribute(card_list, user, &nb_cards_remaining)) 
		return;
	if(!player_distribute(card_list, comp, &nb_cards_remaining)) 
		return;
	
        /*setting bonus for the first round*/
	set_bonus(user);
        set_bonus(comp);
}

void main_game_handle_input()
{
        controller_data_update(c_data);
}

static void main_game_user_turn()
{
        player_turn(user, table);
}

static void main_game_computer_turn()
{
        if ((comp->sel_hand == -1 || comp->sel_table == -1) &&
                (state == NO_VALID_INPUT))
                set_computer_choice(comp, table);
        
        player_turn(comp, table);
}

static inline bool round_end()
{
	return (user->nb_cards_in_hand == 0) &&
		(comp->nb_cards_in_hand == 0) && (state == NO_VALID_INPUT);
}

static inline bool game_end()
{
	return (nb_cards_remaining == 0) && round_end();
}

void main_game_update()
{
        if (c_data->exit)
                game_exit();
                
        if (c_data->selected_card_hand != -1)
                user->sel_hand = c_data->selected_card_hand;
                
        if ((c_data->selected_card_table != -1) && (user->sel_hand != -1))
                user->sel_table = c_data->selected_card_table;
        
	if (game_end()) {
		if (nb_cards(table, MAX_NB_CARDS_TABLE) != 0)
			take_all_cards(last_card_taker, table);
		handle_bonus(user, comp);
                SDL_Delay(800);
                game_render();
		SDL_Delay(1500);		
		
		set_final_score(user);
		set_final_score(comp);
		
		user_score = user->score;
		computer_score = comp->score;
		
		game_state_t *tmp = set_state_winner();
		push(&s, *tmp);
		top(s).init();
	
	} else {
		if (round_end()) {
			SDL_Delay(800);
			handle_bonus(user, comp);
                        
                        /*setting new round*/
			player_distribute(card_list, user, &nb_cards_remaining);
			player_distribute(card_list, comp, &nb_cards_remaining);
                        
                        dropped_card = EMPTY;
			set_bonus(user);
                        set_bonus(comp);
		}
	
		if (current_player == USER)
			main_game_user_turn();
                else 
			main_game_computer_turn();
	}
        
        if ((user->sel_hand != -1) && (get_sel_hand_val(*user) != EMPTY)) {
                selection_pos = malloc(sizeof(SDL_Rect));
                selection_pos->x = PLAYER_XPOS(user->sel_hand);
                selection_pos->y = PLAYER_YPOS(USER);
                selection_pos->x -= 5;
                selection_pos->y -= 5;
        } else {
                free(selection_pos);
                selection_pos = NULL;
        }
}
		
bool main_game_render(SDL_Surface *screen)
{
	unsigned short i;
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
        if (SDL_BlitSurface(bg, NULL, screen, NULL) == -1) 
			return 0;
	
	if (!player_render(user, screen)) 
		return 0;
	if (!player_render(comp, screen))
		return 0;			
	
	for (i=0; i < MAX_NB_CARDS_TABLE; i++) 
		if (SDL_BlitSurface(table[i].surf, NULL, screen, table[i].position) == -1) 
			return 0;
	
        if (selection_pos != NULL)
                if (SDL_BlitSurface(selection, NULL, screen, selection_pos) == -1) 
			return 0;
	
	SDL_Flip(screen);
	return 1;
}

void main_game_free()
{
	unsigned short i;
	
	SDL_FreeSurface(empty_card);
	SDL_FreeSurface(back_card);
        SDL_FreeSurface(selection);
        SDL_FreeSurface(bg);
        
	for (i=0;i < MAX_NB_CARDS_TABLE;i++) {
		if((table[i].surf != NULL) && (table[i].value != EMPTY))
			SDL_FreeSurface(table[i].surf);
		if(table[i].position != NULL) 
			free(table[i].position);
	}
	
	player_free(user);
	player_free(comp);

        free(c_data);

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
