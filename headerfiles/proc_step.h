#ifndef PROC_H
#define PROC_H

#include "definitions.h"
#include "IO.h"

extern long long get_time();
extern void time_unit();
extern pid start_process(uint id, jobstat *stat, uint exec_time, int pipefd[2]);
extern pid process_control(uint id, jobstat *stat, pid PID, pid prevPID, uint exec_time, int pipefd[2], bool running);
extern pid update_status(int id, int waitstatus, pid PID, jobstat *stat, int *fd);

#endif