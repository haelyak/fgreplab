#
# Sample Makefile for the fgrep lab.
#

#
# Explicitly specify the shell to work around inconsistencies in "make".
#
SHELL   =       /bin/sh

#
# Ask for debugging and warnings.
#
CFLAGS  =       -g -Og -Wall 

all:    fgrep

fgrep:        fgrep.c
	$(CC) $(CFLAGS) -o fgrep fgrep.c

clean:
	rm -f fgrep