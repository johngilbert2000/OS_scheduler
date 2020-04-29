#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "headerfiles/useful_funcs.h"
#include "headerfiles/linkedlist.h"
#include "headerfiles/definitions.h"

int getmax(int arr[], int N) {
    // returns max value of arr
    int maxval;
    maxval = arr[0];
    for (int i = 0; i < N; i++) {
        if (arr[i] > maxval) {
            maxval = arr[i];
        }
    }
    return maxval;
}

int get_pos_min(int arr[], int N, int qsize) {
    // returns minimum positive value of arr
    int minval;
    // minval = getmax(arr, N);
    minval = qsize; // initialize to largest value
    for (int i = 0; i < N; i++) {
        if (arr[i] > 0) {
            if (arr[i] < minval) minval = arr[i];
        }
    }
    return minval;
}





int add_process(int ready_times[], int current_step, int *ready_queue, int N) {
    int qsize = 0;
    for (int i = 0; i < N; i++) {
        if (ready_times[i] <= current_step) {
            ready_queue[i] = qsize;
            qsize += 1;
        }
        else {
            ready_queue[i] = -1;
        }
    }
    return qsize;
}

int cleaned_queue(int ready_queue[], jobstat stats[], int N) {
    for (int i = 0; i < N; i ++) {
        if (stats[i] == FINISHED) {
            ready_queue[i] = -2;
        }
    }
    return 0;
}


int select_by_FIFO(int ready_queue[], int N, int qsize) {
    int minval;
    minval = get_pos_min(ready_queue, N, qsize);

    for (int i = 0; i < N; i++) {
        if (ready_queue[i] == minval) {
            return i;
        }
    }
    if (DEBUG) printf("FIFO ERROR\n");
    return 0;
}


// int select_by_RR(int prev_id, bool *ready_or_not, int current_step, int N){

//     return 0;
// }


// int select_by_SJF(int prev_id, bool *ready_or_not, int *remaining_times, int *elapsed_steps, int current_step, enum policy_type policy, int N){

//     return 0;
// }


// int select_by_PSJF(int prev_id, bool *ready_or_not, int *remaining_times, int *elapsed_steps, int current_step, enum policy_type policy, int N){

//     return 0;
// }

int select_process(int prev_id, int ready_queue[], int remaining_times[], \
    int elapsed_steps[], int current_step, enum policy_type policy, int qsize, int N, bool running){

    int id; // minval; //maxval; 
    id = 0; //initialized to silence warnings
    if (policy == FIFO) {
        id = select_by_FIFO(ready_queue, N, qsize);
    }
    else if (policy == RR) {
        // id = select_by_RR(ready_or_not, current_step, elapsed_steps, N);
        id = select_by_FIFO(ready_queue, N, qsize);
        if (elapsed_steps[id] % 500 == 0) {

            // maxval = getmax(ready_queue, N);
            for (int i = 0; i< N; i++) {
                if (ready_queue[i] > 0) {
                    ready_queue[i] += 1 % qsize; // shift the ready queue
                }
            }
            id = select_by_FIFO(ready_queue, N, qsize);
        }
        // for (int i = 0; i < N; i++) {
        //     if (ready_queue[i] > 0) {
        //         if (elapsed_steps[i] )
        //     }
        // }

    }
    else if (policy == SJF) {
        // id = select_by_SJF(ready_queue, remaining_times, current_step, N);
        if (running == 0) {

            int minval;
            minval = get_pos_min(remaining_times, N, qsize);
            for (int i = 0; i < N; i++) {

                if (remaining_times[i] == minval) {
                    id = i;
                    break;
                }
            }
        }
        else {
            id = prev_id;
        }
    }
    else {
        // id = select_by_PSJF(ready_or_not, remaining_times, current_step, N);
        int minval;
        id = 0;
        minval = get_pos_min(remaining_times, N, qsize);
        for (int i = 0; i < N; i++) {

            if (remaining_times[i] == minval) {
                id = i;
                break;
            }
        }

    }
    return id;
}


// id = select_process(ready_or_not, remaining_times, current_step, policy, running, N);




// typedef unsigned int uint;
// enum policy_type {FIFO, RR, SJF, PSJF};

maybe_int select_job(node **head, node **tail, enum policy_type policy, maybe_int elapsed_time, maybe_int remain_times[], bool running){
    // Selects job based on policy;

    // head: the head node of the ready queue (linked list)
    // policy: FIFO, RR, SJF, or PSJF
    // elapsed_time: time elapsed (for either process or main), used for RR
    // remain_times[]: array of execution times for each id; 
    // running: whether or not previous process is still running; used for SJF
    if (policy == FIFO) {
        return (*head)->val;
    }
    else if (policy == RR) {
        if (elapsed_time % 500 == 0) {
            shift_left(head, tail);
        }
        return (*head)->val;
    }
    else if (policy == SJF) {
        // check if current job is running
        if (running) {
            return (*head)->val;
        }
        else {
            // get id with minimum remaining execution time, 
            // and move selected job to head of ready queue list
            node *tmp;
            node *min_node;
            
            tmp = *head;
            // maybe_int id;
            maybe_int min_time;
            // id = tmp->val;
            min_time = remain_times[tmp->val];

            while (tmp != NULL){
                if (remain_times[tmp->val] < min_time) {
                    // id = tmp->val;
                    min_node = tmp;
                    min_time = remain_times[tmp->val];
                }
                tmp = tmp->next;
            }
            move_ahead(head, min_node);
            // *head = min_node;
            return min_node->val;
        }
    }
    else if (policy == PSJF) {
        // get id with minimum remaining execution time, 
        // and move selected job to head of ready queue list
        node *tmp;
        node *min_node;
        
        tmp = *head;
        maybe_int min_time;
        min_time = remain_times[tmp->val];

        while (tmp != NULL){
            if (remain_times[tmp->val] < min_time) {
                min_node = tmp;
                min_time = remain_times[tmp->val];
            }
            tmp = tmp->next;
        }
        move_ahead(head, min_node);
        return min_node->val;
    }
    else {
        fprintf(stderr, "Improper policy used in selection: %d", policy);
        exit(1);
    }
}