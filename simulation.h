#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdbool.h>

typedef struct process
{
	int time_created;
	int process_id;
	int memory_size;
	int time_remaining;
	int memory_address;
	int disk_entry_time;
	int memory_entry_time;
	bool added_to_disk;
} Process;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "memory.h"
#include "disk.h"


void run_simulation (List* process_list, int mem_size, int quantum, char* algorithm);

void process_copy (Process* src, Process* dest);

// runs the process and return the time to 'jump'
Process* run_process (List* round_robin_queue, int quantum, Memory* memory);

#endif