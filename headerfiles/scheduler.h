#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "linkedlist.h"
#include "definitions.h"

extern int add_process(int ready_times[], int current_step, int **ready_or_not, int N);
extern int select_process(bool *ready_or_not, int *remaining_times, int current_step, enum policy_type policy, bool running, int N);

extern maybe_int select_job(node **head, node **tail, enum policy_type policy, maybe_int elapsed_time, maybe_int remain_times[], bool running);

#endif