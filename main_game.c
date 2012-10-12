#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "main_game.h"
#include "player.h"
#include "common.h"
#include "game_state.h"
#include "controller.h"
#include "menu.h"
#include "game.h"


#define INNER_SPACE_TABLE 15
#define TABLE_X 40
#define TABLE_FIRSTROW 160
#define TABLE_SECONDROW 305

#define TABLE_XPOS(index) (CARD_WIDTH + INNER_SPACE_TABLE) * (index % 5) \
                + TABLE_X
                
#define TABLE_YPOS(index) (index <= 4)? TABLE_FIRSTROW:TABLE_SECONDROW

#define PAUSE_FRAMES_PLAYERS 30
#define PAUSE_FRAMES_CARDS 20

typedef enum {
        NO_VALID_INPUT, PUT_CARD, GET_FIRST_CARD, GET_CARDS, END_ACTIONS
} player_state_type;

extern SDL_Surface *empty_card;
extern SDL_Surface *back_card;
extern stack s;

/*local data*/
static player *user, *comp;
static card table[MAX_NB_CARDS_TABLE];  
static card_num card_list[NB_CARDS];
static unsigned short nb_cards_remaining;
static type_t current_player;
static int nb_frames = 0;
static SDL_Surface *selection;  /*highlight the selected card*/
static SDL_Rect *selection_pos = NULL;
static SDL_Surface *bg = NULL;
static controller_data *c_data = NULL;

static card_num dropped_card = EMPTY;  /* last card dropped */
static player *last_card_taker = NULL;
static player_state_type state = NO_VALID_INPUT; /*player turn states*/
static card_num current_card = EMPTY; /*used when player is taking cards*/

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

static void set_bonus(player *p)
{
	if(tringla(p))
		p->bonus_type = TRINGLA;
	else if (ronda(p))
		p->bonus_type = RONDA;
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

static void call_menu_state()
{
	pop(&s);
        top(s).init();
}

static bool pause(short max_frames)
{
        if (nb_frames < max_frames) {
		nb_frames++;
                return 1;
        } else {
                nb_frames = 0;
                return 0;
        }
}

static bool take_card(player *p, card table[]) 
{	
	int index = exist(table, MAX_NB_CARDS_TABLE, current_card);
	if(index != -1) {
                if(!pause(PAUSE_FRAMES_CARDS)) {
                        /*the cards exists and it's time*/
                        SDL_FreeSurface(table[index].surf);
                        set_card(&table[index], EMPTY, -1, -1, 0);
                        p->score.gained_cards++;
                        if (current_card != 9)
                                current_card++;
                }
                return 1;
	} else {
                /*no cards to take*/
                if (empty(table, MAX_NB_CARDS_TABLE)) 
                        p->score.points++;
                        
                current_card = EMPTY;
                return 0;        
        }
}

static unsigned short get_gain(card_num c, card table[])
{
	unsigned short i = 0, count = 0;
        /*res: the more cards/points the player can gain, 
                the more is the variable's value */
	unsigned short res = 1; 
        
        /*how many cards can the player can get*/
	if (c % 10 != 9) {
		short index = exist(table, MAX_NB_CARDS_TABLE, c);
		while ((index != -1) && (i <= (9 - c%10))) {
			count++;
                        i++;
			index = exist(table, MAX_NB_CARDS_TABLE, c+count);
		}
	}
        
        res += count;
        
        /*we make sure this is a better gain than in taking 2 cards*/
        if (c == dropped_card)
                res += 2; 

        return res;
}

static short get_index_ai(player *p, card table[], short *index_tab)
{
	unsigned short i, j;
	unsigned short tmp_gain, gain = 0;
	short best_card_index = -1;
	*index_tab = -1;
        
        /*let's choose the best card (the best gain)*/
	for (i=0; i < MAX_NB_CARDS_HAND; i++) {
		card_num tmp = p->hand[i].value;
		if (tmp != EMPTY) 
			for (j=0; j < MAX_NB_CARDS_TABLE; j++) {
				if (equal(tmp, table[j].value)) {
					tmp_gain = get_gain(tmp, table);
					if (gain < tmp_gain) {
						gain = tmp_gain;
						best_card_index = i;
						*index_tab = j;
					}
				}
			}
		
	}
	
	return best_card_index;
}

void update_state(player p, card table[])
{
	card_num selected_hand = get_sel_hand_val(p);
	card_num selected_table = table[p.sel_table].value;
	
        if ((selected_table == EMPTY) && 
		(exist(table, MAX_NB_CARDS_TABLE, selected_hand) == -1))
		state = PUT_CARD;
	else if ((selected_table != EMPTY) &&
		(equal(selected_hand, selected_table)))
		state = GET_FIRST_CARD;
	else 
                state = NO_VALID_INPUT;
}

void player_turn(player *p, card table[]) 
{
	card_num selected_hand = get_sel_hand_val(*p);
	card_num selected_table = table[p->sel_table].value;
        
	short y = PLAYER_YPOS(p->type);	
        
        /*what will the player do, depending on the states*/
        switch (state) {
        case NO_VALID_INPUT:       
                if ((p->sel_hand != -1) && (p->sel_table != -1))
                        update_state(*p, table);
        break;
        
        case PUT_CARD:
                if (pause(PAUSE_FRAMES_PLAYERS) && (p->type == COMPUTER))
                        break;
                        
                /*moving the card from hand to table*/
                if (p->type == USER)
			table[p->sel_table].surf = get_sel_hand_surf(*p);
		else
			table[p->sel_table].surf = IMG_Load(get_file(selected_hand));
		
		table[p->sel_table].value = selected_hand;
		dropped_card = selected_hand;
		set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0);
                p->nb_cards_in_hand--;
                
                /*no further actions*/
                state = END_ACTIONS;
        break;
        
        case GET_FIRST_CARD:
                if (pause(PAUSE_FRAMES_PLAYERS) && (p->type == COMPUTER))
                        break;
                if(selected_table == dropped_card) 
			p->score.points++;
                        
		dropped_card = EMPTY;
		last_card_taker = p;
                
                /*removing both cards from hand/table */
                set_card(&p->hand[p->sel_hand], EMPTY, -1, y, 0);
                SDL_FreeSurface(table[p->sel_table].surf);
		set_card(&table[p->sel_table], EMPTY, -1, -1, 0);
		p->score.gained_cards += 2;
                p->nb_cards_in_hand--;
                
                card_num tmp = selected_hand % 10;
                
                /*take the rest of the cards if possible*/
                if (tmp != 9) {
                        state = GET_CARDS;
                        current_card = tmp + 1;
                } else {
                        state = END_ACTIONS;
                }
        break;
        
        case GET_CARDS:
                p->sel_hand = p->sel_table = -1;
                if (!take_card(p, table)) 
                        state = END_ACTIONS;
        break;
        
        case END_ACTIONS:
                current_player = !p->type;
                p->sel_hand = p->sel_table = -1;
                state = NO_VALID_INPUT;
        break;
        }
	
}

