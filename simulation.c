#include <stdio.h>
#include "list.h"
#include "simulation.h"
#include "memory.h"
#include "disk.h"

void run_simulation (List* process_list, int mem_size, int quantum)
{
    int curr_time = 0;
	List* in_disk = new_list();
	// List* round_robin_queue = new_list();

	Memory* memory = create_new_memory (mem_size);

	// while the process is not empty and etc
	// check for the case that the last process is being processed
	while (!list_is_empty(process_list) || !memory_is_empty(memory))
	{
		Process* curr_process;

		// take out the processes from the input list and put them on disk
		while (process_list->head && ((Process *) process_list->head->data)->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			add_to_disk (in_disk, curr_process, curr_time);
		}


		// now we need to select a process from the disk and put it in the memory
		add_to_memory (memory, remove_from_disk (in_disk), curr_time, in_disk);
		
		curr_time++;
	}
}