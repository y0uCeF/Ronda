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
DESKTOPDIR = $(prefix)/share/applications
ICONDIR = $(prefix)/share/pixmaps
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
	install -D -m755 ronda $(BINDIR)/ronda
	install -d -m755 $(GFXDIR)
	install -m755 data/gfx/* $(GFXDIR)
	install -d -m755 $(FONTSDIR)
	install -m755 data/fonts/* $(FONTSDIR)
	install -d -m755 $(CARDSDIR)
	install -m755 data/cards/* $(CARDSDIR)
	sed -i "s|Icon=|Icon=$(ICONDIR)/ronda.png|" data/ronda.desktop
	install -D -m755 data/ronda.desktop $(DESKTOPDIR)/ronda.desktop
	install -D -m775 data/gfx/ronda.png $(ICONDIR)/ronda.png


uninstall:
	rm -rf $(BINDIR)/ronda $(DATADIR) $(ICONDIR)/ronda.png $(DESKTOPDIR)/ronda.desktop
	

clean:
	rm -rf *o ronda	
