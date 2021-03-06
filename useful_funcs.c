#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "headerfiles/definitions.h"

void map(int (*f)(unsigned int), int nums[], int results[], int n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */

    // Example:
    // int squared(int n){
    //     return n*n;
    // }

    // int arr[3] = {9,21,3};
    // int res[3] = {0,0,0};
    // map(squared, arr, res, 3); // --> res == {81, 441, 9}

    for (int i=0; i<n; i++) {
        results[i] = (*f)(nums[i]);
    }
}

void filter(int (*expr)(unsigned int), int nums[], int results[], int n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */
    
    // Example:
    // int expression(int n){
    //     return (n % 2) ? 1 : 0;
    // };
    // int arr[3] = {1,2,3};
    // int res[3] = {0,0,0};
    // filter(expression, arr,res,3); // -->  res == {1,3,0}

    int offset = 0;
    for (int i=0; i<n; i++) {
        if ((*expr)(nums[i])) {
            results[i-offset] = nums[i];
        }
        else {
            offset += 1;
        }
    }
}

int reduce(int (*f)(int, int), int nums[], int length){
    /* Reduces a list integer nums[] of given length with function f */

    // Example:
    // int add(int a, int b){ return a + b };
    // int arr[3] = {1,2,7};
    // int res = reduce(add, arr, 3) // --> res == 1+2+7 == 10

    int res = nums[0];
    for (int i = 1; i < length; i++) {
        res = (*f)(res, nums[i]);
    }
    return res;
}


int get_sort_idx(int nums[], int left, int right, int order) {
    // used in quicksort function

    // nums[] array of numbers
    // left: low value of partition
    // right: high value of partition
    // order: 0 ascending, 1 descending
    int pivot, idx, temp;
    pivot = nums[right]; // initialize comparison with rightmost point
    idx = left; // initialize point to swap as leftmost point
    for (int i = left; i < right; i++){
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


void quicksort_recursion(int nums[], int left, int right, int order){
    int idx;
    if (left < right) {
        idx = get_sort_idx(nums, left, right, order); // swaps vals and gives pivot point
        quicksort_recursion(nums, left, idx-1, order); // before pivot
        quicksort_recursion(nums, idx+1, right, order);// after pivot
    }
}


void quicksort(int nums[], int results[], int length){
    // turns results[] into a sorted version of nums[]

    // Example:
    // int arr[4] = {1,3,5,2};
    // int res[4] = {0,0,0,0};
    // quicksort(arr, res, 4); // --> res == {1,2,3,5}

    int left = 0;
    int right = length-1;
    int order = 1; // 1 ascending, 0 descending

    // copy values in nums[] to results[]
    for (int i=0; i<length; i++) {
        results[i] = nums[i];
    }
    // sort results
    quicksort_recursion(results, left, right, order);
}


void id_sort(int inputs[], int ids[], int length){
    // Sorts ids by values in inputs

    int sorted_inputs[length];
    quicksort(inputs, sorted_inputs, length);

    int i = 0;
    while (i < length) {
        for (int j = 0; j < length; j++) {
            if (inputs[i] == sorted_inputs[j]) {
                ids[i] = j;
                break;
            }
        }
        i += 1;
    }
}

int add(int a, int b) {
    return a + b;
}

// int get_lcm(int a, int b){
//     /* Least Common Multiple of two integers a and b */
//     return a*b / get_gcd(a,b);
// }


// int LCM(int nums[], int length) {
//     /* Least Common Multiple from an array of integers of given length */
//     return reduce(get_lcm, nums, length);
// }

// int get_gcd(int a, int b){ 
//     /* Greatest Common Denominator of two integers a and b */
//     if (b == 0)
//         return a;
//     else
//         return get_gcd(b, a % b);
// }

// int GCD(int nums[], int length){
//     /* Greatest Common Denominator from an array of integers of given length */
//     return reduce(get_gcd, nums, length);
// }

// void zeros(int results[], int length) {
//     // turns nums into an array of 0's up to given length
//     for (int i = 0; i < length; i++) {
//         results[i] = 0;
//     }
// }

// void repeat(int x, int results[]) {
//     // fills array results[] with values x, up to length x
//     for (int i = 0; i < x; i++) {
//         results[i] = x;
//     }
// }
