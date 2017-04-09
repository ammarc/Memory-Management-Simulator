/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#include "simulation.h"
#include "list.h"

extern char *optarg;

int main (int argc, char** argv)
{
	char input;
	char* algorithm_name;
	int mem_size;
	int quantum;
	FILE* input_file;
	List* processes_to_run = new_list();

	/** This part of the code was originally put on the LMS and was modified 
	  * for this assignment */
	while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		/* This is the process that will be used through out the simulation */
		Process* process_data_input = malloc(sizeof(Process));
		switch (input)
		{
			case 'f':
				input_file = fopen (optarg, "r");
				/* Looping till the end of the file while initializing the 
				 * process fromt the input data read */
				while (fscanf(input_file, "%d %d %d %d", &process_data_input->
															time_created, 
												&process_data_input->process_id, 
												&process_data_input->memory_size,
												&process_data_input->time_remaining)!=EOF)
				{
					process_data_input->added_to_disk = false;
					list_add_end(processes_to_run, process_data_input);
					/* Allocating memory for the next process */
					process_data_input = malloc(sizeof(Process));
				}
 				break;
           
			case 'a':
				algorithm_name = optarg;
				break;

			case 'm':
				mem_size = atoi(optarg);
				break;

			case 'q':
				quantum = atoi(optarg);
				break;

			default:
				break;
		}
		/* Freeing the lingering unused memory allocation when reading 
		 * with the -f option */
		free (process_data_input);
	}
	run_simulation (processes_to_run, mem_size, quantum, algorithm_name);
}
