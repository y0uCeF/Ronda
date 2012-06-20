
CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c random.c functions.c basic.c
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
	
