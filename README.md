# OS Scheduler
<hr>

A user space scheduler with the following policies, designed to run on Linux:

- FIFO (First In First Out)
- RR (Round Robin)
- SJF (Shortest Job First)
- PSJF (Preemptive Shortest Job First)

**More details can be found in `report.pdf`**

<hr>

### Demo

There is a bug in my code, and I have worked long and hard to find it, but with little success. The result is that my code can run for small numbers, but it segfaults for larger ones. 70+ commits, hours and hours of work, I tried my best.

Here is a video demonstration:

[![Demo video](https://github.com/johngilbert2000/OS_scheduler/blob/master/demo/demo_vid_img.png)](https://www.youtube.com/watch?v=ljkfqwqAUFs)

<hr>

### Compiling and Running

#### Compiling the Custom Kernel

This code requires Linux Kernel 5.4.35. Download the kernel source on a Linux machine with:

```bash
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.4.35.tar.xz
tar xf linux-5.4.35.tar.xz
cd linux-5.4.35/
```

Once you have the kernel source, you likely want to build with your existing kernel config:

```bash
make mrproper
cp /boot/config-$(uname -r) .config
yes '' | make oldconfig
```

Now copy in the files required for the custom system call (my project files are in `~/OS_scheduler` but this may vary):

```bash\
cp ~/OS_scheduler/kernel_files/syscall_32.tbl arch/x86/entry/syscalls/syscall_32.tbl
cp ~/OS_scheduler/kernel_files/syscall_64.tbl arch/x86/entry/syscalls/syscall_64.tbl
cp ~/OS_scheduler/kernel_files/unistd.h include/uapi/asm-generic/unistd.h
cp ~/OS_scheduler/kernel_files/syscalls.h include/linux/syscalls.h
cp ~/OS_scheduler/kernel_files/sys_ni.c kernel/sys_ni.c
cp -r ~/OS_scheduler/kernel_files/dummy_proc dummy_proc
cp ~/OS_scheduler/kernel_files/Makefile Makefile
```


#### Compiling the App

Compile and run with the following command (*Python required*):
```
make runall
```
This will compile and run for all input text files in the `input/` folder, and pipe the results into files in `output/`


(*Without Python*) this is equivalent to:
```
make

./schedProgram < FIFO_1.txt > FIFO_1_stdout.txt
dmesg | grep "Project 1" > FIFO_1_dmesg.txt

./schedProgram < RR_1.txt > RR_1_stdout.txt
dmesg | grep "Project 1" > RR_1_dmesg.txt

...
```
for all text files (`FIFO_1.txt`, `RR_1.txt`, ...) in the `input/` folder. (Note: `make run` is also equivalent to `./schedProgram`)

When finished:
```
make clean
```

To remove all output files as well (*Python required*):
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
