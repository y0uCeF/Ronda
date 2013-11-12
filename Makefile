CC = gcc
CFLAGS = -c -Wall
PKGCONFIG=pkg-config
LDFLAGS = $(shell $(PKGCONFIG) --libs sdl2) $(shell $(PKGCONFIG) --libs SDL2_image) $(shell $(PKGCONFIG) --libs SDL2_ttf)
LIBS = $(shell $(PKGCONFIG) --cflags sdl2) 

SOURCES = common.c \
          player.c \
          game_state.c \
          winner.c \
          controller.c \
          game_play.c \
          main_game.c \
          game.c \
          main.c 
          

OBJECTS = $(SOURCES:.c=.o) $(RESFILE)
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


ifdef RESFILE
$(RESFILE): win32/ronda.rc
	$(WINDRES) -i $< -o $@
endif	

install:
	install -D -m755 ronda $(BINDIR)/ronda
	install -d -m755 $(GFXDIR)
	install -m755 data/gfx/* $(GFXDIR)
	install -d -m755 $(FONTSDIR)
	install -m755 data/fonts/* $(FONTSDIR)
	install -d -m755 $(CARDSDIR)
	install -m755 data/cards/* $(CARDSDIR)
	install -D -m755 data/ronda.desktop $(DESKTOPDIR)/ronda.desktop
	install -D -m775 data/ronda.png $(ICONDIR)/ronda.png

win32-installer: exec
	mkdir -p dist/installer
	cp win32/install.nsi dist/installer/install.nsi
	cp -r win32/dlls dist/dlls
	cp win32/ronda.ico dist/
	
	makensis dist/installer/install.nsi
	
	
uninstall:
	rm -rf $(BINDIR)/ronda $(DATADIR) $(ICONDIR)/ronda.png $(DESKTOPDIR)/ronda.desktop
	

clean:
	rm -rf *.o ronda ronda.exe dist
