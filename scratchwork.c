#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headerfiles/linkedlist.h"

/* localtime example */
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

#include <sys/types.h> // for pid_t
#include <unistd.h> // for fork()
#include <inttypes.h> // for printing pointers
#include <sys/wait.h> // for waitpid?
#include <fcntl.h>
#include <sys/kernel.h>
#include <sys/resource.h>
// #include <pthread.h> // for pipes?


#include <stdarg.h>
#include <sched.h>
#include "headerfiles/definitions.h"
#include "headerfiles/IO.h"

#include <signal.h> // for kill()

// #define DIO 1 // debug io
// #define NOT_LINUX 

// note:
// - kill() with SIGSTOP --> pause process
// - kill() with SIGCONT --> resume process


#define MAXN 20
// #define STARTED 1
// #define FINISHED 2
// #define LOW_PRIORITY 90
// #define HIGH_PRIORITY 10
// #define HIGH_PRIORITY -10
// #define LOW_PRIORITY 10


// void time_unit() {
//   for (unsigned int j = 0; j < 1000; j++) {
//     for (unsigned int i = 0; i < 100000; i++) {

//     }
//   }
// }

// enum job_status {UNAVAILABLE, STARTED, FINISHED};
// typedef job_status jobstat;

// struct job_data {
//   pid PID;
//   uint id;
//   char *name;
//   uint ready_time;
//   uint execution_time;
//   enum job_status;
// };

// typedef struct job_data job;

void disp_main(pid id, jobstat x){
  printf("[ P%d ]: %d  (main - process %d)\n", id, x, getpid());
}

void disp_parent(pid id, jobstat x){
  printf("[ P%d ]: %d  (parent - process %d)\n", id, x, getpid());
}

void disp_child(pid id, jobstat x){
  printf("[ P%d ]: %d  (child - process %d)\n", id, x, getpid());
}

int id_input() {
  int id;
  printf("Input id: "); scanf("%d", &id);
  return id;
}

void disp_end(pid PID) {
  printf("[main: child process %d has ended]\n", PID);
}

// void make_dmesg(pid PID, long double start_time, long double stop_time) {
//     printf("[Project 1] %d %Lf %Lf\n", PID, start_time, stop_time);
//     // %llu long long
//     // syscall(PRINTK, PID, start_time, stop_time);
//     return; 
// }

long double get_time(){
    // gets the clock time in nanoseconds (from time.h)
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    // return (t.tv_sec*(int)1e9 + t.tv_nsec);
    return (t.tv_sec + (( (long double)t.tv_nsec )/1e9));
}

void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
}

void swap_priorities(pid oldPID, pid newPID) {
    // TODO: use nice() values instead?
    struct sched_param old_param, new_param;

    old_param.sched_priority = LOW_PRIORITY;
    new_param.sched_priority = HIGH_PRIORITY;
    // sched_setscheduler(oldPID, SCHED_FIFO, &old_param);
    // sched_setscheduler(newPID, SCHED_FIFO, &new_param);
    // setpriority(PRIO_PROCESS, newPID, HIGH_PRIORITY);
    // setpriority(PRIO_PROCESS, oldPID, LOW_PRIORITY);
    kill(oldPID, SIGSTOP);
    kill(newPID, SIGCONT);


}

void init_priority(pid PID) {
    struct sched_param param;
    param.sched_priority = HIGH_PRIORITY;
    // sched_setscheduler(PID, SCHED_FIFO, &param);
    // setpriority(PRIO_PROCESS, PID, HIGH_PRIORITY);
    kill(PID, SIGCONT);
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
      if (true) printf("Stopping: %d\n", prevPID);
      kill(prevPID, SIGSTOP);
    }
    if (*stat == STARTED) {
      if (true) printf("Continue: %d\n", PID);
      kill(PID, SIGCONT);
    }
    else if (*stat == UNAVAILABLE) {
      PID = start_process(id, stat, exec_time, pipefd);
    }
    

    // if ((running == true) && (*stat == STARTED)) {
    //   // swap_priorities(prevPID, PID);
    //   kill(prevPID, SIGSTOP);
    //   kill(PID, SIGCONT);
    // }
    // else {
    //     init_priority(PID);
    // }

   return PID;
}

pid update_status(int id, int waitstatus, pid PID, jobstat *stat, int *fd) {

  // Ensure pipe file descriptor is set to nonblocking
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
  fcntl(fd[1], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);

  if (waitstatus > 0) waitpid(PID, &waitstatus, WNOHANG);

  if ((waitstatus == 0) && (*stat != FINISHED)) {
    read(fd[0], stat, sizeof(*stat));
  } 
  if (DIO) disp_main(id, *stat);
  return PID;
}


