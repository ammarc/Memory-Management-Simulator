#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "disk.h"

void add_to_disk (List* in_disk, void* process, int curr_time)
{
    list_add_end(in_disk, process);

    // now we need to modify the data of the node we added to set
    // the time the process was in the disk
    ((Process *) in_disk->last->data)->disk_entry_time = curr_time;
}


Process* remove_from_disk (List* in_disk)
{
    return list_remove_start (in_disk);
}