#include <stdio.h>
#include "list.h"
#include "simulation.h"
#include "memory.h"

void run_simulation (List* process_list, int mem_size, int quantum)
{
    int curr_time = 0;
	List* in_disk = new_list();
	Memory* memory = create_new_memory (mem_size);

	// incorrect condition for this while loop
	while (!list_is_empty(process_list))
	{
		Process* curr_process;
		
		// take out the processes from the input list and put them on disk
		while (process_list->head && ((Process *) process_list->head->data)->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			list_add_end(in_disk, curr_process);
		}

		fprintf(stderr, "Time is: %d\n", curr_time);
		list_print (in_disk->head);

		// now we need to start taking the processes out of disk and put them in memory
		while (!memory_is_full (memory) && !list_is_empty (process_list))
		{
			list_add_end (memory->in_memory, list_remove_start (process_list));
		}

		curr_time++;
	}
}