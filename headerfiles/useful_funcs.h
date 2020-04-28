#ifndef USEFUL_FUNCS_H
#define USEFUL_FUNCS_H

#include "definitions.h"


extern int reduce(unsigned int (*f)(unsigned int,unsigned int), unsigned int nums[], unsigned int length);
extern unsigned int get_gcd(unsigned int a, unsigned int b);
extern unsigned int get_lcm(unsigned int a, unsigned int b);
extern unsigned int LCM(unsigned int nums[], unsigned int length);
extern unsigned int get_sort_idx(unsigned int nums[], unsigned int left, unsigned int right, unsigned int order);
extern void quicksort_recursion(unsigned int nums[], unsigned int left, unsigned int right, unsigned int order);
extern void quicksort(unsigned int nums[], unsigned int results[], unsigned int length);
extern void zeros(unsigned int results[], unsigned int length);
extern void repeat(unsigned int x, unsigned int results[]);
extern void id_sort(unsigned int inputs[], unsigned int ids[], unsigned int length);
extern unsigned int add(unsigned int a, unsigned int b);

#endif