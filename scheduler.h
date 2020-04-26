#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "linkedlist.h"
#include "definitions.h"

extern uint select_job(node **head, node **tail, enum policy_type policy, uint elapsed_time, uint remain_times[], bool running);

#endif