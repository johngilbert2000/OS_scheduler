#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "useful_funcs.h"
#include "linkedlist.h"
#include "definitions.h"

// typedef unsigned int uint;
// enum policy_type {FIFO, RR, SJF, PSJF};

uint select_job(node **head, node **tail, enum policy_type policy, uint elapsed_time, uint remain_times[], bool running){
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
            uint id;
            uint min_time;
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
        uint id;
        uint min_time;
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