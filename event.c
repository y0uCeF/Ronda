#include "event.h"


void getEvent(SDL_Event event) {
	switch (event.type) {
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE
		break;
			
}
