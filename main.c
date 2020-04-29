#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include "headerfiles/useful_funcs.h"
#include "headerfiles/linkedlist.h"
#include "headerfiles/scheduler.h"
#include "headerfiles/IO.h"
#include "headerfiles/definitions.h"
#include "headerfiles/process.h"


// - optimize PSJF, SJF (4 ready queues instead of 1)

// - dmesg syscall
// - get_time() syscall (in proc_step)


// maybe_int clean_list(node **head, int *qsize, int *total_remaining, jobstat stats[], bool *running){
//     maybe_int num_finished;
//     maybe_int id;
//     num_finished = 0;
//     if (DEBUG) { printf("Cleaning the queue \n"); }

//     id = (*head)->val;
//     while ((qsize > 0) && (stats[id] == FINISHED)) {
//         *running = false;
//         remove_head(head);
//         *qsize = *qsize - 1;
//         *total_remaining = *total_remaining - 1;
//         num_finished += 1;
//         if (qsize > 0) { id = (*head)->val; }
//     }
//     return num_finished;
// }

int main(int argc, char *argv[]) {

    enum policy_type policy; 

    // -------------
    // Get Policy
    // -------------
    policy = get_policy();

    // N
    maybe_int N;
    if (IO) printf("Number processes: ");
    scanf("%u", &N);

    N += 1; // because reasons

    int total_remaining;
    total_remaining = N; // number of processes left to finish

    // --------------
    // Input Stuff
    // --------------

    char names[MAXN][32];
    maybe_int ready_times[MAXN];
    maybe_int execution_times[MAXN];
    maybe_int remaining_times[MAXN];
    
    pid PIDs[MAXN];
    maybe_int current_step, current_process_step, total_steps;
    maybe_int elapsed_steps[MAXN]; // elapsed time_units() of each individual process   

    jobstat stats[MAXN];
    // N R T
    for (int i = 0; i < N; i++) { // because reasons
        if (IO) printf("N R T: ");
        scanf("%s", names[i]); 
        scanf("%u", &ready_times[i]);
        scanf("%u", &execution_times[i]);

        remaining_times[i] = execution_times[i];
        PIDs[i] = -42; // arbitrarily initialize PID
        elapsed_steps[i] = 0;
        stats[i] = UNAVAILABLE;
    }
    
    // because reasons
    // the last process wasn't getting displayed, so I made an extra process that runs at the end...
    // int maximum = 0;
    // int maximum2 =0;
    // for (int i = 0; i < N-1; i++) {
    //     if (ready_times[i] > maximum){
    //         maximum = ready_times[i];
    //     }
    // }
    // for (int i = 0; i < N-1; i++) {
    //     if (execution_times[i] > maximum){
    //         maximum2 = ready_times[i];
    //     }
    // }
    // strcpy(names[N-1], "");
    // ready_times[N-1] = maximum+10;
    // execution_times[N-1] = maximum2+10;
    // remaining_times[N-1] = maximum2+10;
    // elapsed_steps[N-1] = 0;
    // stats[N-1] = UNAVAILABLE;
    // PIDs[N-1] = -42;

    // ------------------
    // Sort Ready Times
    // ------------------
    maybe_int sorted_ids[MAXN];
    id_sort(ready_times, sorted_ids, N);
    if (DEBUG) printf("Sorted ids \n");

    // ------------------
    // Parameters
    // ------------------
    maybe_int prev_id; // previous selected id
    maybe_int id = 0; // current selected id
    int qsize = 0; // size of ready_queue (linked list)

    // int pipe_fds[2][MAXN];

    bool running;

    // get next job to arrive
    maybe_int arrival_itr = 0;
    maybe_int next_arrival; 
    next_arrival = sorted_ids[arrival_itr];

    node *head;
    node *tail;
    node *tmp;
    tmp = create_node(next_arrival);
    head = tmp;
    tail = tmp;

    running = false; // indicates previous job is still running; (used for SJF)
    
    total_steps = reduce(add, execution_times, N); // number of time_units() to complete in total
    current_step = 0;
    maybe_int finished_jobs = 0;

    int ready_queue[MAXN];

    // for (int i = 0; i < N; i++) {
    //     printf("%s %d %d\n", names[i], ready_times[i], execution_times[i]);
    // }

    while (finished_jobs < N) {
        // ------------------
        // Update Ready Queue
        // ------------------
        // Check if next job arrived, and if so, update the ready queue
        if (DEBUG) printf("___________ \n");
        if (DEBUG) printf("adding jobs\n");

        // for (int i = 0; i < N; i++) {
        //     if ((ready_times[i] <= current_step) && (stats[i] == UNAVAILABLE)) {
        //         printf("Add job: %id\n", id);
        //         append_value(&tail, next_arrival);
        //     }
        // }

        // -----------
        // Add jobs
        // -----------

        qsize = add_process(ready_times, current_step, ready_queue, N);


        // tmp1 = ready_times[next_arrival];
        // while ((arrival_itr < N) && (tmp1 <= current_step)) {
        //     // add job to ready_queue
        //     if (DEBUG) printf("Added: %d\n", sorted_ids[arrival_itr]);
        //     append_value(&tail, next_arrival);
        //     qsize += 1;
        //     // get next arrival
        //     arrival_itr += 1;
        //     next_arrival = sorted_ids[arrival_itr];
        // }

        if (DEBUG) printf("selecting jobs \n");

        if (qsize > 0) {
            // ------------------
            // Select Job
            // ------------------
            prev_id = id;
            // id = select_job(&head, &tail, policy, current_step, remaining_times, running);

            id = select_process(prev_id, ready_queue, remaining_times, elapsed_steps, current_step, policy, qsize, N, running);

// int select_process(int prev_id, int *ready_queue, int *remaining_times, \
//     int *elapsed_steps, int current_step, enum policy_type policy, int qsize, int N, bool running){

            // printf("id: %d\n", id);

            if (DEBUG) printf("running jobs \n");
            // ------------------
            // Run Job
            // ------------------
            PIDs[id] = process_control(id, &stats[id], PIDs[id], PIDs[prev_id], \
                execution_times[id], running);
            running = true;
            time_unit();
            if (DEBUG) printf("PIDs[%d]: %d\n", id, PIDs[id]);

            if (DEBUG) printf("update params \n");
            // ------------------
            // Update Parameters
            // ------------------
            elapsed_steps[id] = update_status(id, PIDs[id], &stats[id]);
            remaining_times[id] = remaining_times[id] - elapsed_steps[id];
            // update_status(id, PIDs[id], &stats[id], pipe_fds[id]);

            // finished_jobs += clean_list(&head, &qsize, &total_remaining, stats, &running);
            finished_jobs += cleaned_queue(ready_queue, stats, N);
            current_process_step = reduce(add, elapsed_steps, N); // elapsed process steps

            if (DEBUG) printf("%d / %d\n", finished_jobs, N);
            elapsed_steps[id] += 1;
        }
        current_step += 1;
        sleep(1);


        if (DEBUG) printf("sync?\n");
        if (DEBUG) printf("___________ \n");
        // ------------------
        // Sync Steps
        // ------------------
        // if (current_process_step > current_step) {
        //     // sync main() steps with process steps
        //     current_step = current_process_step;
        // }
    }

    if (IO) printf("_____________\n");
    for (int i = 0; i < N-1; i++) { // because reasons
        printf("%s", names[i]);
        printf(" %d\n", PIDs[i]);
    }
    if (IO) printf("_____________\n");

    // sleep(1);

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
        //     maybe_int *rtime_ptr = NULL; 
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

// - pipes, jobstatus [done?]
// - process_control [done?]
// - update_status [done?]
// - cleanlist() --> total_remaining_procs--, qsize-- [done]
