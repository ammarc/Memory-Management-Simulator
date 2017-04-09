#include "disk.h"
#include "memory.h"

Memory* create_new_memory (int mem_size)
{
    // Setting the default values for memory
    fprintf(stderr, "Creating new memory\n");
    Memory* memory = malloc (sizeof(Memory));
    memory->total_size = mem_size;// - 1;
    memory->num_holes = 1;
    memory->num_processes = 0;
    memory->size_occupied = 0;

    // initialize the first segment as free
    Block* block = malloc (sizeof(Block));
    // change this to -1
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

// given a process, it will be added to the memory
// (if there isn't enough space, it will be made by swapping)
void add_to_memory (Memory* memory, void* process, int curr_time, List* in_disk, List* round_robin_queue, char* algorithm)
{
    int address;
    Process* to_be_removed;
    double mem_usage;
    int count = 0;

    // keep going unless a big enough space is available
    while (!(address = is_space_available(memory, ((Process *)process)->memory_size, algorithm)))
    // while (count < 150)
    {
        // fprintf(stderr, "GOOD %d\n", count);
        // call the swap function here
        // fprintf(stderr, "Good till now\n");
        to_be_removed = process_to_remove (memory, curr_time);
        fprintf (stderr, "The process address to be removed: %d\n", to_be_removed->memory_address);
        // take the process out from R.R.
        // take the process out from memory segments and update the boolean
        remove_from_memory (memory, to_be_removed, round_robin_queue);
        
        // transfer to the disk
        add_to_disk (in_disk, to_be_removed, curr_time);
        // fprintf(stderr, "The changed value is: %d\n", address);
        count++;
    }

    // now we need to modify the data of the block we added to set
    // the time the process was in the memory
    ((Process *)process)->memory_entry_time = curr_time;

    // set the address of the process
    ((Process *)process)->memory_address = address;
    
    // insert the process to the address calculated
    insert_at_address (memory, address, process, round_robin_queue);

    fprintf(stderr, "\n");
    mem_usage = memory->size_occupied*1.0/memory->total_size*1.0;
    fprintf(stderr, "Memory usage is is: %f\n", mem_usage);
    fprintf(stderr, "Memory size is: %d\n", memory->size_occupied);
    fprintf(stderr, "Memory total size is: %d\n", memory->total_size);
    fprintf (stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    curr_time, ((Process *)process)->process_id, memory->num_processes, 
                            memory->num_holes, (int) ceil(mem_usage*100));
}


// the following needs to be changed depending upon the input algorithm
// which is taken from its input
int is_space_available (Memory* memory, int process_size, char* algorithm)
{
    Block* block;
    if (strcmp (algorithm, "best") == 0)
    {
        // fprintf(stdout, "running best\n");
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

    block = memory->head;
    while (block)
    {
        if (block->is_empty && block->size >= process_size)
            return block->address;
        block = block->next;
    }
    return 0;
}


Process* process_to_remove (Memory* memory, int curr_time)
{
    
    int longest_wait_time = INT_MIN;
    int highest_priority = INT_MAX;
    Process* longest_process_in_mem;
    Block* block = memory->head;

    while (block)
    {
        if (((Block *)block)->is_empty)
        {
            block = block->next;
            continue;
        }

        if (curr_time - ((Process *)block->process)->memory_entry_time > longest_wait_time)
        {
            highest_priority = ((Process *)block->process)->process_id;
            longest_wait_time = curr_time - ((Process *)block->process)->memory_entry_time;
            longest_process_in_mem = (Process *) block->process;
        }
        // break ties by priority
        else if (curr_time - ((Process *)block->process)->memory_entry_time == longest_wait_time)
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

void remove_from_memory (Memory* memory, Process* process, List* round_robin_queue)
{
    int pid = process->process_id;
    Block* block = memory->head;
    int count = 0;
    fprintf(stderr, "Now removing process: %d\n", process->process_id);
    // first let us remove the process from the memory segment list
    // scan the entire list for the process with pid
    while (block)
    {
        if (!block->is_empty && block->process->process_id == pid)
        {
            block->is_empty = true;
            block->process = NULL;
            
            memory->size_occupied -= process->memory_size;
            
            memory->num_processes -= 1;            

            // merge with back node
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
                // free the back node as well
                free (node_to_free);
            }

            // merge with the next node
            if (block->next && block->next->is_empty)
            {
                count += 1;
                Block* node_to_free = block->next;
                
                block->size += block->next->size;
                // fprintf(stderr, "Next size is: %d\n", block->size);
                // checking for the head
                if (memory->last == block->next)
                    memory->last = block;

                if (block->next->next)
                    block->next->next->back = block;

                block->next = block->next->next;

                // free the next node as well
                free (node_to_free);
            }
            // this will only be true when both the left and right are merged
            if (count == 0)
                // one more hole left as no merge happened
                memory->num_holes += 1;
            else if (count == 2)
                // one less hole left
                memory->num_holes -= 1;
            break;
        }
        block = block->next;
    }
    // removing from the round robin queue (not sure of this)
    list_remove_process (round_robin_queue, process->process_id);
}

void insert_at_address (Memory* memory, int address, Process* process, List* round_robin_queue)
{
    Block* block = memory->head;
    // int length = memory->num_holes + memory->num_processes;
    fprintf (stderr, "Inserting at address %d\n", address);
    
    while (block)
    {
        if (block->address == address)
        {
            if (block->size == process->memory_size)
                memory->num_holes -= 1;

            else if (block->size > process->memory_size)
            {
                // split the free space and make a new block
                Block* new_block = malloc (sizeof(Block));

                new_block->next = block->next;
                if (block->next)
                    block->next->back = new_block;
                block->next = new_block;
                new_block->back = block;
                new_block->size = block->size - process->memory_size;
                new_block->is_empty = true;
                new_block->address = address - process->memory_size;
                
                block->size = process->memory_size;

                // check whether the block was the last one
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
        // if (head->process) { fprintf (stderr, "HEAD FOUND\n"); }
        fprintf(stderr, "The PID of the process is: %d\n", ((Block *)head)->process->process_id);
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