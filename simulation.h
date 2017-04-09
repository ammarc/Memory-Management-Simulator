/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdbool.h>

/* defining a process struct */
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

/* this runs the core simulation */
void run_simulation (List* process_list, int mem_size, int quantum, 
										char* algorithm);

/* this runs the process and returns the process to deque */
Process* run_process (List* round_robin_queue, int quantum, Memory* memory);

#endif