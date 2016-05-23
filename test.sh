#!/bin/sh
# +AMDG

make;
valgrind -v --leak-check=full --track-origins=yes ./minlib -a "greekfathers00fortgoog.pdf"
#valgrind -v --leak-check=full --track-origins=yes ./minlib
