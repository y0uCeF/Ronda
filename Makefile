
CC = gcc
CFLAGS = -c -Wall
LDFLAGS =
SOURCES = common.c player.c game_state.c winner.c controller.c game_play.c \
main_game.c game.c main.c 
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ronda
INCLUDE = -I/usr/include/SDL

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lSDL -lSDL_image -lSDL_ttf

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	

clean:
	rm -rf *o ronda	
	
