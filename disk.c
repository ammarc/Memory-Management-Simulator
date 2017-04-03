#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct processes_on_disk
{
    int time_on_disk;
    int process_id;
    int memory_size;
    int job_time;
} ProcessesOnDisk;
