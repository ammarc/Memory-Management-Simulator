/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#include "disk.h"
#include "memory.h"
#include <assert.h>

Memory* create_new_memory (int mem_size)
{
    /* Setting the default values for memory */
    Memory* memory = malloc (sizeof(Memory));
    memory->total_size = mem_size;
    memory->num_holes = 1;
    memory->num_processes = 0;
    memory->size_occupied = 0;

    /* initialize the first segment as free */
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
    return memory->size_occupied == 0;
}

/* given a process, it will be added to the memory
 * (if there isn't enough space, it will be created by swapping) */
void add_to_memory (Memory* memory, void* process, int curr_time, 
                    List* in_disk, List* round_robin_queue, char* algorithm)
{
    int address;
    Process* to_be_removed;
    int mem_usage;

    /* this loop will run until a big enough block is available */
    while (!(address = is_space_available(memory, 
                            ((Process *)process)->memory_size, algorithm)))
    {
        /* this is the process to be swapped out of memory */
        to_be_removed = process_to_remove (memory, curr_time);

        remove_from_memory (memory, to_be_removed, round_robin_queue);
        
        /* the process are then added to disk */
        add_to_disk (in_disk, to_be_removed, curr_time);
    }

    /* now we need to modify the data of the process added to memory
     * with the current time and address */
    ((Process *)process)->memory_entry_time = curr_time;

    ((Process *)process)->memory_address = address;
    
    insert_at_address (memory, address, process, round_robin_queue);

    /* the percentage is calculated here and floating point numbers
     * are avoided due to their limitations */
    mem_usage = ((100 * memory->size_occupied) / memory->total_size) + 
                            (((100 * memory->size_occupied) % 
                                        memory->total_size) ? 1 : 0);

    fprintf (stdout, 
        "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    curr_time, ((Process *)process)->process_id, 
                            memory->num_processes, 
                            memory->num_holes, mem_usage);
}


/* this returns the address of the space available and changes
 * according to the input its gotten */
int is_space_available (Memory* memory, int process_size, char* algorithm)
{
    Block* block;
    /* this loop compares traverses the memory and returns the 
     * smallest free hole (which is big enough) and returns */
    if (strcmp (algorithm, "best") == 0)
    {
        Block* best_block = NULL;
        block = memory->head;
        int best_size = INT_MAX;
        while (block)
        {
            if (block->is_empty && block->size >= process_size)
            {
                if (block->size < best_size)
                {
                    best_size = block->size;
                    best_block = block;
                }
            }
            block = block->next;
        }
        if (best_block)
            return best_block->address;
        else
            return 0;
    }
    /* worst fit is applied here and the biggest hole (which can fit
     * in the process) is returned */
    else if (strcmp (algorithm, "worst") == 0)
    {
        Block* worst_block = NULL;
        block = memory->head;
        int worst_size = INT_MIN;
        while (block)
        {
            if (block->is_empty && block->size >= process_size)
            {
                if (block->size > worst_size)
                {
                    worst_size = block->size;
                    worst_block = block;
                }
            }
            block = block->next;
        }
        if (worst_block)
            return worst_block->address;
        else
            return 0;
    }
    /* finally if the conditials fail, first fit is applied here */
    block = memory->head;
    while (block)
    {
        if (block->is_empty && block->size >= process_size)
            return block->address;
        block = block->next;
    }
    return 0;
}

/* given memory, this function finds the next process to be removed
 * from the memory */
Process* process_to_remove (Memory* memory, int curr_time)
{   
    int longest_wait_time = INT_MIN;
    int highest_priority = INT_MAX;
    Process* longest_process_in_mem;
    Block* block = memory->head;

    /* memory is traversed here and the block with the process that has
     * been in it the longest is set to be removed. If we find ourselves
     * in a tie for the longest time, it is broken by priority */
    while (block)
    {
        if (((Block *)block)->is_empty)
        {
            block = block->next;
            continue;
        }

        if (curr_time - ((Process *)block->process)->memory_entry_time > 
                                                    longest_wait_time)
        {
            highest_priority = ((Process *)block->process)->process_id;
            longest_wait_time = curr_time - ((Process *)block->process)->
                                                    memory_entry_time;
            longest_process_in_mem = (Process *) block->process;
        }
        /* break ties by priority */
        else if (curr_time - ((Process *)block->process)->
                            memory_entry_time == longest_wait_time)
        {
            if (((Process *)block->process)->process_id < highest_priority)
            {
                highest_priority = ((Process *)block->process)->process_id;
                longest_process_in_mem = (Process *) block->process;
            }
        }
        block = block->next;
    }
    return longest_process_in_mem;
}