int main() {
  // ----------------
  // Initialize stuff

  jobstat j, j2;
  int id2 = 1;
  int id = 0;
  j = UNAVAILABLE;
  j2 = UNAVAILABLE;
  int exec_time[MAXN], PIDs[MAXN];
  exec_time[id] = 70;
  exec_time[id2] = 70;
  int waitstatus;
  int waitstat2;

  int pipe_ret;
  int fd[2][MAXN];
  bool running; // whether or not a previous child process is running

  // PROCESS 1 PIPE
  // Ensure pipe file descriptor is set to nonblocking
  fcntl(fd[0][id2], F_SETFL, fcntl(fd[0][id2], F_GETFL) | O_NONBLOCK);
  fcntl(fd[1][id2], F_SETFL, fcntl(fd[0][id2], F_GETFL) | O_NONBLOCK);

  // PROCESS 0 PIPE
  // Ensure pipe file descriptor is set to nonblocking
  fcntl(fd[0][id], F_SETFL, fcntl(fd[0][id], F_GETFL) | O_NONBLOCK);
  fcntl(fd[1][id], F_SETFL, fcntl(fd[0][id], F_GETFL) | O_NONBLOCK);

  // ---------------

  // Create pipe 0
  pipe_ret = pipe(fd[id]);
  if (pipe_ret == -1) { perror("pipe error"); exit(1); }

  // Create pipe 1
  pipe_ret = pipe(fd[id2]);
  if (pipe_ret == -1) { perror("pipe error"); exit(1); }

  if (DIO) disp_main(id, j); // initially 0
  if (DIO) disp_main(id2, j2); // initially 0

  // PIDs[id] = start_process(id, &j, exec_time[id], fd[id]);
  // PIDs[id2] = start_process(id2, &j2, exec_time[id2], fd[id2]);

  running = false; // no child process running

  // Start process 0
  PIDs[id] = process_control(id, &j, PIDs[id], PIDs[id2], exec_time[id], fd[id], running);
  running = true; // process 0 is running

  // Main - check if process 0 is running
  update_status(id, waitstatus, PIDs[id], &j, fd[id]);

  // Start process 1 (interrupt process 0)
  PIDs[id2] = process_control(id2, &j2, PIDs[id2], PIDs[id], exec_time[id2], fd[id2], running);

  // Main - check if process 1 is running
  update_status(id2, waitstat2, PIDs[id2], &j2, fd[id2]);
  // disp_main(id, j);
  // update_status(id, waitstatus, PIDs[id], &j, fd[id]);

  // Wait for process 1 to finish
  waitpid(PIDs[id2], &waitstat2, 0); 

  // Main - Check that process 1 finished
  update_status(id2, waitstat2, PIDs[id2], &j2, fd[id2]);

  // Continue process 0
  PIDs[id] = process_control(id, &j, PIDs[id], PIDs[id2], exec_time[id], fd[id], running);

  // Main - check process 0 is running
  update_status(id, waitstatus, PIDs[id], &j, fd[id]);

  // Wait for process 0 to finish
  waitpid(PIDs[id], &waitstatus, 0);

  // Main - check that process 0 finished
  update_status(id, waitstatus, PIDs[id], &j, fd[id]);
  // if (DIO) disp_main(id, j);

  // waitpid(PIDs[id2], &waitstat2, 0);
  // update_status(id2, waitstat2, PIDs[id2], &j2, fd[id2]);
 
  return 0;

}









// --------

// pid runproc(int id, int **X, int *fd) {
//   pid_t PID;
//   PID = fork();
//   int x;
//   x = **(X+id);

//   if (PID == 0) { // CHILD

//     x = STARTED; // some value to indicate process started

//     disp_child(id, x);
//     write(fd[1], &x, sizeof(int));

//     for (int i = 0; i<5; i++) time_unit();
//     X[id] = FINISHED; // some value to indicate process finished

//     write(fd[1], &x, sizeof(int));
//     disp_child(id, x);

//     printf("[child: ending process %d]\n", getpid());
//     exit(EXIT_SUCCESS);
//   }
//   else { // PARENT

//     read(fd[0], (X+id), sizeof(int));
//     disp_parent(PID, x);

//     &X[id] = x;
//     return PID;
//   }
// }


