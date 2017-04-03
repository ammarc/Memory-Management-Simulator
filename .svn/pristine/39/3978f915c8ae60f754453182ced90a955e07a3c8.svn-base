/* Made by Ammar
*/

#include "prime.h"
#include "list.h"

extern char *optarg;

typedef struct process_data
{
	int time_created;
	int process_id;
	int memory_size;
	int job_time;
} Process;

int main(int argc, char** argv)
{
	int time = 0;
	char input;
	char* algorithm_name;
	int memsize;
	int quantum;
	FILE* input_file;
	List* process_to_run = new_list();
	
	fprintf(stderr, "\nStarting program...\n\n");

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
					fprintf(stderr, "Your struct is stored at: %p\n", process_data_input);
					list_add_end(process_to_run, process_data_input);
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
				memsize = atoi(optarg);
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

	
	fprintf(stderr, "\nNow printing the list:\n");
	int curr_list_length = list_size(process_to_run);
	for (int i = 0; i < curr_list_length; i++)
	{
		fprintf(stderr, "Process time is: %d\n", * (int *) list_remove_start(process_to_run));
	}
	fprintf(stderr, "......................\n\n");


	fprintf(stderr, "Applying algorithm: %s\n", algorithm_name);
	fprintf (stderr, "The memsize is: %d\n", memsize);
	fprintf (stderr, "The quantum is: %d\n", quantum);

	// let us add a while loop
	while (!list_is_empty(process_to_run))
	{
		// take it out from the list
		
		time++;
	}
}
