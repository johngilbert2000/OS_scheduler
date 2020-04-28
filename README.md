# OS Scheduler
<hr>

A user space scheduler with the following policies, designed to run on Linux:

- FIFO (First In First Out)
- RR (Round Robin)
- SJF (Shortest Job First)
- PSJF (Preemptive Shortest Job First)

More details can be found in `report.pdf`

<hr>

### Compiling and Running

Compile and run with the following command (Python required):
```
make runall
```
This will compile and run for all input text files in the `input/` folder, and pipe the results into files in `output/`


(Without Python) this is equivalent to:
```
make

./schedProgram < FIFO_1.txt > FIFO_1_stdout.txt
dmesg | grep OS_scheduler > FIFO_1_dmesg.txt

./schedProgram < RR_1.txt > RR_1_stdout.txt
dmesg | grep OS_scheduler > RR_1_dmesg.txt

...
```
for all text files (`FIFO_1.txt`, `RR_1.txt`, ...) in the `input/` folder. (Note: `make run` is equivalent to `./schedProgram`)

When finished:
```
make clean
```

To remove all output files as well (Python required):
```
make cleanall
```


<hr>

### Input format

The program gets input parameters from the standard input in the following format
```
S // the scheduling policy, one of the following strings: FIFO, RR, SJF, PSJF.
N // the number of processes
N1 R1 T1
N2 R2 T2
…
Nn Rn Tn
//Ni - a character string with a length less than 32 bytes, specifying the name of the i-th process.
//Ri - a non-negative integer, specifying the ready time of the i-th process.
//Ti - a non-negative integer, specifying the execution time of the i-th process.
```

For example, input may look like:
```
FIFO
3
P1 1 10
P2 2 5
P3 2 7
```


### Output

Standard output of the program will have the following format:
```
NAME PID
//NAME - the name of this process specified by the input
//PID - the process id of this process
```

In the Linux kernel, start and stop times will be displayed as follows:
```
TAG PID ST FT
//TAG - the tag to identify the message of this project.
//PID - the process ID of this process
//ST - the start time of this process in the format seconds.nanoseconds.
//FT - the finish time of this process in the format seconds.nanoseconds.
```

An example output:
```
P1 4007
P2 4008
P3 4009
```

And output from the command dmesg might contain:
```
[Project1] 4007 1394530429.657899070 1394530430.004979285
[Project1] 4008 1394530429.668132665 1394530430.218223930
[Project1] 4009 1394530429.676547365 1394530430.221544405
```
