#include "random.h"

int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}


void mix(int* array, int size)
{
	int i=0;
	int number=0;
	int temp=0;

	for(i = 0; i< size;i++) {
		number=rand_a_b(0,size);
		// Swapping 
		temp = array[i];
		array[i] = array[number];
		array[number]=temp;
	}
}


