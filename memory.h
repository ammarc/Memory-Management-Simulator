#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "simulation.h"


typedef struct block Block;
typedef struct memory Memory;

// Memory is our self-defined list to apply the round robin queue
// and the free holes list
struct memory
{
    int total_size;
    int size_occupied;
    int num_processes;
    int num_holes;
    Block* head;
    Block* last;
};

// a structure for a hole in memory
struct block
{
    // location is the end of the address
    int address;
    int size;
    bool is_empty;
    Block* next;
    Block* back;
    Process* process;
};

// creates a new memory and assigns the default values to it
Memory* create_new_memory (int mem_size);

// checks whether the memory is at capacity
bool memory_is_full (Memory* memory);

bool memory_is_empty (Memory* memory);

// adds a process to memory
void add_to_memory (Memory* memory, void* process, int curr_time, List* in_disk, List* round_robin_queue);

// returns a free hole
// Block* hole_to_insert (Memory* memory, int process_size);

// checks the memory segments list for contiguous space as the input
int is_space_available (Memory* memory, int process_size);

// the function to swap processes out of memory
Process* process_to_remove (Memory* memory, int curr_time);

// this function removes the given process from memory
void remove_from_memory (Memory* memory, Process* process, List* round_robin_queue);

// Implement: a function that fetches the next process
void* fetch_process ();

void insert_at_address (Memory* memory, int address, Process* process, List* round_robin_queue);

void memory_print (Block* head);

#endif