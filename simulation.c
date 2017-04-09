#include "simulation.h"

void run_simulation (List* process_list, int mem_size, int quantum, char* algorithm)
{
    int curr_time = 0;
	List* in_disk = new_list();
	int jump_time;
	List* round_robin_queue = new_list();
	Process* process_to_deque = NULL;

	Memory* memory = create_new_memory (mem_size);

	// while the process is not empty and etc
	// check for the case that the last process is being processed
	while (!memory_is_empty(memory) || !list_is_empty (in_disk) || !list_is_empty(process_list))
	// change this:
	// while (curr_time < 120)
	{
		fprintf(stderr, "\n***************************************************\n");
		Process* curr_process;

		// take out the processes from the input list and put them on disk
		while (process_list->head && ((Process *) process_list->head->data)->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			// fprintf (stderr, ":-----------PID-----------: %d\n", curr_process->process_id);
			add_to_disk (in_disk, curr_process, curr_time);
		}

		if (!list_is_empty (in_disk))
			add_to_memory (memory, remove_from_disk (in_disk), curr_time, in_disk, round_robin_queue, algorithm);

		// what if taken out of memory
		if (process_to_deque && process_in_memory (memory, process_to_deque->process_id))
		{
			// put the process at the end of the round robin
			list_add_end (round_robin_queue, list_remove_start (round_robin_queue));
			process_to_deque = NULL;
		}
		// fprintf (stderr, ":-----------PID-----------:\n");
		// this will happen in the case that time_remaining <= quantum
		if (!list_is_empty(round_robin_queue))
			jump_time = ((Process *)round_robin_queue->head->data)->time_remaining;
		else
			// then only jump by 1
			jump_time = 1;
		// fprintf(stderr, "The jump time is %d\n", jump_time);
		//if (!list_is_empty(round_robin_queue) && (process_to_deque = run_process (round_robin_queue, quantum, memory)))
		//	jump_time = quantum;
		if ((process_to_deque = run_process (round_robin_queue, quantum, memory)))
			jump_time = quantum;
		// if (!list_is_empty(round_robin_queue))
		//	remove_from_memory (memory, (Process *) round_robin_queue->head->data, round_robin_queue);
		// jump_time = quantum;
		curr_time += jump_time;
		fprintf(stderr, "\n***************************************************\n");
	}

	fprintf (stdout, "time %d, simulation finished.\n", curr_time);
}

Process* run_process (List* round_robin_queue, int quantum, Memory* memory)
{
	if (list_is_empty(round_robin_queue))
		return NULL;
	fprintf(stderr, "Head of q: %d\n", ((Process *)round_robin_queue->head->data)->process_id);
	Process* process_to_run = round_robin_queue->head->data;
	if ( process_to_run->time_remaining - quantum > 0)
	{
		process_to_run->time_remaining -= quantum;
		fprintf(stderr, "\nRunning process: %d\nTime left for process %d: %d\n", process_to_run->process_id, process_to_run->time_remaining, process_to_run->process_id);
		return process_to_run;
	}
	else
	{
		// delete the process from the round robin and also the memory
		remove_from_memory (memory, process_to_run, round_robin_queue);
		free (process_to_run);
	}
	return NULL;
}