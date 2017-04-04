#include <stdio.h>
#include "list.h"
#include "simulation.h"
#include "memory.h"
#include "disk.h"

void run_simulation (List* process_list, int mem_size, int quantum)
{
    int curr_time = 0;
	List* in_disk = new_list();
	List* round_robin_queue = new_list();

	Memory* memory = create_new_memory (mem_size);

	// incorrect condition for this while loop
	// while the process is not empty and etc
	while (!list_is_empty(process_list) || !list_is_empty (round_robin_queue))
	{
		Process* curr_process;

		// take out the processes from the input list and put them on disk
		while (process_list->head && ((Process *) process_list->head->data)->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			add_to_disk (in_disk, curr_process, curr_time);
		}



		// now we need to start taking the processes out of disk and put them in memory
		while (!list_is_empty (in_disk))
		{
			// the following just removes the process from the head
			// ties must be broken by priority
			fprintf (stderr, "Good till now\n");
			add_to_memory (memory, remove_from_disk (in_disk), curr_time);
			// add to R.R. queue (this needs to be done inside the memory)
			// list_add_end (round_robin_queue, curr_process);
			// remove from the disk
		}
		
		curr_time++;
	}
}