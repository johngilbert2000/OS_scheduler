#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "headerfiles/definitions.h"

void map(maybe_int (*f)(unsigned int), maybe_int nums[], maybe_int results[], maybe_int n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */

    // Example:
    // maybe_int squared(maybe_int n){
    //     return n*n;
    // }

    // maybe_int arr[3] = {9,21,3};
    // maybe_int res[3] = {0,0,0};
    // map(squared, arr, res, 3); // --> res == {81, 441, 9}

    for (maybe_int i=0; i<n; i++) {
        results[i] = (*f)(nums[i]);
    }
}

void filter(maybe_int (*expr)(unsigned int), maybe_int nums[], maybe_int results[], maybe_int n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */
    
    // Example:
    // maybe_int expression(maybe_int n){
    //     return (n % 2) ? 1 : 0;
    // };
    // maybe_int arr[3] = {1,2,3};
    // maybe_int res[3] = {0,0,0};
    // filter(expression, arr,res,3); // -->  res == {1,3,0}

    maybe_int offset = 0;
    for (maybe_int i=0; i<n; i++) {
        if ((*expr)(nums[i])) {
            results[i-offset] = nums[i];
        }
        else {
            offset += 1;
        }
    }
}

maybe_int reduce(maybe_int (*f)(maybe_int, maybe_int), maybe_int nums[], maybe_int length){
    /* Reduces a list integer nums[] of given length with function f */

    // Example:
    // maybe_int add(maybe_int a, maybe_int b){ return a + b };
    // maybe_int arr[3] = {1,2,7};
    // maybe_int res = reduce(add, arr, 3) // --> res == 1+2+7 == 10

    maybe_int res = nums[0];
    for (maybe_int i = 1; i < length; i++) {
        res = (*f)(res, nums[i]);
    }
    return res;
}


maybe_int get_sort_idx(maybe_int nums[], maybe_int left, maybe_int right, maybe_int order) {
    // used in quicksort function

    // nums[] array of numbers
    // left: low value of partition
    // right: high value of partition
    // order: 0 ascending, 1 descending
    maybe_int pivot, idx, temp;
    pivot = nums[right]; // initialize comparison with rightmost point
    idx = left; // initialize point to swap as leftmost point
    for (maybe_int i = left; i < right; i++){
        if (order) {
            // ascending
            if (nums[i] < pivot) {
                // swap nums[i] with nums[idx], increment idx
                if (i != idx) {
                    temp = nums[idx];
                    nums[idx] = nums[i];
                    nums[i] = temp;
                }
                idx += 1;
            }
        } else {
            // descending
            if (nums[i] > pivot) {
                // swap nums[i] with nums[idx+1], increment idx, 
                if (i != idx) {
                    temp = nums[idx];
                    nums[idx] = nums[i];
                    nums[i] = temp;
                }
                idx += 1;
            }
        }
    }
    // swap nums[right] (pivot) with nums[idx]
    temp = nums[idx];
    nums[idx] = nums[right];
    nums[right] = temp;
    return idx;
}


void quicksort_recursion(maybe_int nums[], maybe_int left, maybe_int right, maybe_int order){
    maybe_int idx;
    if (left < right) {
        idx = get_sort_idx(nums, left, right, order); // swaps vals and gives pivot point
        quicksort_recursion(nums, left, idx-1, order); // before pivot
        quicksort_recursion(nums, idx+1, right, order);// after pivot
    }
}


void quicksort(maybe_int nums[], maybe_int results[], maybe_int length){
    // turns results[] into a sorted version of nums[]

    // Example:
    // maybe_int arr[4] = {1,3,5,2};
    // maybe_int res[4] = {0,0,0,0};
    // quicksort(arr, res, 4); // --> res == {1,2,3,5}

    maybe_int left = 0;
    maybe_int right = length-1;
    maybe_int order = 1; // 1 ascending, 0 descending

    // copy values in nums[] to results[]
    for (maybe_int i=0; i<length; i++) {
        results[i] = nums[i];
    }
    // sort results
    quicksort_recursion(results, left, right, order);
}


void id_sort(maybe_int inputs[], maybe_int ids[], maybe_int length){
    // Sorts ids by values in inputs

    maybe_int sorted_inputs[length];
    quicksort(inputs, sorted_inputs, length);

    maybe_int i = 0;
    while (i < length) {
        for (maybe_int j = 0; j < length; j++) {
            if (inputs[i] == sorted_inputs[j]) {
                ids[i] = j;
                break;
            }
        }
        i += 1;
    }
}

maybe_int add(maybe_int a, maybe_int b) {
    return a + b;
}

// maybe_int get_lcm(maybe_int a, maybe_int b){
//     /* Least Common Multiple of two integers a and b */
//     return a*b / get_gcd(a,b);
// }


// maybe_int LCM(maybe_int nums[], maybe_int length) {
//     /* Least Common Multiple from an array of integers of given length */
//     return reduce(get_lcm, nums, length);
// }

// maybe_int get_gcd(maybe_int a, maybe_int b){ 
//     /* Greatest Common Denominator of two integers a and b */
//     if (b == 0)
//         return a;
//     else
//         return get_gcd(b, a % b);
// }

// maybe_int GCD(maybe_int nums[], maybe_int length){
//     /* Greatest Common Denominator from an array of integers of given length */
//     return reduce(get_gcd, nums, length);
// }

// void zeros(maybe_int results[], maybe_int length) {
//     // turns nums into an array of 0's up to given length
//     for (maybe_int i = 0; i < length; i++) {
//         results[i] = 0;
//     }
// }

// void repeat(maybe_int x, maybe_int results[]) {
//     // fills array results[] with values x, up to length x
//     for (maybe_int i = 0; i < x; i++) {
//         results[i] = x;
//     }
// }
