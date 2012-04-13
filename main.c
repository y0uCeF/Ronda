#include <stdio.h>
#include <stdlib.h>
#include "define.h"

int main(int argc,char** argv) {
	SDL_Event event;
	int running=1;
	player user,computer;
	card* table[10]={0};
	init();
	
	while (running) {
		while(SDL_PollEvent(&event))	{
			switch (event.type) {
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running=0;
						break;
					}	
				break;
		
				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button) {
						case plah:
				
						break;
					}
			}
		
		}
		
		loop();
	}	
	
	
	return 0;
}
