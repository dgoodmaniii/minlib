#+AMDG
#
#This document was begun on 9 May 1200, the feast of St.
#Gregory Nazianzen, ECD, and it is humbly dedicated to
#him, to St.  Wulfric of Haselbury, and to the Immaculate
#Heart of Mary for their prayers, and to the Sacred Heart
#of Jesus for His mercy.

minlib :	main.c utility.o readlib.o format_recs.o gui.o
	gcc -o minlib main.c utility.o readlib.o format_recs.o gui.o

gui.o : gui.c
	gcc -c gui.c -lform -lmenu -lncurses

format_recs.o : format_recs.c utility.c
	gcc -c format_recs.c utility.c

readlib.o : readlib.c utility.c
	gcc -c readlib.c utility.c

utility.o : utility.c
	gcc -c utility.c
