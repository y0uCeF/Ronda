#include <stdarg.h>
#include <errno.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "common.h"
#include "game.h"

/* global data */
SDL_Surface *back_card = NULL;
SDL_Surface *empty_card = NULL;

void mix(card_num array[], unsigned short size)
{
	int i=0;
	int random_number=0;
	int tmp=0;

	for (i = 0; i < size; i++) {
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
		if ((tab[i].value != EMPTY) && equal(tab[i].value, value)) 
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

void error(char *file, int line, char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	#ifdef _DEBUG
		fprintf(stderr, ", in:%s - line = %d\n",file,  line);
	#endif
	game_exit(EXIT_FAILURE);
}

void general_error(char *file, int line, char *msg)
{
	if (errno)
		error(file, line, "%s : %s\n", msg, strerror(errno)); 
	else
		error(file, line, "%s\n", msg);
}

void *malloc_(size_t size, char *file, int line)
{
	errno = 0;
	void *tmp = malloc(size);
	if (!tmp)
		general_error(file, line, "malloc fail");
	return tmp;
}

void *calloc_(size_t n, size_t size, char *file, int line)
{
	errno = 0;
	void *tmp = calloc(n, size);
	if (!tmp)
		general_error(file, line, "calloc fail");
	return tmp;
}

void *realloc_(void *ptr, size_t size,char *file, int line)
{
	errno = 0;
	void *tmp = realloc(ptr, size);
	if (!tmp)
		general_error(file, line, "realloc fail");
	return tmp;
}

SDL_Surface *load_image(char *path, char *file, int line)
{
	SDL_Surface *tmp = IMG_Load(path);
	if (!tmp)
		sdl_image_error("file not found", file, line);
	return tmp;
}

char* get_file(card_num n) 
{
	char num[3]="";
	char* file = try_calloc(21, sizeof(char));
	num[0]=(char) (n/10+ (char) '0');
	num[1]=(char) (n%10+ (char) '0');
	strcat(file, "data/cards/");
	strcat(file, num);
	strcat(file, ".gif");

	return file;
}

void set_card(card *c, card_num num, short xpos, short ypos, bool back)
{
	c->value=num;

	if (((xpos != -1) || (ypos != -1)) && (!c->position))
		c->position = try_malloc(sizeof(SDL_Rect));

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
		c->surf = load_image(file, __FILE__, __LINE__);
	}

	if (c->surf == NULL) 
		sdl_error("file not found", __FILE__, __LINE__);
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
	if (!font)
		sdl_ttf_error("TTF_OpenFont", __FILE__, __LINE__);
	SDL_Color foreground = {r, g, b};

	SDL_Surface *surf = TTF_RenderText_Blended(font, text, foreground);
	if (!surf)
		sdl_ttf_error("TTF_RenderText", __FILE__, __LINE__);
	TTF_CloseFont(font);

	return surf;
}

void show_text(char *font, short size, char *txt, short posx, short posy, 
			short r, short g, short b, SDL_Surface *scr)
{
	SDL_Rect pos = {posx, posy};
	SDL_Surface *surf = set_text_surf(font, size, txt, r, g, b);
	if (SDL_BlitSurface(surf, NULL, scr, &pos) == -1)
		sdl_error("Blit fail", __FILE__, __LINE__);
	free_surface(surf);
}

inline void show_white_text(char *font, short size, char *txt, short posx, 
		short posy, SDL_Surface *scr)
{
	show_text(font, size, txt, posx, posy, 255, 255, 255, scr);
}
