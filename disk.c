#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "disk.h"
#include "simulation.h"

void add_to_disk (List* in_disk, Process* process, int curr_time)
{
    list_add_end(in_disk, process);
    // now we need to modify the data of the node we added to set
    // the time the process was in the disk
    process->disk_entry_time = curr_time;
}


void* remove_from_disk (List* in_disk)
{
    assert(in_disk != NULL);
	assert(in_disk->size > 0);

    Node* curr_process = in_disk->head;
    Node* max_process = in_disk->head;
    Node* node = in_disk->head;
    // note that a higher priority is denoted by a lower process ID
    int highest_priority = ((Process *)curr_process)->process_id;
    int current_time = ((Process *)curr_process)->time_created;

    // we need to run a loop that goes through the list
    // if the arrival times for all the processes is the same
    while (in_disk->size > 1 && current_time == ((Process *)curr_process)->time_created)
    {
        if (highest_priority > ((Process *)(curr_process->next))->process_id)
        {
            highest_priority = ((Process *)curr_process->next)->process_id;
            max_process = curr_process->next;
        }
        curr_process = curr_process->next;
    }
    // fprintf(stderr, "Good\n");
    // also need to remove the process from the in_disk list
    // using the highest priority
    while (node)
    {
        if (((Process *) node)->process_id == highest_priority)
        {
            if(node->next)
            {
                node->next->back = node->back;
            }
            else
            {
                in_disk->last = node->back;
            }

            if(node->back)
            {
                node->back->next = node->next;
            }
            else
            {
                in_disk->head = node->next;
            }

            node->back = NULL;
            node->next = NULL;
            break;
        }

            
        node = node->next;
        
    }

    in_disk->size--;

    return max_process;
}