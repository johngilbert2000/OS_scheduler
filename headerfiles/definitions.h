#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DEBUG 0
#define IO 0 // Turn this to 1 to display prompts for the user (running without input files)
#define DIO 0 // another debug io
#define NOT_LINUX // define this if not using linux

#define LOW_PRIORITY 90
#define HIGH_PRIORITY 10

#define LOW_PRIORITY_NICE 10
#define HIGH_PRIORITY_NICE -10

#define USE_LINKED_LIST 1
#define USE_ARRAYS 0

#define MAXN 20
#define NAMESIZE 32

#include <sys/wait.h>

// typedef unsigned int uint;

typedef int pid;

enum policy_type {FIFO, RR, SJF, PSJF};

#ifdef DOUBLE_LINKED
struct linked {
    int val;
    struct linked *next;
    struct linked *prev;
};
#else
struct linked {
    int val:6;
    struct linked *next;
};
#endif

typedef struct linked node;

// struct sched_param {
//     int sched_priority;
// };

enum job_status {UNAVAILABLE, STARTED, FINISHED};
typedef enum job_status jobstat;

#endif