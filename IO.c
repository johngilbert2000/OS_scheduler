
#include <stdio.h>
#include <string.h>
#include "definitions.h"

enum policy_type get_policy() {

    char S[4];
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
        printf("ERROR: Incorrect Policy - %s", S);
        return 1;
    }
    return policy;
}
