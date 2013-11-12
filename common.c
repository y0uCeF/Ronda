#include <stdarg.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "game.h"

/* global data */
SDL_Texture *back_card = NULL;
SDL_Texture *empty_card = NULL;

extern SDL_Renderer *renderer;

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

SDL_Texture *load_image_(const char *path, SDL_Renderer *ren, char *file, int line)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, path);

	if (!texture)		
		sdl_image_error("LoadTexture", file, line);

	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
	int w, h;
	
	if (!tex)
		return;
	
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	
	SDL_Rect dst = {x, y, w, h};
	
	if(SDL_RenderCopy(ren, tex, clip, &dst) < 0)
		sdl_error("render failed");
}

int tex_w(SDL_Texture *tex)
{
	int w = 0;
	
	if (!tex)
		return -1;
	
	SDL_QueryTexture(tex, NULL, NULL, &w, NULL);
	
	return w;
}

int tex_h(SDL_Texture *tex)
{
	int h = 0;
	
	if (!tex)
		return -1;
	
	SDL_QueryTexture(tex, NULL, NULL, NULL, &h);
	
	return h;
}

char* get_card_file(card_num n) 
{
#define CARD_FILE_SIZE 5
	char num[3]="";
	unsigned short len = strlen(CARDS_DIR) + 1;
	len += CARD_FILE_SIZE;
	char* file = try_calloc(len, sizeof(char));
	num[0]=(char) (n/10+ (char) '0');
	num[1]=(char) (n%10+ (char) '0');
	strcat(file, CARDS_DIR);
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
		c->tex = empty_card;
	} else if (back){
		c->tex = back_card;
	} else {
		char *file = get_card_file(num);
		c->tex = load_image(file, renderer);
	}

	if (c->tex == NULL) 
		sdl_error("file not found");
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

SDL_Texture* set_text_surf(char *font_name, int size, char* text, short r,
	short g, short b, SDL_Renderer *renderer)
{
	TTF_Font* font = TTF_OpenFont(font_name, size);
	if (!font)
		sdl_ttf_error("TTF_OpenFont");
	SDL_Color foreground = {r, g, b};

	SDL_Surface *surf = TTF_RenderText_Blended(font, text, foreground);
	if (!surf)
		sdl_ttf_error("TTF_RenderText");
		
	TTF_CloseFont(font);
	
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	return tex;
}

void show_text(char *font, short size, char *txt, short posx, short posy, 
			short r, short g, short b, SDL_Renderer *renderer)
{
	SDL_Texture *tex = set_text_surf(font, size, txt, r, g, b, renderer);
	renderTexture(tex, renderer, posx, posy, NULL);
}

inline void show_white_text(char *font, short size, char *txt, short posx, 
		short posy, SDL_Renderer *renderer)
{
	show_text(font, size, txt, posx, posy, 255, 255, 255, renderer);
}

inline void show_black_text(char *font, short size, char *txt, short posx, 
		short posy, SDL_Renderer *renderer)
{
	show_text(font, size, txt, posx, posy, 0, 0, 0, renderer);
}
