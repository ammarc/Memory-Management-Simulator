/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#include "simulation.h"

void run_simulation (List* process_list, int mem_size, int quantum, 
												char* algorithm)
{
	/* initializing variables to be used through out the simulation */
    int curr_time = 0;
	int jump_time;
	List* in_disk = new_list();
	List* round_robin_queue = new_list();
	Process* process_to_deque = NULL;

	Memory* memory = create_new_memory (mem_size);

	/* if there is nothing in memory, nothing in disk and the process
	 * list that is read from the input is also empty, it means we
	 * are done */
	while (!memory_is_empty(memory) || !list_is_empty (in_disk) || 
						!list_is_empty(process_list))
	{
		Process* curr_process;

		/* this loop removes the processes from the process input list
		 * and then adds them to the disk, after checking their time */
		while (process_list->head && ((Process *) process_list->head->data)
									->time_created <= curr_time)
		{
			curr_process = list_remove_start(process_list);
			add_to_disk (in_disk, curr_process, curr_time);
		}

		/* if the disk isn't emtpy a process is picked and added to memory */
		if (!list_is_empty (in_disk))
			add_to_memory (memory, remove_from_disk (in_disk), curr_time, 
							in_disk, round_robin_queue, algorithm);

		/* if a process is set to deque and it is still in the memory
		 * it is put at the back of the round robin queue */
		if (process_to_deque && process_in_memory (memory, 
								process_to_deque->process_id))
		{
			list_add_end (round_robin_queue, list_remove_start 
										(round_robin_queue));
			process_to_deque = NULL;
		}


		/* this will happen in the case that time_remaining <= quantum 
		 * and the run_process runs nothing which the process ended 
		 * before its quantum expired */
		if (!list_is_empty(round_robin_queue))
			jump_time = ((Process *)round_robin_queue->head->data)->
														time_remaining;
		else
			jump_time = 1;

        /* the process is run here and the jump time is set if a process is 
		 * returned as that would mean its quantum has expired */
		if ((process_to_deque = run_process (round_robin_queue, quantum, 
															memory)))
			jump_time = quantum;

		curr_time += jump_time;
	}

	fprintf (stdout, "time %d, simulation finished.\n", curr_time);

	/* Freeing all the used lists */
	free_list (in_disk);
	free_list (round_robin_queue);
	free_memory (memory);
}

/* this function basically changes the time a process has left
 * and returns the process if its quantum expires */
Process* run_process (List* round_robin_queue, int quantum, Memory* memory)
{
	if (list_is_empty(round_robin_queue))
		return NULL;

	Process* process_to_run = round_robin_queue->head->data;
	if ( process_to_run->time_remaining - quantum > 0)
	{
		process_to_run->time_remaining -= quantum;
		return process_to_run;
	}
	else
	{
		/* delete the process from the round robin and also the memory */
		remove_from_memory (memory, process_to_run, round_robin_queue);
		free (process_to_run);
	}
	return NULL;
}