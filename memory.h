#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "simulation.h"

typedef struct memory
{
    int total_size;
    int size_occupied;
    int num_processes;
    int num_holes;
    List* memory;
} Memory;

// a structure for a hole in memory
typedef struct block
{
    // location is the end of the address
    int address;
    int size;
    bool is_empty;
} Block;

// creates a new memory and assigns the default values to it
Memory* create_new_memory (int mem_size);

// checks whether the memory is at capacity
bool memory_is_full (Memory* memory);

// adds a process to memory
void add_to_memory (Memory* memory, void* process, int curr_time);

// returns a free hole
// Block* hole_to_insert (Memory* memory, int process_size);

// checks the free holes list for whether the input space in available
// int is_hole_available (List* free_holes, int process_size);

// the function to swap processes out of memory
// void swap (List* round_robin, int curr_time, Memory* memory);

#endif