CC = gcc
CFLAGS = -c -Wall
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf
LIBS = -I/usr/include/SDL
SOURCES = common.c \
          player.c \
          game_state.c \
          winner.c \
          controller.c \
          game_play.c \
          main_game.c \
          game.c \
          main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ronda
prefix = $(DESTDIR)/usr
BINDIR = $(prefix)/bin
DATADIR = $(prefix)/share/ronda/
GFXDIR = $(DATADIR)/gfx/
FONTSDIR = $(DATADIR)/fonts/
CARDSDIR = $(DATADIR)/cards/
INPLACE =

ifndef INPLACE
	CFLAGS = -c -Wall -DDATA_DIR=\"$(DATADIR)\"
endif

all: exec

debug: CFLAGS += -D_DEBUG -g
debug: exec

exec: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LIBS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

install:
	install -d -m755 $(BINDIR)
	install -m755 ronda $(BINDIR)
	install -d -m755 $(GFXDIR)
	install data/gfx/* $(GFXDIR)
	install -d -m755 $(FONTSDIR)
	install data/fonts/* $(FONTSDIR)
	install -d -m755 $(CARDSDIR)
	install data/cards/* $(CARDSDIR)

uninstall:
	rm -rf $(BINDIR)/ronda $(DATADIR)

clean:
	rm -rf *o ronda	
