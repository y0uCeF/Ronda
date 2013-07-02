#!/bin/sh

export PATH=/usr/i486-mingw32/bin:$PATH
make CC=i486-mingw32-gcc PKGCONFIG=i486-mingw32-pkg-config WINDRES=i486-mingw32-windres \
PKG_CONFIG_PATH=/usr/i486-mingw32/lib/pkg-config/ EXECUTABLE=ronda.exe RESFILE=ronda.o INPLACE=y \
win32-installer
