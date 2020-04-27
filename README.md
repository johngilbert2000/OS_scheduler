# OS Scheduler
<hr>

A user space scheduler with the following policies:

- FIFO (First In First Out)
- RR (Round Robin)
- SJF (Shortest Job First)
- PSJF (Preemptive Shortest Job First)

<br>

Compile and run with the following commands:
```
make
make run
```

And, when finished:
```
make clean
```

Note that you can also pipe info to and from files as shown:
```
make run < FIFO_1.txt > FIFO_1_stdout.txt
sudo dmesg | grep OS_scheduler > FIFO_1_dmesg.txt
```
<hr>
