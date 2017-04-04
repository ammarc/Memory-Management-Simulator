#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include "list.h"
#include "simulation.h"

void add_to_disk (List* in_disk, Process* process, int curr_time);

void* remove_from_disk (List* in_data);

#endif