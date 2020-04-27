#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"

/* localtime example */
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

#include <sys/types.h> // for pid_t
#include <unistd.h> // for fork()
#include <inttypes.h> // for printing pointers
#include <sys/wait.h> // for waitpid?
#include <fcntl.h>
// #include <pthread.h> // for pipes?

#define MAXN 20
#define STARTED 1
#define FINISHED 2


void time_unit() {
  for (unsigned int j = 0; j < 1000; j++) {
    for (unsigned int i = 0; i < 100000; i++) {

    }
  }
}

int main(int argc, char *argv[]){
  pid_t PID; // process id
  int pipe_rw[2]; // pipe[0]: pipe read, pipe[1]: pipe write
  int X[MAXN], id, status, pipe_return_value;  
  fcntl(pipe_rw[0], F_SETFL, fcntl(pipe_rw[0], F_GETFL) | O_NONBLOCK);
  fcntl(pipe_rw[1], F_SETFL, fcntl(pipe_rw[0], F_GETFL) | O_NONBLOCK);
  // if (fcntl(pipe_rw[0], F_SETFL, O_NONBLOCK) < 0) { perror("nonblocking pipe error"); exit(1)};
  // if (fcntl(pipe_rw[1], F_SETFL, O_NONBLOCK) < 0) { perror("nonblocking pipe error"); exit(1)};

  // goal: change X[id] in child process;
  printf("Input id: "); scanf("%d", &id);

  // initialize X[]
  for (int i = 0; i < MAXN; i++) { X[i] = 0; }
  printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

  pipe_return_value = pipe(pipe_rw);
  if (pipe_return_value == -1) { perror("pipe error"); exit(1); }

  PID = fork();
  if (PID == 0) { // CHILD
    X[id] = 1; // some value to indicate process started
    printf("X[%d]: %d  (child - process %d)\n", id, X[id], getpid());
    write(pipe_rw[1], (X+id), sizeof(int));

    for (int i = 0; i<5; i++) time_unit();
    X[id] = 2; // some value to indicate process finished
    write(pipe_rw[1], (X+id), sizeof(int));
    printf("X[%d]: %d  (child - process %d)\n", id, X[id], getpid());
    printf("[child - ending process %d]\n", getpid());
    exit(EXIT_SUCCESS);
  }
  else { // PARENT
    read(pipe_rw[0], (X+id), sizeof(int));
    // printf("X[%d]: %d  (parent - process %d)\n", id, X[id], getpid());
  }
  time_unit();
  time_unit();
  time_unit();
  if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

  time_unit();
  if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

  time_unit();
  if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

  time_unit();
  if (X[id] != FINISHED) read(pipe_rw[0], (X+id), sizeof(int));
  printf("X[%d]: %d  (main - process %d)\n", id, X[id], getpid());

  waitpid(PID, &status, 0); // don't end main until child ends
  printf("[main - process %d has ended]\n", PID);
  printf("Final X[%d]: %d  (main - process %d)\n", id, X[id], getpid());
  return 0;
}

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
