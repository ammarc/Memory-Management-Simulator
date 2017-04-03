#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "memory.h"
#include "simulation.h"

Memory* create_new_memory (int mem_size)
{
    // Setting the default values for memory
    Memory* memory = malloc (sizeof(Memory));
    memory->total_size = mem_size;
    memory->num_holes = 1;
    memory->num_processes = 0;
    memory->size_occupied = mem_size;
    memory->free_holes = new_list();

    // initialize the free holes list with 1 hole
    Node* head = malloc (sizeof(Node));
    Hole* hole = malloc (sizeof(hole));
    hole->location = mem_size;
    hole->size = mem_size;
    head->data = hole;
    memory->free_holes->head = head;

    return memory;
}

bool memory_is_full (Memory* memory)
{
    return memory->size_occupied == memory->total_size;
}

void add_to_memory (List* queue, void* process, int curr_time)
{
    list_add_end(queue, process);

    // now we need to modify the data of the node we added to set
    // the time the process was in the memory
    ((Process *) queue->last->data)->disk_entry_time = curr_time;
}