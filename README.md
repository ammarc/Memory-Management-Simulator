# Memory Management Simulator

This was done as part Project 1 for Computer Systems at the University
of Melbourne. This is essentially a simulator which takes processes of
different sizes; loads them into memory when required, using one of the
three different algorithms:
* First fit
* Best fit
* Worst fit
When needed, it swaps processes out to create a sufficiently large hole
for a new process to come into memory. It also takes care of scheduling
processes using a Round Robin algorithm.

The format of `input.txt` is rows of tuples, with each containing 
`<time-created>, <process-id>, <memory-size>, <job-time>`.

Command line arguments used:
```
./swap -f <filename> -a <algorithm> -m <memsize>-q <quantum>
```
where
* _filename_ refers to the file containing the input processes
* _algorithms_ is either set to best, first or worst
* _memsize_ refers to the size of the main memory
* _quantum_ refers to the quantum used in the Round Robin queue

Each time a process is swapped into memory information about the process
is printed in the following format:
```
time 42, 10 loaded, numprocesses=3, numholes=1, memusage=94%
```
where
* _time_ refers to the time when the event happens
* _10_ refers to the process ID of the process just loaded
* _numprocesses_ refers to the number of processes currently in memory
* _numholes_ refers to the number of holes currently in memory
* _memusage_ is a rounded up percentage of the memory currently occupied by processes
