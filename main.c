#include "game.h"
#include "game_state.h"


int main() 
{
	game_init();

	while (!stack_empty(s)) {
		game_handle_input();
		game_update();
		game_render(); 
	}

	game_free();
	return 0;
}
