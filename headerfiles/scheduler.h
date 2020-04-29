#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "linkedlist.h"
#include "definitions.h"

extern maybe_int select_job(node **head, node **tail, enum policy_type policy, maybe_int elapsed_time, maybe_int remain_times[], bool running);

#endif