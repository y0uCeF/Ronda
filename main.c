#include "game.h"
#include "game_state.h"

extern stack s;

int main() 
{
	game_init();
	
	while (!stack_empty(s)) {     
		game_handle_input();		
		game_update();
		if(!game_render())
			return 1; 
	}
	
	game_free();	
	return 0; 		
}
