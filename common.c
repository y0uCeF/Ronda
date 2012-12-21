#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "common.h"

SDL_Surface *back_card = NULL;
SDL_Surface *empty_card = NULL;

inline int rand_a_b(int a, int b)
{
	return rand() % (b - a) + a;
}

inline bool between(unsigned short x, unsigned short a, unsigned short b) 
{
	return 	((x >= a) && ( x<= b));
}

inline bool equal(card_num a, card_num b) 
{
	return ((a % 10) == (b % 10));
}

void mix(card_num array[], unsigned short size)
{
	int i=0;
	int random_number=0;
	int tmp=0;

	for(i = 0; i < size; i++) {
		random_number=rand_a_b(0, size);
		 
		tmp = array[i];
		array[i] = array[random_number];
		array[random_number] = tmp;
	}
}

void swap(card_num *a, card_num *b) 
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

short exist(card tab[], unsigned short size, card_num value)
{
	short i;
	for (i=0; i < size; i++) 
		if((tab[i].value != EMPTY) && equal(tab[i].value, value)) 
				return i;
    return -1;
}

bool empty(card tab[], unsigned short size) 
{
	unsigned short i;
	for (i=0; i < size; i++) 
		if (tab[i].value != -1) 
                        return 0;
	return 1;
}

unsigned short nb_cards(card tab[], unsigned short size) 
{
	unsigned short i, cpt=0;
	for (i=0; i < size; i++) 
		if (tab[i].value != EMPTY) 
			cpt++;
	return cpt;	
}

char* get_file(card_num n) 
{
	char num[3]="";
	char* file = calloc(21, sizeof(char));
	num[0]=(char) (n/10+ (char) '0');
	num[1]=(char) (n%10+ (char) '0');
	strcat(file, "data/cards/");
	strcat(file, num);
	strcat(file, ".gif");
	return file;
}

bool sdl_error(char* msg)
{
	fprintf(stderr, "%s : %s", msg, SDL_GetError());
	return 0;
}

bool set_card(card *c, card_num num, short xpos, short ypos, bool back)
{
	c->value=num;
	
	if(((xpos != -1) || (ypos != -1)) && (!c->position))
		c->position=malloc(sizeof(SDL_Rect));
	
	if (xpos != -1)
		c->position->x = xpos;
	if (ypos != -1)
		c->position->y = ypos;
	
	if (num == EMPTY) {
		c->surf = empty_card;
	} else if (back){
		c->surf = back_card;
	} else {
		char *file = get_file(num);
		c->surf = IMG_Load(file);
	}
	
	if(c->surf == NULL) 
		return sdl_error("file not found");
	return 1;	
}

bool passed(short max_frames, int *nb_frames)
{
        if (*nb_frames < max_frames) {
		(*nb_frames)++;
                return 0;
        } else {
                *nb_frames = 0;
                return 1;
        }
}

SDL_Surface* set_text_surf(char *font_name, int size, char* text, short r,
                                short g, short b)
{
        TTF_Font* font = TTF_OpenFont(font_name, size);
	SDL_Color foreground = {r, g, b};
	
	SDL_Surface *surf = TTF_RenderText_Blended(font, text, foreground);
        
        TTF_CloseFont(font);
        return surf;
}

void show_text(char *font, short size, char *txt, short posx, short posy, 
			short r, short g, short b, SDL_Surface *scr)
{
	SDL_Rect pos = {posx, posy};
	SDL_Surface *surf = set_text_surf(font, size, txt, r, g, b);
	SDL_BlitSurface(surf, NULL, scr, &pos);
    SDL_FreeSurface(surf);	
}

inline void show_white_text(char *font, short size, char *txt, short posx, short posy, 
		SDL_Surface *scr)
{
	show_text(font, size, txt, posx, posy, 255, 255, 255, scr);
}
