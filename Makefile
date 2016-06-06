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
CFLAGS=-I -Wall -g
OBJ=utility.o read_optfile.o readfile.o readlib.o format_recs.o \
	gui.o full_search.o add_files.o
SRCDIR=./
CURSLIB=-lform -lmenuw -lncursesw
EXTLIB=-lextractor
prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(prefix)/bin
includedir=$(prefix)/include
datarootdir=$(prefix)/share
datadir=$(datarootdir)
docdir=$(datarootdir)/doc/minlib
mandir=$(datarootdir)/man
binfiles=minlib
manfiles=minlib.1
othdocs=minlib_man.html

all : minlib doc

minlib :	main.c $(OBJ) errcodes.h options.h
	$(CC) $(CFLAGS) -o minlib main.c $(OBJ) $(CURSLIB) $(EXTLIB)

gui.o : gui.c errcodes.h
	$(CC) $(CFLAGS) -c gui.c $(CURSLIB)

full_search.o : full_search.c errcodes.h
	$(CC) $(CFLAGS) -c full_search.c $(CURSLIB)

format_recs.o : format_recs.c utility.c errcodes.h
	$(CC) $(CFLAGS) -c format_recs.c utility.c

readlib.o : readlib.c utility.c errcodes.h
	$(CC) $(CFLAGS) -c readlib.c utility.c

readfile.o : readfile.c errcodes.h utility.c
	$(CC) $(CFLAGS) -c readfile.c utility.c

read_optfile.o : read_optfile.c errcodes.h utility.c options.h
	$(CC) $(CFLAGS) -c read_optfile.c utility.c

add_files.o : add_files.c utility.c
	$(CC) $(CFLAGS) -c add_files.c utility.c $(EXTLIB)

utility.o : utility.c errcodes.h
	$(CC) $(CFLAGS) -c utility.c

install :
	cp $(binfiles) $(bindir)
	cp $(manfiles) $(mandir)/man1
	test -d $(docdir) || mkdir -p $(docdir)
	cp $(othdocs) $(docdir)

uninstall :
	rm $(bindir)/$(binfiles)
	rm $(mandir)/man1/$(manfiles)
	rm -R $(docdir)

doc :
	groff -man -Tascii -a minlib.1 > minlib_man.txt;
	groff -man -Thtml minlib.1 > minlib_man.html;

.PHONY : clean
clean :
	rm $(OBJ) minlib
