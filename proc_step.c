#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h> // for nonblocking file descriptor pipes
#include "headerfiles/definitions.h"
#include "headerfiles/IO.h"

#include <signal.h> // for kill()
#include <unistd.h> // for fork()

// TODO:
// - Test swap_priorities
// - use nice() values instead of sched_setscheduler in swap_priorities
// - change get_time() into a syscall
// - change make_dmesg() into a syscall

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

// struct job_data {
//   pid PID;
//   char *name;
//   uint ready_time;
//   uint execution_time;
//   enum job_status;
// };

// typedef struct job_data job;

// void make_dmesg(pid PID, long long start_time, long long stop_time) {
//     printf("[Project 1] %d %llu %llu", PID, start_time, stop_time);
//     // syscall(PRINTK, PID, start_time, stop_time);
//     return; 
// }

void disp_main(pid id, jobstat x){
  printf("[ P%d ]: %d  (main - process %d)\n", id, x, getpid());
}

void disp_parent(pid id, jobstat x){
  printf("[ P%d ]: %d  (parent - process %d)\n", id, x, getpid());
}

void disp_child(pid id, jobstat x){
  printf("[ P%d ]: %d  (child - process %d)\n", id, x, getpid());
}




long long get_time(){
    // gets the clock time in nanoseconds (from time.h)
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (t.tv_sec*(int)1e9 + t.tv_nsec);
}

void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
}

#ifdef NOT_LINUX
  // not linux
  int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param){
    return -1;
  }
#endif

pid start_process(uint id, jobstat *stat, uint exec_time, int pipefd[2]) {
    // Create new process with fork(); (used in process_control)
 
    // stat: the status of the selected job
    // exec_time: execution time of selected job
    // pipefd: file descriptor for piping selected job status from child process
    pid PID;
    jobstat localstatus;
    localstatus = *stat;
    long double start_time, stop_time;


    PID = fork();
    if (PID == 0) { // CHILD PROCESS
        // Pass start status into pipe
        PID = getpid();
        localstatus = STARTED;
        write(pipefd[1], &localstatus, sizeof(localstatus));
        if (DIO) {printf(" "); disp_child(id, localstatus);}

        start_time = get_time();

        // Set scheduler to other
        struct sched_param param;
        param.sched_priority = 0;
        sched_setscheduler(PID, SCHED_OTHER, &param);

        // Run process
        for (int i = 0; i < exec_time; i++) {
            time_unit();
            if ((DIO) && (i % 10 == 0)) {
              printf(" ");
              if (DIO) {disp_child(id, localstatus);}
            }
        }

        // Pass finished status into pipe and exit
        localstatus = FINISHED;
        write(pipefd[1], &localstatus, sizeof(localstatus));
        if (DIO) { printf("  "); disp_child(id, localstatus);}

        stop_time = get_time();
        if (DIO != 1) make_dmesg(PID, start_time, stop_time);
        exit(EXIT_SUCCESS);
    }
    else { // PARENT PROCESS
        // Get start status from child process
        read(pipefd[0], &localstatus, sizeof(localstatus));
        *stat = localstatus;
        if (DIO) disp_parent(id, localstatus);
    }
    return PID;
}

pid process_control(uint id, jobstat *stat, pid PID, pid prevPID, uint exec_time, int pipefd[2], bool running) {
    // Creates new process if stat indicates job has not started
    // Otherwise, switches to process with given PID 
    // from the previous process (prevPID)

    // stat: the status of the selected job
    // PID: PID of selected job, if it exists
    // prevPID: PID of previous job, if there was one
    // exec_time: execution time of selected job
    // pipefd: file descriptor for piping selected job status from child process
    // running: true if previous job is running, else false

    if (running == true) {
      if (DIO) printf("Stopping: %d\n", prevPID);
      kill(prevPID, SIGSTOP);
    }
    if (*stat == STARTED) {
      if (DIO) printf("Continue: %d\n", PID);
      kill(PID, SIGCONT);
    }
    else if (*stat == UNAVAILABLE) {
      PID = start_process(id, stat, exec_time, pipefd);
    }
    
   return PID;
}

