#ifndef IO_H
#define IO_H

#include "definitions.h"

extern enum policy_type get_policy();
extern void make_dmesg(pid PID, long double start_time, long double stop_time);

#endif