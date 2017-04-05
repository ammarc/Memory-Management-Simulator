#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "list.h"
#include "memory.h"
#include "simulation.h"
#include "disk.h"

Memory* create_new_memory (int mem_size)
{
    // Setting the default values for memory
    Memory* memory = malloc (sizeof(Memory));
    memory->total_size = mem_size;
    memory->num_holes = 1;
    memory->num_processes = 0;
    memory->size_occupied = 0;

    // initialize the first segment as free
    Block* block = malloc (sizeof(Block));
    block->address = mem_size;
    block->size = mem_size;
    block->is_empty = true;
    block->next = NULL;
    block->back = NULL;
    block->process = NULL;

    memory->head = block;

    return memory;
}

bool memory_is_full (Memory* memory)
{
    return memory->size_occupied == memory->total_size;
}

bool memory_is_empty (Memory* memory)
{
    return memory->head;
}

// given a process, it will be added to the memory
// (if there isn't enough space, it will be made by swapping)
void add_to_memory (Memory* memory, void* process, int curr_time, List* in_disk)
{
    int address;
    Process* to_be_removed;
    
    // keep going unless a big enough space is available
    while (!(address = is_space_available(memory, ((Process *)process)->memory_size)))
    {
        // call the swap function here
        to_be_removed = swap (memory, curr_time);
        // take the process out from R.R.
        // take the process out from memory segments and update the boolean
        remove_from_memory (memory, round_robin_queue, to_be_removed);
        // transfer to the disk
        add_to_disk (in_disk, to_be_removed, curr_time);
        // print the swapped out process
    }
    

    // now we need to modify the data of the block we added to set
    // the time the process was in the memory
    ((Process *)process)->memory_entry_time = curr_time;

    // set the address of the process
    ((Process *)process)->memory_address = address;

    // WRONG:
    // list_add_end(memory, process);

    // fprintf(stderr, "Time is: %d\n", curr_time);
    // list_print (memory->head);
}


// the following needs to be changed depending upon the input algorithm
// a modified version of next-fit is shown below
int is_space_available (Memory* memory, int process_size)
{
    Block* block = memory->head;

    while (block)
    {
        if (block->size >= process_size && block->is_empty)
        {
            return block->address;
        }
        block = block->next;
    }
    return 0;
}


void* swap (Memory* memory, int curr_time)
{
    int longest_wait_time = INT_MIN;
    int highest_priority = INT_MAX;
    Process* longest_process_in_mem;
    Block* block = memory->head;
    double mem_usage;

    while (block)
    {
        if (((Block *)block)->is_empty)
        {
            block = block->next;
            continue;
        }

        if (curr_time - ((Process *)block)->memory_entry_time > longest_wait_time)
        {
            highest_priority = ((Process *)block)->process_id;
            longest_wait_time = curr_time - ((Process *)block)->memory_entry_time;
            longest_process_in_mem = (Process *) block;
        }
        // break ties by priority
        else if (curr_time - ((Process *)block)->memory_entry_time == longest_wait_time)
        {
            if (((Process *)block)->process_id < highest_priority)
            {
                highest_priority = ((Process *)block)->process_id;
                longest_wait_time = curr_time - ((Process *)block)->memory_entry_time;
                longest_process_in_mem = (Process *) block;
            }
        }
        block = block->next;
    }

    // at this stage we know the process that is to be taken out of memory
    // now we need to transfer this process from the memory to the disk
    // update the memory segments and the round robin


    mem_usage = memory->size_occupied*1.0/memory->total_size*1.0;

    // WRONG (print the process swapped in, not out)
    fprintf (stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    curr_time, longest_process_in_mem->process_id, memory->total_size, 
                            memory->total_size, (int) ceil(mem_usage*100));
    return longest_process_in_mem;
}

void remove_from_memory (Memory* memory, List* round_robin_queue, void* process)
{
    int pid = ((Process *) process)->process_id;
    Block* block = memory->head;

    // first let us remove the process from the memory segment list
    // scan the entire list for the process with pid

    // NULL EVERYTHING
    while (block)
    {
        if (((Process *)block)->process_id == pid)
        {
            // l
        }
        block =  block->next;
    }
}