#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

#include "player.h"
#include "common.h"

player* player_init(type_t t) 
{
	unsigned short i;
	player *p = malloc(sizeof(player));
        
        TTF_Init();
	for (i=0; i<MAX_NB_CARDS_HAND; i++) {
		p->hand[i].value = EMPTY;
		p->hand[i].surf = NULL;
		p->hand[i].position = NULL;
	}	
	p->nb_cards_in_hand = 0;
	p->score.gained_cards = 0;
	p->score.points = 0;
	p->type = t;
	p->pos_score_box.x = 600;
	p->pos_score_box.y = (t == USER)? 470:40;
	p->sel_hand = -1;
	p->sel_table = -1;
	p->bonus_card = -1;
	p->bonus_type = NONE;
	p->score_box=SDL_CreateRGBSurface(SDL_HWSURFACE, 130, 90, 32,  0, 0, 0, 0);
	return p;	
}


bool player_distribute(card_num card_list[],player *p, unsigned short *nb_cards_remaining) 
{
	unsigned short i;   
	unsigned short posx, posy;
	bool back;

	for(i=0; i<MAX_NB_CARDS_HAND;i++) {
		posx = (90+40)*i+150;
		
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

static void show_text(SDL_Surface *screen, char *font_name, char *text, int size, 
                        int x, int y)
{
        TTF_Font* font = TTF_OpenFont(font_name, size);
	SDL_Color foreground = {255, 255, 255};
	
	SDL_Surface *surf = TTF_RenderText_Blended(font, text, foreground);
	
        SDL_Rect pos = {x, y};
        SDL_BlitSurface(surf, NULL, screen, &pos);
	TTF_CloseFont(font);	
}

static void player_show_score(player *p, SDL_Surface *scr)
{
        char s[13] = "";
        int y = (p->type == USER)? 485 : 55;
        sprintf(s, "cards   : %d", p->score.gained_cards);
        show_text(scr, "georgiai.ttf", s, 18, 620, y);
        
        sprintf(s, "points : %d", p->score.points);
        show_text(scr, "georgiai.ttf", s, 18, 620, y + 35);
}

bool player_render(player *p, SDL_Surface *scr)
{
	unsigned short i;
	SDL_FillRect(p->score_box, NULL, SDL_MapRGB(p->score_box->format, 35, 50, 20));
	
	for (i=0; i < MAX_NB_CARDS_HAND; i++)
		if (SDL_BlitSurface(p->hand[i].surf, NULL, scr, p->hand[i].position) == -1) 
			return 0; 
	if(SDL_BlitSurface(p->score_box, NULL, scr, &p->pos_score_box) == -1) 
		return 0;
        player_show_score(p, scr);
                	
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
