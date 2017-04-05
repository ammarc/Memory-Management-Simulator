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
    memory->last = block;

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
void add_to_memory (Memory* memory, void* process, int curr_time, List* in_disk, List* round_robin_queue)
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
        remove_from_memory (memory, to_be_removed, round_robin_queue);
        // transfer to the disk
        add_to_disk (in_disk, to_be_removed, curr_time);
        // print the swapped out process
    }
    

    // now we need to modify the data of the block we added to set
    // the time the process was in the memory
    ((Process *)process)->memory_entry_time = curr_time;

    // set the address of the process
    ((Process *)process)->memory_address = address;

    // insert the process to the address calculated
    insert_at_address (memory, address, process, round_robin_queue);

    // fprintf(stderr, "Time is: %d\n", curr_time);
    // list_print (memory->head);

    /* WRONG (print the process swapped in, not out)
    fprintf (stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    curr_time, longest_process_in_mem->process_id, memory->total_size, 
                            memory->total_size, (int) ceil(mem_usage*100));
    */
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


    return longest_process_in_mem;
}

void remove_from_memory (Memory* memory, void* process, List* round_robin_queue)
{
    int pid = ((Process *) process)->process_id;
    Block* block = memory->head;

    // first let us remove the process from the memory segment list
    // scan the entire list for the process with pid

    while (block)
    {
        if (((Process *)block)->process_id == pid)
        {
            block->is_empty = true;
            block->process = NULL;

            memory->size_occupied -= ((Process *) process)->memory_size;
            memory->num_holes += 1;
            memory->num_processes += 1;            

            // merge with back node
            if (block->back && block->back->is_empty)
            {
                block->address = block->back->address;
                block->size += block->back->size;
                block->back = block->back->back;

                memory->num_holes -= 1;
                // checking for the last
                if (memory->last == block->back)
                {
                    memory->last = block;
                }

                // free the back node as well
                free (block->back);
            }

            // merge with the next node
            if (block->next && block->next->is_empty)
            {
                block->size += block->next->size;
                block->next = block->next->next;

                memory->num_holes -= 1;
                // checking for the head
                if (memory->head == block->next)
                {
                    memory->head = block;
                }
                // free the next node as well
                free (block->next);
            }
        }
        block =  block->next;
    }
}

void insert_at_address (Memory* memory, int address, Process* process, List* round_robin_queue)
{
    Block* block = memory->head;

    while (block)
    {
        if (block->address == address)
        {
            if (block->size > process->memory_size)
            {
                // split the free space and make a new block
                Block* new_block = malloc (sizeof(Block));
                new_block->next = block->next;
                block->next = new_block;
                new_block->back = block;
                new_block->size = block->size - process->memory_size;
                new_block->is_empty = true;
                new_block->address = address - process->memory_size;

                block->size = process->memory_size;

                // check whether the block was the last one
                if (memory->last == block)
                {
                    memory->last = new_block;
                }
            }
            block->is_empty = false;
            block->process = process;

            memory->size_occupied += process->memory_size;
            memory->num_processes += 1;

            if (block->size == process->memory_size)
            {
                memory->num_holes -= 1;
            }
        }
        block = block->next;
    }

    list_add_end (round_robin_queue, process);
}