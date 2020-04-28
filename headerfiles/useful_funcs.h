#ifndef USEFUL_FUNCS_H
#define USEFUL_FUNCS_H

#include "definitions.h"

extern uint reduce(uint (*f)(uint,uint), uint nums[], uint length);
extern uint get_sort_idx(uint nums[], uint left, uint right, uint order);
extern void quicksort_recursion(uint nums[], uint left, uint right, uint order);
extern void quicksort(uint nums[], uint results[], uint length);
extern void id_sort(uint inputs[], uint ids[], uint length);
extern uint add(uint a, uint b);
// extern uint get_gcd(uint a, uint b);
// extern uint get_lcm(uint a, uint b);
// extern uint LCM(uint nums[], uint length);
// extern void zeros(uint results[], uint length);
// extern void repeat(uint x, uint results[]);

#endif