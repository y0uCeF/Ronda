
CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c common.c play.c player.c game.c game_state.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ronda
INCLUDE= -I/usr/include/SDL

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lSDL -lSDL_image

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	

clean:
	rm -rf *o ronda	
	
