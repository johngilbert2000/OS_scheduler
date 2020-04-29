#ifndef USEFUL_FUNCS_H
#define USEFUL_FUNCS_H

#include "definitions.h"

extern int reduce(int (*f)(int, int), int nums[], int length);
extern int get_sort_idx(int nums[], int left, int right, int order);
extern void quicksort_recursion(int nums[], int left, int right, int order);
extern void quicksort(int nums[], int results[], int length);
extern void id_sort(int inputs[], int ids[], int length);
extern int add(int a, int b);
// extern int get_gcd(int a, int b);
// extern int get_lcm(int a, int b);
// extern int LCM(int nums[], int length);
// extern void zeros(int results[], int length);
// extern void repeat(int x, int results[]);

#endif