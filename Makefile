#  Project 1 for COMP30023: Computer Systems
#  at the University of Melbourne
#  Semester 1, 2017
#  by: Ammar Ahmed
#  Username: ammara
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC = driver.c list.c simulation.c memory.c disk.c
OBJ = driver.o list.o simulation.o memory.o disk.o
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
simulation.o: simulation.h list.h
memory.o:     memory.h list.h
disk.o:       disk.h list.h