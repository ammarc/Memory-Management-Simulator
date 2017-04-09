/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "list.h"
#include "simulation.h"
#include "disk.h"


typedef struct block Block;
typedef struct memory Memory;

/* Memory is our self-defined list to apply the round robin queue
 * and the free holes list without needing two separate lists */
struct memory
{
    int total_size;
    int size_occupied;
    int num_processes;
    int num_holes;
    Block* head;
    Block* last;
};

/* Memory is made up of blocks and each of these blocks contains
 * an address, a pointer to a process and also a value telling if
 * it is empty or not */
struct block
{
    // Note: address is taken from the end
    int address;
    int size;
    bool is_empty;
    Block* next;
    Block* back;
    Process* process;
};

/* creates a new memory and assigns the default values to it */
Memory* create_new_memory (int mem_size);

/* checks whether the memory is at capacity */
bool memory_is_full (Memory* memory);

/* checks whether memory is empty */
bool memory_is_empty (Memory* memory);

/* adds a process to memory */
void add_to_memory (Memory* memory, void* process, int curr_time, 
                        List* in_disk, List* round_robin_queue, 
                                                char* algorithm);

/* checks the memory segments list for contiguous space and returns
 * its address if found */
int is_space_available (Memory* memory, int process_size, char* algorithm);

/* returns the function to be swapped out */
Process* process_to_remove (Memory* memory, int curr_time);

/* this function removes the given process from memory and also the
 * round robin queue */
void remove_from_memory (Memory* memory, Process* process, 
                                        List* round_robin_queue);

/* given an address the given process is inserted into memory and 
 * also the round robin queue */
void insert_at_address (Memory* memory, int address, Process* process,
                                         List* round_robin_queue);

/* checks whether the given PID is in memory */
bool process_in_memory (Memory* memory, int pid);

/* frees the entire memory */
void free_memory (Memory* memory);

#endif