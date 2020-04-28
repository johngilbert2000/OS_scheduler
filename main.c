#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include "headerfiles/useful_funcs.h"
#include "headerfiles/linkedlist.h"
#include "headerfiles/scheduler.h"
#include "headerfiles/IO.h"
#include "headerfiles/definitions.h"
#include "headerfiles/process.h"


// - pipes, jobstatus [done?]
// - process_control [done?]
// - update_status [done?]
// - cleanlist() --> total_remaining_procs--, qsize-- [almost done]
// - optimize PSJF, SJF (4 ready queues instead of 1)

// - dmesg syscall
// - get_time() syscall (in proc_step)


uint clean_list(node **head, int *qsize, int *total_remaining, jobstat stats[], bool *running){
    uint num_finished;
    uint id;
    num_finished = 0;

    id = (*head)->val;
    while ((qsize > 0) && (stats[id] == FINISHED)) {
        *running = false;
        remove_head(head);
        *qsize = *qsize - 1;
        *total_remaining = *total_remaining - 1;
        num_finished += 1;
        if (qsize > 0) { id = (*head)->val; }
    }
    return num_finished;
}

int main(int argc, char *argv[]) {

    enum policy_type policy; 

    // -------------
    // Get Policy
    // -------------
    policy = get_policy();

    // N
    uint N;
    if (IO) printf("Number processes: ");
    scanf("%u", &N);

    int total_remaining;
    total_remaining = N; // number of processes left to finish

    // input stuff
    char names[N][32];
    uint ready_times[N];
    uint execution_times[N];
    uint remaining_times[N];
    
    pid PIDs[N];
    uint current_step, current_process_step, total_steps;
    uint elapsed_steps[N]; // elapsed time_units() of each individual process   

    // N R T
    for (int i = 0; i < N; i++) {
        if (IO) printf("N R T: ");
        scanf("%s", names[i]); 
        scanf("%u", &ready_times[i]);
        scanf("%u", &execution_times[i]);

        remaining_times[i] = execution_times[i];
        PIDs[i] = -1; // arbitrarily initialize PID
        elapsed_steps[i] = 0;
    }

    // ------------------
    // Sort Ready Times
    // ------------------
    uint sorted_ids[N];
    id_sort(ready_times, sorted_ids, N);

    // ------------------
    // Parameters
    // ------------------
 
    // uint current_time = 0;

    uint prev_id; // previous selected id
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
    jobstat stats[N];
    int pipe_fds[2][N];

    total_steps = reduce(add, execution_times, N); // number of time_units() to complete in total
    current_step = 0;
    uint finished_jobs = 0;
    
    while (finished_jobs < N) {
        // ------------------
        // Update Ready Queue
        // ------------------
        // Check if next job arrived, and if so, update the ready queue
        while ((arrival_itr < N) && (ready_times[next_arrival] <= current_step)) {
            // add job to ready_queue
            append_value(&tail, next_arrival);
            qsize += 1;
            // get next arrival
            arrival_itr += 1;
            next_arrival = sorted_ids[arrival_itr];
        }

        if (qsize > 0) {
            // ------------------
            // Select Job
            // ------------------
            prev_id = id;
            id = select_job(&head, &tail, policy, current_step, remaining_times, running);
            
            // ------------------
            // Run Job
            // ------------------
            PIDs[id] = process_control(id, &stats[id], PIDs[id], PIDs[prev_id], \
                execution_times[id], pipe_fds[id], running);
            running = true;
            time_unit();

            // ------------------
            // Update Parameters
            // ------------------
            elapsed_steps[id] = update_status(id, PIDs[id], &stats[id], pipe_fds[id]);
            finished_jobs += clean_list(&head, &qsize, &total_remaining, stats, &running);
            current_process_step = reduce(add, elapsed_steps, N); // elapsed process steps
        }
        current_step += 1;

        // ------------------
        // Sync Steps
        // ------------------
        if (current_process_step > current_step) {
            // sync main() steps with process steps
            current_step = current_process_step;
        }
    }

    return 0;
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


// TODO (short version)
// - cleanlist(), total_remaining_procs, qsize
// - start_process()
// - change_process()
// - change_n_start()
// - PSJF, SJF: use 4 ready_queues based on execution time
// - syscall timer (add to processes)
// - printk syscall for dmesg
// - dmesg implementation
// - move header files into folder


// TODO:
// - change total_remaining to total_remaining_procs, initialized to N
// - decrement total_remaining_procs every time clean_list(head) is called
// - implement clean_list(node **head), which cycles through the ready_queue, 
// checks to see if process finished, and if so, remove from list;
// cleanlist(**head) stops when it hits a process that isn't finished
// cleanlist(**head) decrements total_remaining_procs AND qsize
// - implement start_process with fork(), getpid(), waitpid(), and pipe()
// - implement change_process
// - implement change_n_start_process

// - split PSJF and SJF ready queue into 4 ready queues, based on execution times
// i.e., ids with smaller execution times get added to ready_queue[0]'s linked list
// and ids with largest execution times get added to ready_queue[3]'s linked list
// when selecting jobs, if ready_queue[0] is empty, check ready_queue[1] for a linked list
// etc.; that way, the scheduler doesn't have to search through all available jobs
// to find the one with the minimum execution time

// - timer syscall for dmesg
// - printk syscall for dmesg

// NOT URGENT TODO:
// - Is it possible to move get policy stuff all into IO, 
// and store execution_times[] and ready_times[] and names[] in a struct?
// N will depend on user input...


// #include <linux/ktime.h> // for getnstimeofday()

// QUESTION: do I use fork to call time_unit(), with time_unit in a separate file?
// void time_unit(){
//     volatile unsigned long i; for(i=0;i<1000000UL;i++); 
// } 

// A note on implementation:
// a linked list is used to hold ids of available jobs
// the head of the linked list will always be the current job
// (some policies will move new jobs to the head of the list when selecting them)

// long long get_time(){
//     // gets the clock time in nanoseconds (from time.h)
//     struct timespec t;
//     clock_gettime(CLOCK_REALTIME, &t);
//     return (t.tv_sec*(int)1e9 + t.tv_nsec);
// }

    // run process
        // if (running == false) {
        //     uint *rtime_ptr = NULL; 
        //     *rtime_ptr = &remaining_times[id]; 
        //     PIDs[id] = start_process(&rtime_ptr);
        // }
        // else if (id != prev_id) {

        // }


        // if (running[id] == true) {
        //     continue_process(PID[id]);
        // }
        // else {
        //     PID = startprocess();
        // }

// if (remaining_times[id] == execution_times[id]){
//                 start_times[id] = get_time(); // process start time
//             }



            //     remaining_times[id] -= 1;
            //     total_remaining -= 1;

            //     // remove job from ready queue if complete
            //     if (remaining_times[id] == 0) {
            //         // node *tmp;
            //         // tmp = lookup(head, id);
            //         remove_head(&head);
            //         running = false;
            //         qsize -= 1;

            //         end_times[id] = get_time(); // process stop time
            //     }

            // current_time += 1;




    // total remaining execution time
    // int total_remaining = reduce(add, remaining_times, N);

