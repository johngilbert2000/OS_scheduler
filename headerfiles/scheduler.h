#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "linkedlist.h"
#include "definitions.h"

extern int select_process(int prev_id, int ready_queue[], int remaining_times[], \
    int elapsed_steps[], int current_step, enum policy_type policy, int qsize, int N, bool running);
extern int add_process(int ready_times[], int current_step, int ready_queue[], int N);
// extern int select_process(bool *ready_or_not, int *remaining_times, int current_step, enum policy_type policy, bool running, int N);
extern int cleaned_queue(int ready_queue[], jobstat stats[], int N);

extern maybe_int select_job(node **head, node **tail, enum policy_type policy, maybe_int elapsed_time, maybe_int remain_times[], bool running);

#endif