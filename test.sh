#!/bin/sh
# +AMDG

gcc -o minlib main.c;
valgrind -v --leak-check=full --track-origins=yes ./minlib
