#include "game.h"

int main(int argc,char** argv) 
{	
	game_t *g = game_init();
	
	if (g == NULL) 
		return 1;
	game_run(g);
	game_free(g);	
	return 0;
}
