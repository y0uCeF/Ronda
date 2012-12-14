#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

#include "player.h"
#include "common.h"

/* score constants */
#define PLAYER_BOX_X 600
#define USER_BOX_Y 470
#define COMPUTER_BOX_Y 40
#define PLAYER_BOX_Y(type) (type == USER)? USER_BOX_Y:COMPUTER_BOX_Y
#define USER_SCORE_Y 485
#define COMPUTER_SCORE_Y 55
#define PLAYER_SCORE_Y(type) (type == USER)? USER_SCORE_Y:COMPUTER_SCORE_Y
#define PLAYER_SCORE_X 620

/* bonus coordinates and constants */
#define USER_BONUS_Y 420
#define COMPUTER_BONUS_Y 150
#define PLAYER_BONUS_Y(type) (type == USER)? USER_BONUS_Y:COMPUTER_BONUS_Y
#define PLAYER_BONUS_X 600
  
#define BONUS_SHOW_TIME 100

player* player_init(type_t t) 
{
	unsigned short i;
	player *p = malloc(sizeof(player));
        
        TTF_Init();
	for (i = 0; i < MAX_NB_CARDS_HAND; i++) {
		p->hand[i].value = EMPTY;
		p->hand[i].surf = NULL;
		p->hand[i].position = NULL;
	}
        	
	p->nb_cards_in_hand = 0;
	p->score.gained_cards = 0;
	p->score.points = 0;
	p->type = t;
	p->pos_score_box.x = PLAYER_BOX_X;
	p->pos_score_box.y = PLAYER_BOX_Y(t);
	p->sel_hand = -1;
	p->sel_table = -1;
	p->bonus_card = -1;
	p->bonus_type = NONE;
        p->extra_bonus = NO_EXTRA;
        p->bonus_surf = NULL;
        p->bonus_shown = 0;
        p->bonus_show_frames = 0;
	
        p->score_box = IMG_Load("data/scorebox.png");
	return p;	
}


bool player_distribute(card_num card_list[],player *p, unsigned short *nb_cards_remaining) 
{
	unsigned short i;   
	unsigned short posx, posy;
	bool back;

	for(i=0; i<MAX_NB_CARDS_HAND;i++) {
		posx = PLAYER_XPOS(i);
		
		if (p->type == USER) {			
			posy = 450;
			back = 0;
		} else {
			posy = 15;
			back = 1;
		}
		
		set_card(&p->hand[i], card_list[*nb_cards_remaining-1-i], posx, posy, back);

		p->nb_cards_in_hand++;
	}
        
	*nb_cards_remaining -= i;
        return 1;
}

static void player_show_score(player *p, SDL_Surface *scr)
{
        char s[13] = "";
        int y = PLAYER_SCORE_Y(p->type);
        
        /* gained cards number */
        sprintf(s, "cards   : %d", p->score.gained_cards);
        SDL_Surface *surf = set_text_surf("data/georgiai.ttf", 18, s, 255, 255, 255);
        SDL_Rect pos = {PLAYER_SCORE_X, y};
        SDL_BlitSurface(surf, NULL, scr, &pos);
        SDL_FreeSurface(surf);
        
        /* points */
        y += 35;
        sprintf(s, "points : %d", p->score.points);
        surf = set_text_surf("data/georgiai.ttf", 18, s, 255, 255, 255);
        pos.x = PLAYER_SCORE_X;
        pos.y = y;
        SDL_BlitSurface(surf, NULL, scr, &pos);
        SDL_FreeSurface(surf);
}

static void player_show_bonus(player *p, SDL_Surface *scr)
{
        char *s = calloc(8, sizeof(char));
        int y = PLAYER_BONUS_Y(p->type);
        
        /* bonus type? */
        if (p->extra_bonus == ESTE)
                strcat(s, "Este");
        else if (p->extra_bonus == MISSA)
                strcat(s, "Missa");
        else if (p->bonus_type == RONDA)
                strcat(s, "Ronda");
        else if (p->bonus_type == TRINGLA)
                strcat(s, "Tringla");
        else
                return;
        
        /* displaying bonus for 100 frames */
        if (p->bonus_shown) {
                p->extra_bonus = NO_EXTRA;
                goto end;
        }
        
        if (!passed(BONUS_SHOW_TIME, &p->bonus_show_frames)) {
                p->bonus_surf = set_text_surf("data/georgiai.ttf", 20, s, 255, 255, 255);
                SDL_Rect pos = {PLAYER_BONUS_X + (130 - p->bonus_surf->w)/2 , y};
                SDL_BlitSurface(p->bonus_surf, NULL, scr, &pos);
                SDL_FreeSurface(p->bonus_surf);
        }
        else {
                p->bonus_shown = 1;
        }
        
end:    free(s);
}

bool player_render(player *p, SDL_Surface *scr)
{
	unsigned short i;
	
	for (i=0; i < MAX_NB_CARDS_HAND; i++)
		if (SDL_BlitSurface(p->hand[i].surf, NULL, scr, p->hand[i].position) == -1) 
			return 0; 
	if(SDL_BlitSurface(p->score_box, NULL, scr, &p->pos_score_box) == -1) 
		return 0;

        player_show_score(p, scr);
        
        player_show_bonus(p, scr);
                	
	return 1;
}

void player_free(player *p)
{
	unsigned short i;
	for (i=0; i < p->nb_cards_in_hand;i++) {
		if((p->hand[i].surf != NULL) && (p->hand[i].value != EMPTY)
			&& (p->type != COMPUTER))	
			SDL_FreeSurface(p->hand[i].surf); 
		if(p->hand[i].position != NULL) 
			free(p->hand[i].position);
	}
	if(p->score_box != NULL) 
		SDL_FreeSurface(p->score_box);
	free(p);
        TTF_Quit();	
}

inline card_num get_sel_hand_val(player p)
{
	if (p.sel_hand != -1)
		return p.hand[p.sel_hand].value;
	else 
		return -1;	
}

inline SDL_Surface* get_sel_hand_surf(player p)
{
	if (p.sel_hand != -1)
		return p.hand[p.sel_hand].surf;
	else
		return NULL;
}		

bool ronda(player *p)
{
	if(equal(p->hand[0].value, p->hand[1].value)) 
		p->bonus_card = p->hand[0].value % 10;
	else if (equal(p->hand[0].value, p->hand[2].value))
		p->bonus_card = p->hand[0].value % 10;
	else if (equal(p->hand[1].value, p->hand[2].value))
		p->bonus_card = p->hand[1].value % 10;
		
	return 	equal(p->hand[0].value, p->hand[1].value) ||
		equal(p->hand[0].value, p->hand[2].value) ||
		equal(p->hand[1].value, p->hand[2].value);
} 

bool tringla(player *p)
{
	if (equal(p->hand[0].value, p->hand[1].value) &&
		equal(p->hand[0].value, p->hand[2].value))
		p->bonus_card = p->hand[0].value % 10;
		
	return equal(p->hand[0].value, p->hand[1].value) &&
		equal(p->hand[0].value, p->hand[2].value);
}

void set_bonus(player *p)
{
        p->bonus_shown = 0;
	if(tringla(p))
		p->bonus_type = TRINGLA;
	else if (ronda(p))
		p->bonus_type = RONDA;
        else
                p->bonus_shown = 1;
                
}

void set_final_score(player *p)
{
	if (p->score.gained_cards >20)
		p->score.points += p->score.gained_cards - 20;
}
