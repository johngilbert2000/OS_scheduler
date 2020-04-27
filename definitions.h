#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DEBUG 1
#define IO 1

typedef unsigned int uint;

typedef int pid;

enum policy_type {FIFO, RR, SJF, PSJF};

struct linked {
    int val;
    struct linked *next;
    struct linked *prev;
};

typedef struct linked node;

struct sched_param {
    int sched_priority;
};

enum job_status {UNAVAILABLE, STARTED, FINISHED};
typedef enum job_status jobstat;

#endif