/* given a process, this function removes it from memory and also
 * from the round robin queue */
void remove_from_memory (Memory* memory, Process* process, 
                                        List* round_robin_queue)
{
    int pid = process->process_id;
    Block* block = memory->head;
    int count = 0;

    /* memory is traversed and the block with the input process ID is
     * removed */
    while (block)
    {
        if (!block->is_empty && block->process->process_id == pid)
        {
            block->is_empty = true;
            block->process = NULL;
            
            memory->size_occupied -= process->memory_size;
            
            memory->num_processes -= 1;            

            /* checking the case where the back node is empty
             * and if so it is merged */
            if (block->back && block->back->is_empty)
            {
                count += 1;
                block->address = block->back->address;
                block->size += block->back->size;

                // checking for the head
                if (memory->head == block->back)
                    memory->head = block;

                if (block->back->back)
                    block->back->back->next = block;

                Block* node_to_free = block->back;
                block->back = block->back->back;
                
                free (node_to_free);
            }

            /* checks with the next node, and if it is empty
             * it is merged with the current node */
            if (block->next && block->next->is_empty)
            {
                count += 1;
                Block* node_to_free = block->next;
                
                block->size += block->next->size;
                // checking for the head
                if (memory->last == block->next)
                    memory->last = block;

                if (block->next->next)
                    block->next->next->back = block;

                block->next = block->next->next;

                // free the next node as well
                free (node_to_free);
            }
            /* this will only be true when both the left and right are empty
             * and then the number of holes will increase */
            if (count == 0)
                memory->num_holes += 1;
            else if (count == 2)
                // one less hole left
                memory->num_holes -= 1;
            break;
        }
        block = block->next;
    }
    list_remove_process (round_robin_queue, process->process_id);
}

/* given a process it is added at the given address in memory */
void insert_at_address (Memory* memory, int address, Process* process, 
                            List* round_robin_queue)
{
    Block* block = memory->head;
    
    /* traverse the memory to find the correct address to insert */
    while (block)
    {
        if (block->address == address)
        {
            if (block->size == process->memory_size)
                memory->num_holes -= 1;

            else if (block->size > process->memory_size)
            {
                /* split the free space and make a new block */
                Block* new_block = malloc (sizeof(Block));

                new_block->next = block->next;

                if (block->next)
                    block->next->back = new_block;
                
                /* making a new block and inserting it into memory */
                block->next = new_block;
                new_block->back = block;
                new_block->size = block->size - process->memory_size;
                new_block->is_empty = true;
                new_block->address = address - process->memory_size;
                
                block->size = process->memory_size;

                /* check whether the block was the last one */
                if (memory->last == block)
                    memory->last = new_block;
            }
            block->is_empty = false;
            block->process = process;

            memory->size_occupied += process->memory_size;
            memory->num_processes += 1;
        }
        block = block->next;
    }
    list_add_end (round_robin_queue, process);
}

/* Printing the memory list */
void memory_print (Block* head)
{
    if (head)
	{
        fprintf(stderr, "The PID of the process is: %d\n", 
                            ((Block *)head)->process->process_id);
        memory_print(head->next);
    }
}

bool process_in_memory (Memory* memory, int pid)
{
    // check the memory to see if the process is in it or not
    Block* block = memory->head;

    while (block)
    {
        if (!block->is_empty && block->process->process_id == pid)
            return true;
        block = block->next;
    }
    return false;
}

/* This frees all the nodes in the memory */
void free_memory (Memory* memory)
{
	assert(memory != NULL);
	// free each node
	Block *block = memory->head;
	Block *next;
	while (block)
	{
		next = block->next;
		free (block);
		block = next;
	}
	// free the list struct itself
	free(memory);
}