#include <stdio.h>
#include "list.h"

typedef struct memory
{
    int total_size;
    int size_occupied;
    int num_processes;
    int num_holes;
    List* in_memory;
} Memory;


// creates a new memory and assigns the default values to it
Memory* create_new_memory (int mem_size);

// checks whether the memory is at capacity
bool memory_is_full (Memory* memory);