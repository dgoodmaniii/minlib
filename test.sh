#!/bin/sh
# +AMDG

make;
valgrind -v --leak-check=full --track-origins=yes ./minlib -r"%30t || %20a || %4l || %10q" 2> errors
#valgrind -v --leak-check=full --track-origins=yes ./minlib
