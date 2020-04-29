#ifndef PROCESS_H
#define PROCESS_H

#include "definitions.h"
#include "IO.h"

extern long long get_time();
extern void time_unit();
extern pid start_process(int id, jobstat *stat, int exec_time);
extern pid process_control(int id, jobstat *stat, pid PID, pid prevPID, int exec_time, bool running, char names[MAXN][NAMESIZE]);
extern pid update_status(int id, pid PID, jobstat *stat);

#endif