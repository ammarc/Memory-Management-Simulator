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

	// while the process is not empty and etc
	// check for the case that the last process is being processed
	// while (!list_is_empty(process_list) || !memory_is_empty(memory))
	// change this:
	while (curr_time < 100)
	{
		Process* curr_process;

		// take out the processes from the input list and put them on disk
		while (process_list->head && ((Process *) process_list->head->data)->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			// fprintf (stderr, ":-----------PID-----------: %d\n", curr_process->process_id);
			add_to_disk (in_disk, curr_process, curr_time);
		}

		// fprintf(stderr, "Time is: %d\n", curr_time);
		// list_print(in_disk->head);

		// now we need to select a process from the disk and put it in the memory
		// fprintf (stderr, ":-----------PID-----------: %d\n", ((Process *)in_disk->head->data)->process_id);
		// curr_process = remove_from_disk (in_disk);
		// fprintf (stderr, ":-----------PID-----------: %d\n", curr_process->process_id);
		if (!list_is_empty (in_disk))
			add_to_memory (memory, remove_from_disk (in_disk), curr_time, in_disk, round_robin_queue);
		
		curr_time++;
	}
	fprintf (stdout, "time %d, simulation finished", curr_time);
}

void process_copy (Process* src, Process* dest)
{
	dest->time_created = src->time_created;
	dest->process_id = src->process_id;
	dest->memory_size = src->memory_size;
	dest->job_time = src->job_time;
	dest->time_remaining = src->time_remaining;
	dest->memory_address = src->memory_address;
	dest->disk_entry_time = src->disk_entry_time;
	dest->memory_entry_time = src->memory_entry_time;
}