// int main(int argc, char *argv[]){
//   pid_t PID[MAXN]; // process id
//   int fd[2]; // fd[0]: pipe read file descriptor, fd[1]: pipe write
//   int X[MAXN], id, status, pipe_return_value;  
//   pid_t pid_val;
//   fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
//   fcntl(fd[1], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);

//   // goal: change X[id] in child process;
//   id = id_input();

//   // initialize X[]
//   for (int i = 0; i < MAXN; i++) { X[i] = 0; }
//   disp_main(id, X[id]);

//   pipe_return_value = pipe(fd);
//   if (pipe_return_value == -1) { perror("pipe error"); exit(1); }

//   runproc(id, &X, fd);

//   waitpid(PID[id], &status, WNOHANG);
//   for (int i = 0; i < 7; i++) {
//     time_unit();
    // if (status > 0) waitpid(PID[id], &status, WNOHANG);

    // if ((status == 0) && (X[id] != FINISHED)) {
    //   read(fd[0], (X+id), sizeof(int));
    // } 
    // disp_main(id, X[id]);
//   }

//   waitpid(PID[id], &status, 0); // don't end main until child ends
//   disp_end(PID[id]);
//   printf("Final X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
//   return 0;
// }





// int main(int argc, char *argv[]){
//   pid_t PID; // process id
//   int fd[2]; // fd[0]: pipe read file descriptor, fd[1]: pipe write
//   int X[MAXN], id, status, pipe_return_value;  
//   fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
//   fcntl(fd[1], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
//   // if (fcntl(pipe_rw[0], F_SETFL, O_NONBLOCK) < 0) { perror("nonblocking pipe error"); exit(1)};
//   // if (fcntl(pipe_rw[1], F_SETFL, O_NONBLOCK) < 0) { perror("nonblocking pipe error"); exit(1)};

//   // goal: change X[id] in child process;
//   id = id_input();

//   // initialize X[]
//   for (int i = 0; i < MAXN; i++) { X[i] = 0; }
//   printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());


//   pipe_return_value = pipe(fd);
//   if (pipe_return_value == -1) { perror("pipe error"); exit(1); }

//   PID = fork();

//   if (PID == 0) { // CHILD

//     X[id] = STARTED; // some value to indicate process started
//     printf("X[%d]: %d  (child - process %d)\n", id, X[id], getpid());
//     write(fd[1], (X+id), sizeof(int));

//     for (int i = 0; i<5; i++) time_unit();
//     X[id] = FINISHED; // some value to indicate process finished
//     write(fd[1], (X+id), sizeof(int));
//     printf("X[%d]: %d  (child - process %d)\n", id, X[id], getpid());
//     printf("[child: ending process %d]\n", getpid());
//     exit(EXIT_SUCCESS);
//   }
//   else { // PARENT

//     read(fd[0], (X+id), sizeof(int));
//     printf("X[%d]: %d  (parent - process %d)\n", id, X[id], getpid());
//   }

//   waitpid(PID, &status, WNOHANG);
//   printf("status: %d (child running)\n", status);
//   for (int i = 0; i < 7; i++) {
//     time_unit();
//     if (status > 0) waitpid(PID, &status, WNOHANG);

//     if ((status == 0) && (X[id] != FINISHED)) {
//       read(fd[0], (X+id), sizeof(int));
//       printf("[main: child process %d has ended]\n", PID);
//       printf("status: %d (child finished)\n", status);
//     } 
//     printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
//   }

//   waitpid(PID, &status, 0); // don't end main until child ends
//   printf("Final X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
//   return 0;
// }
  
    // if (status == 0) X[id] = FINISHED;

  // waitpid(PID, &status, WNOHANG);
  // printf("status: %d\n", status);
  // time_unit();
  // time_unit();
  // time_unit();
  // if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  // printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
  // if (status > 0) waitpid(PID, &status, WNOHANG);
  // if (status == 0) X[id] = 2;

  // time_unit();
  // // if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  // printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
  // if (status > 0) waitpid(PID, &status, WNOHANG);
  // if (status == 0) X[id] = 2;

  // time_unit();
  // if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  // printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
  // if (status > 0) waitpid(PID, &status, WNOHANG);
  // if (status == 0) X[id] = 2;

//   time_unit();
//   // if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
//   if (status > 0) waitpid(PID, &status, WNOHANG);
//   if (status == 0) X[id] = 2;

//   printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

//   waitpid(PID, &status, 0); // don't end main until child ends
//   printf("[main: process %d has ended]\n", PID);
//   printf("Final X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
//   printf("status: %d\n", status);
//   return 0;
// }

