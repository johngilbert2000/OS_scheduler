#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include "definitions.h"


// TODO:
// dmesg_info, timer

void to_dmesg(pid PID, long long start_time, long long stop_time) {
    printf("[Project 1] %d %llu %llu", PID, start_time, stop_time);
    // syscall(PRINTK, PID, start_time, stop_time);
    return; 
}

void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
} 

pid start_process(uint **remain_time){
    pid PID;
    PID = fork();
    if (PID == 0) {
        // child process
        long long start_time, stop_time;

        start_time = get_time();
        PID = getpid();
        uint exec_time = **remain_time;
        for (uint i = 0; i < exec_time; i++) {
            time_unit();
            if (**remain_time > 0) **remain_time -= 1;
        }
        stop_time = get_time();
        to_dmesg(PID, start_time, stop_time);

        exit(0);
    }
    // parent process returns to main() scheduling loop
    return PID;
}

void change_process(pid oldPID, pid newPID) {
    sched_yield(oldPID);
    sched_setscheduler(newPID); // use process with given PID
}

pid change_n_start(pid oldPID, uint **remain_time) {
    pid newPID;
    newPID = start_process(remain_time);
    sched_yield(oldPID);
    sched_setscheduler(newPID);
    return newPID;
}



