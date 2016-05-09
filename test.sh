#!/bin/sh
# +AMDG

make;
valgrind -v --leak-check=full --track-origins=yes ./minlib
