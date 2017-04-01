/* Made by Ammar
*/

#include "prime.h"
#include "list.h"

extern char *optarg;

int main(int argc, char** argv)
{
	char input;
	char* algorithm_name;
	int memsize;
	int quantum;
	FILE* input_file;
	List* process_to_run = new_list();
	
	fprintf(stderr, "\n");
	fprintf(stderr, "Starting program...\n");

	while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		// remove magic number
		int line[4];
		switch (input)
		{
			case 'f':
				input_file = fopen (optarg, "r");
				while (fscanf(input_file, "%d %d %d %d", line, line+1, line+2, line+3)!=EOF)
				{
					fprintf(stderr, "Now adding time %d\n", line[0]);
					list_add_end(process_to_run, line);
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
	}

	
	fprintf(stderr, "Now printing the list:\n");
	int curr_list_length = list_size(process_to_run);
	for (int i = 0; i < curr_list_length; i++)
	{
		fprintf(stderr, "%d\n", *(int *)list_remove_start(process_to_run));
	}
	fprintf(stderr, "......................\n");
	

	// list_fprint(NULL, stderr, process_to_run);


	fprintf(stderr, "Applying algorithm: %s\n", algorithm_name);
	fprintf (stderr, "The memsize is: %d\n", memsize);
	fprintf (stderr, "The quantum is: %d\n", quantum);
}
