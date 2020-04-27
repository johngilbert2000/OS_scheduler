#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h> // for nonblocking file descriptor pipes
#include "definitions.h"

#define LOW_PRIORITY 10
#define HIGH_PRIORITY 90

// TODO:
// - sched_setscheduler / sched_yield
// dmesg_info, timer

enum job_status {UNAVAILABLE, STARTED, FINISHED};
typedef enum job_status jobstat;

// struct job_data {
//   pid PID;
//   char *name;
//   uint ready_time;
//   uint execution_time;
//   enum job_status;
// };

// typedef struct job_data job;

void to_dmesg(pid PID, long long start_time, long long stop_time) {
    printf("[Project 1] %d %llu %llu", PID, start_time, stop_time);
    // syscall(PRINTK, PID, start_time, stop_time);
    return; 
}

void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
}
// --------------
// To do in main:

// int pipe_ret;
// jobstat fd[2];
// // Ensure pipe file descriptor is set to nonblocking

// fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
// fcntl(fd[1], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);

// // Create pipe
// pipe_ret = pipe(fd);
// if (pipe_ret == -1) { perror("pipe error"); exit(1); }
// // --------------

struct sched_param {
    int sched_priority;
};

void swap_priorities(pid oldPID, pid newPID) {
   struct sched_param old_param, new_param;
    // int old_priority, new_priority, main_priority;
    int high_priority, low_priority;

    // old_priority = getpriority(PRIO_PROCESS, oldPID);
    // new_priority = getpriority(PRIO_PROCESS, newPID);
    // main_priority = getpriority(PRIO_PROCESS, 0);
    // new_priority = main_priority + 1;

    old_param.sched_priority = LOW_PRIORITY;
    new_param.sched_priority = HIGH_PRIORITY;

    sched_setscheduler(oldPID, SCHED_FIFO, &old_param);
    sched_setscheduler(newPID, SCHED_FIFO, &new_param);
}

void init_priority(pid PID) {
    struct sched_param param;
    param.sched_priority = LOW_PRIORITY;
    sched_setscheduler(PID, SCHED_FIFO, &param);
}

pid change_process(jobstat *stat, pid PID, pid prevPID, uint exec_time, jobstat pipefd[2], bool running) {
    // stat: the status of the selected job
    // PID: PID of selected job, if it exists
    // prevPID: PID of previous job, if there was one
    // exec_time: execution time of selected job
    // pipefd: file descriptor for piping selected job status from child process
    // running: true if previous job is running, else false
    jobstat localstatus;
    localstatus = *stat;
    

    if (localstatus == UNAVAILABLE) {
        // Create new process
        PID = fork();
        if (PID == 0) { // CHILD PROCESS
            // Pass start status into pipe
            PID = getpid();
            localstatus = STARTED;
            write(pipefd[1], &localstatus, sizeof(localstatus));

            // Run process
            for (int i = 0; i < exec_time; i++) {
                time_unit();
            }

            // Pass finished status into pipe and exit
            localstatus = FINISHED;
            write(pipefd[1], &localstatus, sizeof(localstatus));
            exit(EXIT_SUCCESS);
        }
        else { // PARENT PROCESS
            // Get start status from child process
            read(pipefd[0], &localstatus, sizeof(localstatus));
            *stat = localstatus;
        }
    }
    
    // if (running == true) sched_yield(prevPID);
    // sched_setscheduler(PID, SCHED_FIFO, &param);
    if (running = true) {
        swap_priorities(prevPID, PID);
    }
    else {
        init_priority(PID);
    }

   return PID;
}

// pid change_job(jobstat *stat) {
//     if (*stat == UNAVAILABLE) {
//         start_process();
//     }
//     else if (*stat == STARTED) {
//         change_priority();
//     }
//     else if (*stat == FINISHED) {

//     }
// }


// pid start_process(uint **remain_time){
//     pid PID;
//     PID = fork();
//     if (PID == 0) {
//         // child process
//         long long start_time, stop_time;

//         start_time = get_time();
//         PID = getpid();
//         uint exec_time = **remain_time;
//         for (uint i = 0; i < exec_time; i++) {
//             time_unit();
//             if (**remain_time > 0) **remain_time -= 1;
//         }
//         stop_time = get_time();
//         to_dmesg(PID, start_time, stop_time);

//         exit(0);
//     }
//     // parent process returns to main() scheduling loop
//     return PID;
// }

// void change_process(pid oldPID, pid newPID) {
//     sched_yield(oldPID);
//     sched_setscheduler(newPID); // use process with given PID
// }

// pid change_n_start(pid oldPID, uint **remain_time) {
//     pid newPID;
//     newPID = start_process(remain_time);
//     sched_yield(oldPID);
//     sched_setscheduler(newPID);
//     return newPID;
// }



