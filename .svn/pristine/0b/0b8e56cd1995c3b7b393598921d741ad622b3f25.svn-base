#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "memory.h"

Memory* create_new_memory (int mem_size)
{
    // Setting the default values for memory
    Memory* memory = malloc (sizeof(Memory));
    memory->total_size = mem_size;
    memory->num_holes = 1;
    memory->num_processes = 0;
    memory->size_occupied = mem_size;
    memory->in_memory = new_list();

    return memory;
}

bool memory_is_full (Memory* memory)
{
    return memory->size_occupied == memory->total_size;
}
