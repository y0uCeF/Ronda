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

static void bonus_init(bonus_t *bonus, bonus_type_t type, short c)
{
	bonus->type = type;
	bonus->bonus_card = c;
	bonus->surf = NULL;
	bonus->bonus_shown = 1;
	bonus->show_frames = 0;
}

inline static void bonus_init_empty(bonus_t *bonus)
{
	bonus_init(bonus, NONE, -1);
}

player* player_init(type_t t) 
{
	unsigned short i;
	player *p = try_malloc(sizeof(player));

	if (TTF_Init() == -1)
		sdl_ttf_error("Initialisation failed", __FILE__, __LINE__);

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

	bonus_init_empty(&p->card_bonus);
	for (i = 0; i < MAX_BONUS; i++)
		p->action_bonus[i] = NULL;


	p->score_box = load_image("data/scorebox.png", __FILE__, __LINE__);
	return p;
}


void player_distribute(card_num card_list[],player *p, unsigned short *nb_cards_remaining) 
{
	unsigned short i;
	unsigned short posx, posy;
	bool back;

	for (i=0; i<MAX_NB_CARDS_HAND;i++) {
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
}

void add_bonus(player *p, bonus_type_t type, short c)
{
	if (type == RONDA || type == TRINGLA) {
		bonus_t b;
		bonus_init(&b, type, c);
		b.bonus_shown = 0;
		p->card_bonus = b;
	} else if (type == ESTE || type == MISSA) {
		int i;
		for (i = 0; i < MAX_BONUS && p->action_bonus[i] != NULL ; i++);
		p->action_bonus[i] = try_malloc(sizeof(bonus_t));
		bonus_init(p->action_bonus[i], type, c);
		p->action_bonus[i]->bonus_shown = 0;
	}
}

void player_update_bonus(player *p)
{
	int i;
	for (i = 0; i < MAX_BONUS ; i++) {
		if (p->action_bonus[i] != NULL)
			if (p->action_bonus[i]->bonus_shown){
				try_free(p->action_bonus[i]);
				p->action_bonus[i] = NULL;
			}
	}
}

static void player_show_score(player *p, SDL_Surface *scr)
{
	char s[13] = "";
	int y = PLAYER_SCORE_Y(p->type);

	/* gained cards number */
	sprintf(s, "cards   : %d", p->score.gained_cards);
	show_white_text("data/georgiai.ttf", 18, s, PLAYER_SCORE_X, y, scr);

	/* points */
	y += 35;
	sprintf(s, "points : %d", p->score.points);
	show_white_text("data/georgiai.ttf", 18, s, PLAYER_SCORE_X, y, scr);
}

static void display_bonus(bonus_t *b, char *s, short ypos, SDL_Surface *scr)
{
	if (!passed(BONUS_SHOW_TIME, &b->show_frames)) {
		if (s != NULL)
			b->surf = set_text_surf("data/georgiai.ttf", 20, s, 255, 255, 255);
		SDL_Rect pos = {PLAYER_BONUS_X + (130 - b->surf->w)/2 , ypos};
		if (SDL_BlitSurface(b->surf, NULL, scr, &pos) == -1)
			sdl_error("Blit text bonus fail", __FILE__, __LINE__);
		return;
	}

	b->bonus_shown = 1;
	free_surface(b->surf);
	b->surf = NULL;
}

static void player_show_bonus(player *p, SDL_Surface *scr)
{
	int y = PLAYER_BONUS_Y(p->type);
	int diff = (p->type == USER)? -30 : 45;

	if (!p->card_bonus.bonus_shown) {
		char *s = NULL;
		if (!p->card_bonus.surf) {
			s = try_calloc(8, sizeof(char));
			if (p->card_bonus.type == RONDA)
				strcat(s, "Ronda");
			else if (p->card_bonus.type == TRINGLA)
				strcat(s, "Tringla");
		}

		display_bonus(&p->card_bonus, s, y, scr);
		y += diff;
		if (s == NULL)
			free(s);
	}

	int i;
	for (i = 0; i < MAX_BONUS; i++, y += diff) {
		if (p->action_bonus[i] == NULL) 
			continue;

		if (!p->action_bonus[i]->bonus_shown) {
			char *s = NULL;
			if (!p->action_bonus[i]->surf) {
				s = try_calloc(6, sizeof(char));
				if (p->action_bonus[i]->type == ESTE)
					strcat(s, "Este");
				else if (p->action_bonus[i]->type == MISSA)
					strcat(s, "Missa");
			}

			display_bonus(p->action_bonus[i], s, y, scr);

			try_free(s);
		}
	}

}

void player_render(player *p, SDL_Surface *scr)
{
	unsigned short i;

	for (i=0; i < MAX_NB_CARDS_HAND; i++)
		if (SDL_BlitSurface(p->hand[i].surf, NULL, scr, p->hand[i].position) == -1) 
			sdl_error("Blit card fail", __FILE__, __LINE__); 
	if (SDL_BlitSurface(p->score_box, NULL, scr, &p->pos_score_box) == -1) 
		sdl_error("Blit score_box fail", __FILE__, __LINE__);

	player_show_score(p, scr);

	player_show_bonus(p, scr);
}

void player_free(player *p)
{
	if (!p)
		return;

	unsigned short i = 0;
	for (i = 0; i < p->nb_cards_in_hand; i++) {
		if ((p->hand[i].value != EMPTY)	&& (p->type != COMPUTER))
			free_surface(p->hand[i].surf); 
 
		try_free(p->hand[i].position);
	}

	free_surface(p->score_box);
	try_free(p);
	TTF_Quit();
}

card_num get_sel_hand_val(player p)
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

bool ronda(player *p, card_num *c)
{
	if (equal(p->hand[0].value, p->hand[1].value)) 
		*c = p->hand[0].value % 10;
	else if (equal(p->hand[0].value, p->hand[2].value))
		*c = p->hand[0].value % 10;
	else if (equal(p->hand[1].value, p->hand[2].value))
		*c = p->hand[1].value % 10;

	return 	equal(p->hand[0].value, p->hand[1].value) ||
		equal(p->hand[0].value, p->hand[2].value) ||
		equal(p->hand[1].value, p->hand[2].value);
} 

bool tringla(player *p, card_num *c)
{
	if (equal(p->hand[0].value, p->hand[1].value) &&
		equal(p->hand[0].value, p->hand[2].value))
		*c = p->hand[0].value % 10;

	return equal(p->hand[0].value, p->hand[1].value) &&
		equal(p->hand[0].value, p->hand[2].value);
}

void set_card_bonus(player *p)
{
	card_num c = EMPTY;

	if (tringla(p, &c))
		add_bonus(p, TRINGLA, c);
	else if (ronda(p, &c))
		add_bonus(p, RONDA, c);
}

void set_final_score(player *p)
{
	if (p->score.gained_cards > 20)
		p->score.points += p->score.gained_cards - 20;
}
