/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "disk.h"
#include "simulation.h"

void add_to_disk (List* in_disk, Process* process, int curr_time)
{
    /* First checking if the process was already on the disk or not
     * and if so the disk entry time to be compared should not be
     * the time of its creation but the time it arrived on disk */
    if (!process->added_to_disk)
        process->disk_entry_time = process->time_created;
    else
        process->disk_entry_time = curr_time;

    list_add_end(in_disk, process);
    process->added_to_disk = true;
}

/* Removes and returns the next process to be transferred to memory */
void* remove_from_disk (List* in_disk)
{
    assert(in_disk != NULL);
	assert(in_disk->size > 0);

    Node* curr_process_node = in_disk->head;
    Node* max_process_node = in_disk->head;
    
    Node* node = in_disk->head;

    /* The highest priority is set at as the process ID of the current process
     * and is changed over time to get the lowest integer */
    int highest_priority = ((Process *)curr_process_node->data)->process_id;
    int current_time = ((Process *)curr_process_node->data)->disk_entry_time;

    /* This loop goes through the disk list and compares their priority if
     * their disk entry time is the same and sets the max_process_node 
     * accordingly */
    while (curr_process_node->next && current_time == ((Process *)
                                    curr_process_node->next->data)->
                                                        disk_entry_time)
    {
        if (highest_priority > ((Process *)curr_process_node->next->data)->
                                                                process_id)
        {
            highest_priority = ((Process *)curr_process_node->next->data)->
                                                                process_id;
            max_process_node = curr_process_node->next;
        }
        curr_process_node = curr_process_node->next;
    }
    
    /* This loop traverses the disk and removes the node with the highest 
     * priority but the node is not freed as it is not yet processed */
    while (node)
    {
        if (((Process *) node->data)->process_id == highest_priority)
        {
            if(node->next)
                node->next->back = node->back;
            else
                in_disk->last = node->back;

            if(node->back)
                node->back->next = node->next;
            else
                in_disk->head = node->next;

            node->back = NULL;
            node->next = NULL;
            break;
        }
        node = node->next;
    }

    in_disk->size--;

    return max_process_node->data;
}