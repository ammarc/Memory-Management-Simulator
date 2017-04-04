#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    memory->memory = new_list();

    // initialize the free holes list with 1 hole
    Node* head = malloc (sizeof(Node));
    Block* block = malloc (sizeof(Block));
    block->address = mem_size;
    block->size = mem_size;
    block->is_empty = true;
    head->data = block;
    memory->memory->head = head;

    return memory;
}

bool memory_is_full (Memory* memory)
{
    return memory->size_occupied == memory->total_size;
}

// given a process, it will be added to the memory
// (if there isn't enough space, it will be made by swapping)
void add_to_memory (Memory* memory, void* process, int curr_time)
{
    int address;
    /*
    while (!(address  = is_hole_available(memory->free_holes, process->memory_size)))
    {
        // call the swap function here
        // while !is_hole_available
        // find longest process in R.R. and take out
        // put on disk and delete from memroy
        // print the swapped out process
    }
    */

    // now we need to modify the data of the node we added to set
    // the time the process was in the memory
    ((Process *)process)->memory_entry_time = curr_time;

    // set the address of the processor
    // process->memory_address = address;
    list_add_end(memory->memory, process);
    fprintf(stderr, "Time is: %d\n", curr_time);
    list_print (memory->memory->head);
}


// the following needs to be changed depending upon the input algorithm
/* a modified (and partially incorrect) version of next-fit is shown below
int is_hole_available (List* free_holes, int process_size)
{
    Node* node = free_holes->head;

    while (node)
    {
        if (((Hole *) node)->size >= process_size)
        {
            return ((Hole *) node)->address;
        }
        node = node->next;
    }
    return 0;
}

void swap (List* round_robin, int curr_time, Memory* memory)
{
    // Remove magic number below:
    int longest_wait_time = 0;
    Process* longest_process_in_mem;
    Node* node = round_robin->head;
    double mem_usage;

    while (node)
    {
        if (curr_time - ((Process *)node)->memory_entry_time > longest_wait_time)
        {
            longest_wait_time = curr_time - ((Process *)node)->memory_entry_time;
            longest_process_in_mem = (Process *) node;
        }
        node = node->next;
    }

    // transfer the process from the memory to the disk

    mem_usage = memory->size_occupied*1.0/memory->total_size*1.0;


    fprintf (stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    curr_time, longest_process_in_mem->process_id, round_robin->size, 
                            memory->free_holes->size, (int) ceil(mem_usage));
}
*/