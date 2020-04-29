
#include <stdio.h>
#include <string.h>
#include "headerfiles/definitions.h"

enum policy_type get_policy() {

    char S[NAMESIZE];
    enum policy_type policy;

    if (IO) printf("Policy: ");
    scanf("%s", S);

    if (strncmp(S,"FIFO", 4) == 0){
        policy = FIFO;
    }
    else if (strncmp(S, "RR", 4) ==0){
        policy = RR;
    }
    else if (strncmp(S, "SJF", 4) == 0){
        policy = SJF;
    }
    else if (strncmp(S, "PSJF",4) == 0){
        policy = PSJF;
    }
    else{
        printf("ERROR: Incorrect Policy - %s. Default FIFO used.\n", S);
        return FIFO;
    }
    return policy;
}

void make_dmesg(pid PID, long double start_time, long double stop_time) {
    // printf("[Project1] %d %Lf %Lf\n", PID, start_time, stop_time);
    // %llu long long
    // syscall(PRINTK_STUFF, PID, start_time, stop_time);
    return; 
}

