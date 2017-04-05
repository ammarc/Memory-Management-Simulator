/* ¡Add username and info on all files (see spec)!
*/

#include "simulation.h"
#include "list.h"

extern char *optarg;

int main(int argc, char** argv)
{
	char input;
	char* algorithm_name;
	int mem_size;
	int quantum;
	FILE* input_file;
	List* processes_to_run = new_list();
	
	fprintf(stderr, "\nStarting program...\n\n");

	// make this into a new function:
	while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		Process* process_data_input = malloc(sizeof(Process));

		switch (input)
		{
			case 'f':
				input_file = fopen (optarg, "r");
				while (fscanf(input_file, "%d %d %d %d", &process_data_input->time_created, 
												&process_data_input->process_id, 
												&process_data_input->memory_size, 
												&process_data_input->job_time)!=EOF)
				{
					list_add_end(processes_to_run, process_data_input);
					// Now that a node in the list points to this struct
					// we need to allocate a different chunk of memory
					process_data_input = malloc(sizeof(Process));
				}
 				break;
           
			case 'a':
				// set the value of size (int value) based on optarg
				algorithm_name = optarg;
				break;

			case 'm':
				// set the value of size (int value) based on optarg
				mem_size = atoi(optarg);
				break;

			case 'q':
				// set the value of size (int value) based on optarg
				quantum = atoi(optarg);
				break;

			default:
				// do something
				break;
		}

		free (process_data_input);
	}

	fprintf(stderr, "Applying algorithm: %s\n", algorithm_name);
	fprintf (stderr, "The mem_size is: %d\n", mem_size);
	fprintf (stderr, "The quantum is: %d\n\n\n", quantum);

	run_simulation (processes_to_run, mem_size, quantum);

	fprintf(stderr, "\nEnding program...\n\n");
}
