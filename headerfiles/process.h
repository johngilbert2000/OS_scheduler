#ifndef PROCESS_H
#define PROCESS_H

#include "definitions.h"
#include "IO.h"

extern long long get_time();
extern void time_unit();
extern pid start_process(maybe_int id, jobstat *stat, maybe_int exec_time, int pipefd[2]);
extern pid process_control(maybe_int id, jobstat *stat, pid PID, pid prevPID, maybe_int exec_time, int pipefd[2], bool running);
extern pid update_status(int id, pid PID, jobstat *stat, int *fd);

#endif