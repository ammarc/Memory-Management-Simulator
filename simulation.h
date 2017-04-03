#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

typedef struct process_data
{
	int time_created;
	int process_id;
	int memory_size;
	int job_time;
	int time_remaining;
} Process;

void run_simulation (List* process_list, int mem_size, int quantum);