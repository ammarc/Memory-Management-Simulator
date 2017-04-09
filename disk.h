/* Project 1 for COMP30023: Computer Systems
*  at the University of Melbourne
*  Semester 1, 2017
*  by: Ammar Ahmed
*  Username: ammara
*/

#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include "list.h"
#include "simulation.h"

/* Add a process to the disk, this also sets the time of arrival of the process */
void add_to_disk (List* in_disk, Process* process, int curr_time);

/* This removes next process to be transferred to memory and returns it */
void* remove_from_disk (List* in_data);

#endif