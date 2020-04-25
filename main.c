#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "useful_funcs.h"
#include "linkedlist.h"
#include "scheduler.c"

#define DEBUG 1
#define IO 1

// Note: RR round is 500 time units

// QUESTION: does RR alternate when current_time % 500 == 0, 
// or when process_elapsed_time % 500 == 0
// (current time is the number of time steps elapsed since main starts)

enum policy_type {FIFO, RR, SJF, PSJF};

typedef unsigned int uint;

// QUESTION: do I use fork to call time_unit(), with time_unit in a separate file?
void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
} 

// A note on implementation:
// a linked list is used to hold ids of available jobs
// the head of the linked list will always be the current job
// (some policies will move new jobs to the head of the list when selecting them)

double get_time() {

    return 2.0;
}

int main() {

    char S[4];
    enum policy_type policy; 

    // -------------
    // Get Policy
    // -------------
    if (IO) printf("Policy: ");
    scanf("%s", S);

    if (strncmp(S,"FIFO", 4) == 0){
        policy = FIFO;
    }
    else if (strncmp(S, "RR", 4) ==0){
        policy = RR;
    }
    else if (strncmp(S, "SJF", 4) == 0){
        policy = SJF;
    }
    else if (strncmp(S, "PSJF",4) == 0){
        policy = PSJF;
    }
    else{
        printf("ERROR: Incorrect Policy - %s", S);
        return 1;
    }

    // N
    uint N;
    if (IO) printf("Number processes: ");
    scanf("%u", &N);

    // input stuff
    char names[N][32];
    uint ready_times[N];
    uint execution_times[N];
    uint remaining_times[N];
    
    // output stuff, may be unnecessary
    double start_times[N];
    double end_times[N];
    uint PIDs[N];
    
    // N R T
    for (int i = 0; i < N; i++) {
        if (IO) printf("N R T: ");
        scanf("%s", names[i]); 
        scanf("%u", &ready_times[i]);
        scanf("%u", &execution_times[i]);

        remaining_times[i] = execution_times[i];
    }


    // ------------------
    // Parameters
    // ------------------
 
    // sort ready_times[N]
    uint sorted_ids[N];
    id_sort(ready_times, sorted_ids, N);

    // total remaining execution time
    int total_remaining = reduce(add, remaining_times, N);

    uint current_time = 0;

    uint id = 0; // current selected id
    int qsize = 0; // size of ready_queue (linked list)

    // get next job to arrive
    uint arrival_itr = 0;
    uint next_arrival; 
    next_arrival = sorted_ids[arrival_itr];

    node *head;
    node *tail;
    node *tmp;
    tmp = create_node(next_arrival);
    head = tmp;
    tail = tmp;
    bool running;
    running = false; // indicates previous job is still running; (used for SJF)
    double actual_time;
    uint temporary_id;

    
    while (total_remaining > 0) {
        // ------------------
        // Update Ready Queue
        // ------------------

        // Check if next job arrived, and if so, update the ready queue
        while ((arrival_itr < N) && (ready_times[next_arrival] <= current_time)) {
            // add job to ready_queue
            append_value(&tail, next_arrival);
            qsize += 1;

            // get next arrival
            arrival_itr += 1;
            next_arrival = sorted_ids[arrival_itr];
        }

        // ------------------
        // Select Job
        // ------------------
        if (qsize > 0) {
            id = select_job(&head, &tail, policy, current_time, &remaining_times, running);

            if (remaining_times[id] == execution_times[id]){
                start_times[id] = get_time(); // process start time
            }
        }

        // ------------------
        // Run Job
        // ------------------

        time_unit();
        running = true;

        // ------------------
        // Update Parameters
        // ------------------

        if (qsize > 0) {
            remaining_times[id] -= 1;
            total_remaining -= 1;

            // remove job from ready queue if complete
            if (remaining_times[id] == 0) {
                // node *tmp;
                // tmp = lookup(head, id);
                remove_head(&head);
                running = false;
                qsize -= 1;

                end_times[id] = get_time(); // process stop time
            }
        }
        current_time += 1;


    }

    // FIFO: traverse sorted ready_times[N], 
    // in a tie (arrival), lowest index goes first, no preemption

    // SJF: traverse sorted ready_times[N],
    //  check for ties in sorted ready_times[N], sort the ties by shortest execution_time
    // then if still tied, lowest index, no preemption
    // (in tie, arrival, shortest execution_time)
    
    // PSJF: traverse sorted ready_times[N],
    // upon arrival, add to ready queue
    // each time_step, check ready_queue for shortest remaining_time,

    // RR: get sorted ready_times[N] (linked list)
    // traverse sorted_ready_times; upon arrival, add to ready_queue;
    // in ready_queue, first item points to last item
    // each time_step, get next item in ready_queue
    // when complete (remaining_time == 0), remove item from ready_queue
    // if ready_queue is empty, do nothing at time_step 
    // move time_step forward, check for arrival

    // Scheduler Initiate Policy:  FIFO, RR, SJF, PSJF
    
    // Make mutable int array remaining_time[N] (steps left for each job)
    
    // Make int array waiting_queue[N] containing indices,
    // sort by ready_times
    
    // Make waiting queue into linked list
    // sorted by policy
        // SJF || PSJF: Sort again by execution_times

    // Linked list contains pointer to previous item and ptr to next item
        // RR: check previous item remaining_time, use prev if not finished
        // PSJF: Sort by remaining_time?

    // Traverse linked list, get next idx
        // run time_unit()
        // update remaining_time[idx]
        // RR: check remaining_time[idx-1]
        // 

    // Output


    return 0;
}

