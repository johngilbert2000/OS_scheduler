#ifndef USEFUL_FUNCS_H
#define USEFUL_FUNCS_H

#include "definitions.h"

extern maybe_int reduce(maybe_int (*f)(maybe_int, maybe_int), maybe_int nums[], maybe_int length);
extern maybe_int get_sort_idx(maybe_int nums[], maybe_int left, maybe_int right, maybe_int order);
extern void quicksort_recursion(maybe_int nums[], maybe_int left, maybe_int right, maybe_int order);
extern void quicksort(maybe_int nums[], maybe_int results[], maybe_int length);
extern void id_sort(maybe_int inputs[], maybe_int ids[], maybe_int length);
extern maybe_int add(maybe_int a, maybe_int b);
// extern maybe_int get_gcd(maybe_int a, maybe_int b);
// extern maybe_int get_lcm(maybe_int a, maybe_int b);
// extern maybe_int LCM(maybe_int nums[], maybe_int length);
// extern void zeros(maybe_int results[], maybe_int length);
// extern void repeat(maybe_int x, maybe_int results[]);

#endif