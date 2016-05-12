#+AMDG
#
#This document was begun on 9 May 1200, the feast of St.
#Gregory Nazianzen, ECD, and it is humbly dedicated to
#him, to St.  Wulfric of Haselbury, and to the Immaculate
#Heart of Mary for their prayers, and to the Sacred Heart
#of Jesus for His mercy.

minlib :	main.c utility.o readlib.o format_recs.o gui.o errcodes.h
	gcc -o minlib main.c utility.o readlib.o format_recs.o gui.o

gui.o : gui.c errcodes.h
	gcc -c gui.c -lform -lmenu -lncurses

format_recs.o : format_recs.c utility.c errcodes.h
	gcc -c format_recs.c utility.c

readlib.o : readlib.c utility.c errcodes.h
	gcc -c readlib.c utility.c

utility.o : utility.c errcodes.h
	gcc -c utility.c