void set_computer_choice(player *p, card table[])
{
	short index_tab;
	short index_hand = get_index_ai(p, table, &index_tab);
	
	if (index_hand == -1) {
		short i = rand_a_b(0, 3);
		while (p->hand[i].value == EMPTY)
			i = rand_a_b(0, 3);
		index_hand = i;	
	}
	
	if (index_tab == -1) {
		short i = rand_a_b(0, 10);
		while (table[i].value != EMPTY)
			i = rand_a_b(0, 10);
		index_tab = i;	
	}
	p->sel_hand = index_hand;
	p->sel_table = index_tab;
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
	
        /*check the Readme file for points calculation*/
	if (p1->bonus_type > p2->bonus_type)
		p1->score.points += (p1->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_type < p2->bonus_type)
		p2->score.points += (p2->bonus_type == RONDA)? 1:5;
	else if (p1->bonus_card > p2->bonus_card)
		p1->score.points += (p1->bonus_type == RONDA)? 2:5;
	else if (p1->bonus_card < p2->bonus_card)
		p2->score.points += (p2->bonus_type == RONDA)? 2:5;
		
	p1->bonus_card = -1;
	p1->bonus_type = NONE;
	p2->bonus_card = -1;
	p2->bonus_type = NONE;
	
}

void main_game_update()
{
        if (c_data->exit)
                game_exit();
        if (c_data->call_menu == 1)
                call_menu_state();
                
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
				
		game_exit();
	
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
        
        if (user->sel_hand != -1) {
                selection_pos = malloc(sizeof(SDL_Rect));
                selection_pos->x = PLAYER_XPOS(user->sel_hand);
                selection_pos->y = PLAYER_YPOS(USER);
                selection_pos->x -= 2;
                selection_pos->y -= 2;
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
