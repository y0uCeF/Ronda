#ifndef CONST_H
#define CONST_H

#include <SDL/SDL.h>

/**constants definitions to clarify the significance of the numbers
 * most are cards related, others just for algorithmic use.
 */
	#define NB_CARDS 40
    #define NB_CARDS_TABLE 4
    #define MAX_NB_CARDS_TABLE 10
    #define MAX_NB_CARDS_HAND 3
 //   #define EMPTY -1
  //  #define HIDDEN -2
    
/**Definition of data structures used in the program such as cards and 
 * players
 */  
  
typedef enum { HIDDEN = -2, EMPTY=-1,
	  ARGENT1,ARGENT2,ARGENT3,ARGENT4,ARGENT5,ARGENT6,ARGENT7,ARGENT10,ARGENT11,ARGENT12,
	  BATON1,BATON2,BATON3,BATON4,BATON5,BATON6,BATON7,BATON10,BATON11,BATON12,
      EPEE1,EPEE2,EPEE3,EPEE4,EPEE5,EPEE6,EPEE7,EPEE10,EPEE11,EPEE12,
      COUPE1,COUPE2,COUPE3,COUPE4,COUPE5,COUPE6,COUPE7,COUPE10,COUPE11,COUPE12
} card_num;
      

typedef struct card {
	SDL_Surface* surf;
	SDL_Rect* position;
	card_num number;
} card;      


typedef struct score {
	unsigned short int gainedCards;
	unsigned short int points;
} score;


typedef struct player {
    card hand[MAX_NB_CARDS_HAND];
    score score_player;
    int nbCardsInHand;
    enum {USER, COMPUTER} type;
    int ronda_tringla;
} player;

      
#endif
