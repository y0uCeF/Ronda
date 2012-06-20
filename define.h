#ifndef CONST_H
#define CONST_H

#include <SDL/SDL.h>

/**constants definitions to clarify the significance of the numbers
 * most are cards related, others just for algorithmic use or for graphics info.
 */
	#define NB_CARDS 40
    #define NB_CARDS_TABLE 4
    #define MAX_NB_CARDS_TABLE 10
    #define MAX_NB_CARDS_HAND 3
	#define WINDOW_WIDTH 800
	#define WINDOW_HEIGHT 600
	#define FPS 30
	#define FRAME_RATE 1000/FPS
    
/**Definition of data structures used in the program such as cards and 
 * players
 */  
  
typedef unsigned short int shortint; 

typedef enum { HIDDEN = -2, EMPTY=-1,
			COIN1, COIN2, COIN3, COIN4, COIN5, COIN6, COIN7, COIN10, COIN11, COIN12,
            CLUB1, CLUB2, CLUB3, CLUB4, CLUB5, CLUB6, CLUB7, CLUB10, CLUB11, CLUB12,
            SWORD1, SWORD2, SWORD3, SWORD4, SWORD5, SWORD6, SWORD7, SWORD10, SWORD11, SWORD12,
            CUP1, CUP2, CUP3, CUP4, CUP5, CUP6, CUP7, CUP10, CUP11, CUP12
} card_num;
      

typedef struct card {
	SDL_Surface* surf;
	SDL_Rect* position;
	card_num number;
} card;      


typedef struct score {
	//SDL_Surface* box;
	shortint gainedCards;
	shortint points;
} score;


typedef enum {USER, COMPUTER} Type;
typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score score_player;
    SDL_Surface *scoreBox;
    SDL_Rect posScoreBox;
    int nbCardsInHand;
    Type type;
    int ronda_tringla;
} player;

      
#endif
