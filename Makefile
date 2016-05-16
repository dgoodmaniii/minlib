#+AMDG
#
#This document was begun on 9 May 1200, the feast of St.
#Gregory Nazianzen, ECD, and it is humbly dedicated to
#him, to St.  Wulfric of Haselbury, and to the Immaculate
#Heart of Mary for their prayers, and to the Sacred Heart
#of Jesus for His mercy.

SHELL=/bin/sh
.SUFFIXES:
.SUFFIXES: .c .o .h
CC=gcc
CFLAGS=-I -Wall
OBJ=utility.o readfile.o readlib.o format_recs.o gui.o
SRCDIR=./
CURSLIB=-lform -lmenu -lncurses
prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(prefix)/bin
includedir=$(prefix)/include
datarootdir=$(prefix)/share
datadir=$(datarootdir)
docdir=$(datarootdir)/doc/minlib
htmldir=$(docdir)
mandir=$(datarootdir)/man

all : minlib

minlib :	main.c $(OBJ) errcodes.h
	$(CC) $(CFLAGS) -o minlib main.c $(OBJ) $(CURSLIB)

gui.o : gui.c errcodes.h
	$(CC) $(CFLAGS) -c gui.c $(CURSLIB)

format_recs.o : format_recs.c utility.c errcodes.h
	$(CC) $(CFLAGS) -c format_recs.c utility.c

readlib.o : readlib.c utility.c errcodes.h
	$(CC) $(CFLAGS) -c readlib.c utility.c

readfile.o : readfile.c errcodes.h utility.c
	$(CC) $(CFLAGS) -c readfile.c utility.c

utility.o : utility.c errcodes.h
	$(CC) $(CFLAGS) -c utility.c

.PHONY : clean
clean :
	rm $(OBJ) minlib
