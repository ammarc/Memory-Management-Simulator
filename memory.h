#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include "list.h"

typedef struct memory
{
    int total_size;
    int size_occupied;
    int num_processes;
    int num_holes;
    List* free_holes;
} Memory;

// a structure for a hole in memory
typedef struct hole
{
    // location is the end of the address
    int location;
    int size;
} Hole;

// creates a new memory and assigns the default values to it
Memory* create_new_memory (int mem_size);

// checks whether the memory is at capacity
bool memory_is_full (Memory* memory);

// adds a process to memory
void add_to_memory (List* queue, void* process, int curr_time);

#endif