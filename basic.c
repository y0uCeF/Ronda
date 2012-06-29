#include "basic.h"

static int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}

void mix(card_num array[], int size)
{
	int i=0;
	int number=0;
	int tmp=0;

	for(i = 0; i< size;i++) {
		number=rand_a_b(0,size);
		// Swapping 
		tmp = array[i];
		array[i] = array[number];
		array[number]=tmp;
	}
}


void swap(card_num *a, card_num *b) {
    int tmp = *a;
    *a= *b;
    *b = tmp;
}

bool equal(card_num a, card_num b) {
    return ((a%10) == (b%10));
}

short exist(card tab[], int size, card_num value)
{
    short i;
    for (i=0; i<size; i++) {
		if(tab[i].number != EMPTY)
			if (equal(tab[i].number, value)) 
				return i;
    }
    return -1;
}


bool empty(card tab[], shortint size) {
	shortint i;
	for (i=0; i<size; i++) {
		if (tab[i].number != -1) return 0;	
	}
	return 1;
}


shortint nbCards(card tab[], shortint size) {
    shortint i, cpt=0;
    for (i=0; i<size; i++) {
		if (tab[i].number != EMPTY) cpt++;
	}
	return cpt;	
}


void getFile(card_num n, char* file) {
	char num[3]="";
	num[0]=(char) (n/10+ (char) '0');
	num[1]=(char) (n%10+ (char) '0');
	strcat(file,"cards/");
	strcat(file,num);
	strcat(file,".gif");
}

bool between(shortint x, shortint a, shortint b) {
	return 	((x>=a) && (x<=b));
}