void update_status(int id, pid PID, jobstat *stat, int *fd) {
    int waitstatus;
    waitstatus = 1; // initialized to silence warnings

    // Ensure pipe file descriptor is set to nonblocking
    fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
    fcntl(fd[1], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);

    if (*stat == STARTED) waitpid(PID, &waitstatus, WNOHANG);

    if ((waitstatus == 0) && (*stat != FINISHED)) {
        read(fd[0], stat, sizeof(*stat));
        // *stat = FINISHED;
    } 
    if (DIO) disp_main(id, *stat);
    // return *stat;
}







// ---------------------------------------------------
// old stuff
// ---------------------------------------------------

// void swap_priorities(pid oldPID, pid newPID) {
//     // TODO: use nice() values instead?
//     struct sched_param old_param, new_param;

//     old_param.sched_priority = LOW_PRIORITY;
//     new_param.sched_priority = HIGH_PRIORITY;
//     sched_setscheduler(oldPID, SCHED_FIFO, &old_param);
//     sched_setscheduler(newPID, SCHED_FIFO, &new_param);
// }

// void init_priority(pid PID) {
//     struct sched_param param;
//     param.sched_priority = HIGH_PRIORITY;
//     sched_setscheduler(PID, SCHED_FIFO, &param);
// }

// pid start_process(jobstat *stat, uint exec_time, jobstat pipefd[2]) {
//     // Create new process with fork(); (used in process_control)
 
//     // stat: the status of the selected job
//     // exec_time: execution time of selected job
//     // pipefd: file descriptor for piping selected job status from child process
//     pid PID;
//     jobstat localstatus;
//     localstatus = *stat;
//     long long start_time, stop_time;

//     PID = fork();
//     if (PID == 0) { // CHILD PROCESS
//         // Pass start status into pipe
//         PID = getpid();
//         localstatus = STARTED;
//         write(pipefd[1], &localstatus, sizeof(localstatus));
//         start_time = get_time();

//         // Run process
//         for (int i = 0; i < exec_time; i++) {
//             time_unit();
//         }

//         // Pass finished status into pipe and exit
//         localstatus = FINISHED;
//         write(pipefd[1], &localstatus, sizeof(localstatus));
//         start_time = get_time();
//         make_dmesg(PID, start_time, stop_time);
//         exit(EXIT_SUCCESS);
//     }
//     else { // PARENT PROCESS
//         // Get start status from child process
//         read(pipefd[0], &localstatus, sizeof(localstatus));
//         *stat = localstatus;
//     }
//     return PID;
// }

// pid process_control(jobstat *stat, pid PID, pid prevPID, uint exec_time, jobstat pipefd[2], bool running) {
//     // Creates new process if stat indicates job has not started
//     // Otherwise, switches to process with given PID 
//     // from the previous process (prevPID)

//     // stat: the status of the selected job
//     // PID: PID of selected job, if it exists
//     // prevPID: PID of previous job, if there was one
//     // exec_time: execution time of selected job
//     // pipefd: file descriptor for piping selected job status from child process
//     // running: true if previous job is running, else false

//     if (*stat == UNAVAILABLE) {
//         PID = start_process(stat, exec_time, pipefd);
//     }
//        if (running == true) {
//         swap_priorities(prevPID, PID);
//     }
//     else {
//         init_priority(PID);
//     }

//    return PID;
// }




// originally part of swap_priorities
     // old_priority = getpriority(PRIO_PROCESS, oldPID);
    // new_priority = getpriority(PRIO_PROCESS, newPID);
    // main_priority = getpriority(PRIO_PROCESS, 0);
    // new_priority = main_priority + 1;

// originally part of process_control
    // if (running == true) sched_yield(prevPID);
    // sched_setscheduler(PID, SCHED_FIFO, &param);

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



