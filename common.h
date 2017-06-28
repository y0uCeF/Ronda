#ifndef COMMON_H
#define COMMON_H

#include "define.h"

/* some useful macros */ 

#define rand_a_b(a, b)     (rand() % ((b) - (a)) + (a))
#define between(x, a, b)   (((x) >= (a)) && ((x) <= (b)))
#define equal(a, b)        (((a) % 10) == ((b) % 10))

#define sdl_error(msg)     \
	error(__FILE__, __LINE__, "%s : %s\n", msg, SDL_GetError())

#define sdl_image_error(msg, file, line)    \
	error(file, line, "%s : %s\n", msg, IMG_GetError());

#define sdl_ttf_error(msg)  \
	error(__FILE__, __LINE__, "%s : %s\n", msg, TTF_GetError()); 

#define load_image(path, ren)       load_image_(path, ren, __FILE__, __LINE__)
#define try_malloc(size)       malloc_(size, __FILE__, __LINE__)
#define try_calloc(n, size)    calloc_(n, size, __FILE__, __LINE__)
#define try_realloc(ptr, size) realloc_(ptr, size, __FILE__, __LINE__)
#define try_free(ptr)          do { \
				if(!ptr) break; \
				free(ptr); ptr = NULL; \
				} while(0)
#define free_surface(surf)     do { \
				SDL_FreeSurface(surf); surf = NULL; \
				} while(0)

extern SDL_Texture *back_card;
extern SDL_Texture *empty_card;


/* error handling and memory functions */

/*
 * reports error and ends the program, shouldn't be called directly
 * reserved for use by other error functions
 */
void error(char *file, int line, char *format, ...);

/*
 * malloc with error handling
 */
void *malloc_(size_t size, char *file, int line);

/*
 * calloc with error handling
 */
void *calloc_(size_t n, size_t size, char *file, int line);

/*
 * realloc with error handling
 */ 
void *realloc_(void *ptr, size_t size, char *file, int line);

/* other general functions */

/*
 * swaps contents of a and b
 */
void swap(card_num *a, card_num *b);

/*
 * checks if value exists in tab
 */
short exist(card tab[], unsigned short size, card_num value);

/*
 * checks if tab is empty
 */
bool empty(card tab[], unsigned short size);

/*
 * returns path to file representing the card n
 */
char* get_card_file(card_num n);

/*
 * reports error in stderr and terminates the program
 */
void general_error(char *file, int line, char *msg);

/*
 * sets card attributes given in parameters
 */
void set_card(card *c, card_num num, short posx, short posy, bool back);

/*
 * checks if max_frames has passed
 */
bool passed(short max_frames, int *nb_frames);  

/*
 * loads image specified by path, use the macro load_image() instead
 */
SDL_Texture *load_image_(const char *path, SDL_Renderer *ren, char *file, int line);

/*
 * renders the texture "tex" on the screen in the given position
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);

/*
 * returns the texture's width
 */ 
int tex_w(SDL_Texture *tex);

/*
 * returns the texture's height
 */ 
int tex_h(SDL_Texture *tex);

/*
 * returns text surface
 */
SDL_Texture* set_text_surf(char *font_name, int size, char* text, short r,
				short g, short b, SDL_Renderer *renderer);

/*
 * displays text "txt" on the screen, position specified by posx, posy
 */
void show_text(char *font, short size, char *txt, short posx, short posy, 
			short r, short g, short b, SDL_Renderer *renderer);

/*
 * displays text set in "txt" in white color using show_text()
 */
void show_white_text(char *font, short size, char *txt, short posx, 
		short posy, SDL_Renderer *renderer);

/*
 * displays text set in "txt" in black color using show_text()
 */
void show_black_text(char *font, short size, char *txt, short posx, 
		short posy, SDL_Renderer *renderer);

#endif