// int not_main(int argc, char*argv[]){
//   // THIS WORKS!
//   pid_t PID; // process id
//   int fd[2]; //pipe
//   int pipe_ret; // return value
//   int x;
//   int *xptr;

//   x = 0;
//   xptr = &x;
//   pipe_ret = pipe(fd);

//   if (pipe_ret == -1) {
//     perror("Pipe error");
//     exit(1);
//   }

//   PID = fork();

//   if (PID == 0) {
//     printf("child\n");
//     *xptr += 1;
//     write(fd[1], &x, sizeof(x));
//     exit(0);
//   }
//   else {
//     for (int i = 0; i < 1000000; i++) {

//     }
//     printf("parent\n");
//     read(fd[0], &x, sizeof(x));
//   }
//   printf("%d", x);
// }

// int main(int argc, char *argv[]) {
//   pid_t PID;
//   int *x;
//   int fd[2];
//   int ret;
//   char buff[20];
//   int intbuff;
  
//   ret = pipe(fd);
//   *x = 0;
//   ret = 0;
//   PID = fork();
  
//   if (PID == 0) {
//     *x += 1;
//     // sprintf(buff, "%d", *x);
//     // printf("%s", buff);
//     ret += 1;
//     write(fd[1], &ret, sizeof(ret)); //sizeof(buff));
    
//     printf("hello from child: %d\n", ret);
//     exit(0);
//   }
//   else {
//     for (int i = 0; i < 1000000; i++){

//     }
//     read(fd[0], &ret, sizeof(ret));

//     printf("hello from parent: %d \n", ret);
//     // printf("0x%" PRIXPTR "\n", (uintptr_t)x);
//   }
//   // printf("x: %d\n", *x);
//   printf("ret: %d\n", ret);
//   return 0;

// }

// /**
//  * Returns the current time in microseconds.
//  */
// long long get_microseconds(){
// 	struct timeval time;
// 	gettimeofday(&time, NULL);
// 	return (time.tv_sec * (int)1e6 + time.tv_usec);
// }

// long long get_ns_secs(){
//     struct timespec t;
//     clock_gettime(CLOCK_REALTIME, &t);
//     return (t.tv_sec*(int)1e9 + t.tv_nsec);
// }

// int main ()
// {
//   time_t rawtime;
//   struct tm * timeinfo;

//   time ( &rawtime );
//   timeinfo = localtime ( &rawtime );
//   printf ( "Current local time and date: %s", asctime (timeinfo) );
  
//   printf("\n");
//   struct timeval time;
//   gettimeofday(&time, NULL);
//   long t = (long) time.tv_usec;
//   printf("%ld \n",t);
//   printf("sys time: %lld", get_ns_secs());

//   return 0;
// }


// int main() {
//     printf("TESTSTUFF.C\n");
//     node *head;
//     node *tmp;
//     node *tail;
//     node *tmp2;

//     tmp = create_node(1);
//     head = tmp;
//     tail = tmp;

//     append_value(&tail, 2);
//     append_value(&tail, 3);
//     append_value(&tail, 4);
//     append_value(&tail, 5);
//     append_value(&tail, 7);
//     append_value(&tail, 8);
//     append_value(&tail, 9);
//     append_value(&tail, 10);


//     // tmp = create_node(4);
//     // head = tmp;
//     // tail = head;

//     // tmp = create_node(5);
//     // insert_node(tail, tmp);
//     // tail = tmp;

//     // tmp = create_node(7);
//     // insert_node(tail, tmp);
//     // tail = tmp;

//     // append_value(&tail, 9);
//     // // insert_value(tmp, 8);
//     // append_value(&tmp, 8);

//     print_list(head);

//     tmp = lookup(head, 2);
//     remove_node(tmp);

//     tmp = lookup(head, 5);
//     printf("lookup: %d\n", tmp->val);
//     printf("previous: %d \n", tmp->prev->val);
//     printf("next: %d \n", tmp->next->val);

//     append_value(&tmp, 6);

//     // tmp2 = create_node(6);
//     // insert_node(tmp, tmp2);
   
//     print_list(head);
//     print_reverse(tail);

//     remove_head(&head);
//     print_list(head);

//     remove_head(&head);
//     print_list(head);


//     tmp = lookup(head, 9);
//     tmp2 = get_head(tmp);
//     print_node(tmp2);
//     tmp2 = get_tail(tmp);
//     print_node(tmp2);

//     remove_tail(&tail);
//     print_list(head);
//     print_node(tail);

//     return 0;
// }
