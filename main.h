
typedef unsigned int uint;

enum policy_type {FIFO, RR, SJF, PSJF};

struct linked {
    int val;
    struct linked *next;
    struct linked *prev;
};

typedef struct linked node;