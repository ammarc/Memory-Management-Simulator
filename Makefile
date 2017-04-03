# 
# Sample Makefile for project 1 
#
# The program used for illustration purposes is
# a simple program using prime numbers
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC = driver.c list.c simulation.c memory.c
OBJ = driver.o list.o simulation.o memory.o
EXE = swap

## Top level target is executable.
$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
	rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
	rm $(EXE) 

## Dependencies

driver.o:	  simulation.h
list.o:		  list.h
simulation.o: simulation.h
memory.o:     memory.h