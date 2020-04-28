#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "headerfiles/definitions.h"

void map(uint (*f)(unsigned int), uint nums[], uint results[], uint n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */

    // Example:
    // uint squared(uint n){
    //     return n*n;
    // }

    // uint arr[3] = {9,21,3};
    // uint res[3] = {0,0,0};
    // map(squared, arr, res, 3); // --> res == {81, 441, 9}

    for (uint i=0; i<n; i++) {
        results[i] = (*f)(nums[i]);
    }
}

void filter(uint (*expr)(unsigned int), uint nums[], uint results[], uint n){
    /* maps f over nums and stores results in results[]
       nums[] and results[] are of same length n */
    
    // Example:
    // uint expression(uint n){
    //     return (n % 2) ? 1 : 0;
    // };
    // uint arr[3] = {1,2,3};
    // uint res[3] = {0,0,0};
    // filter(expression, arr,res,3); // -->  res == {1,3,0}

    uint offset = 0;
    for (uint i=0; i<n; i++) {
        if ((*expr)(nums[i])) {
            results[i-offset] = nums[i];
        }
        else {
            offset += 1;
        }
    }
}

uint reduce(uint (*f)(unsigned int,unsigned int), uint nums[], uint length){
    /* Reduces a list integer nums[] of given length with function f */

    // Example:
    // uint add(uint a, uint b){ return a + b };
    // uint arr[3] = {1,2,7};
    // uint res = reduce(add, arr, 3) // --> res == 1+2+7 == 10

    uint res = nums[0];
    for (uint i = 1; i < length; i++) {
        res = (*f)(res, nums[i]);
    }
    return res;
}


uint get_sort_idx(uint nums[], uint left, uint right, uint order) {
    // used in quicksort function

    // nums[] array of numbers
    // left: low value of partition
    // right: high value of partition
    // order: 0 ascending, 1 descending
    uint pivot, idx, temp;
    pivot = nums[right]; // initialize comparison with rightmost point
    idx = left; // initialize point to swap as leftmost point
    for (uint i = left; i < right; i++){
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


void quicksort_recursion(uint nums[], uint left, uint right, uint order){
    uint idx;
    if (left < right) {
        idx = get_sort_idx(nums, left, right, order); // swaps vals and gives pivot point
        quicksort_recursion(nums, left, idx-1, order); // before pivot
        quicksort_recursion(nums, idx+1, right, order);// after pivot
    }
}


void quicksort(uint nums[], uint results[], uint length){
    // turns results[] into a sorted version of nums[]

    // Example:
    // uint arr[4] = {1,3,5,2};
    // uint res[4] = {0,0,0,0};
    // quicksort(arr, res, 4); // --> res == {1,2,3,5}

    uint left = 0;
    uint right = length-1;
    uint order = 1; // 1 ascending, 0 descending

    // copy values in nums[] to results[]
    for (uint i=0; i<length; i++) {
        results[i] = nums[i];
    }
    // sort results
    quicksort_recursion(results, left, right, order);
}


void id_sort(uint inputs[], uint ids[], uint length){
    // Sorts ids by values in inputs

    uint sorted_inputs[length];
    quicksort(inputs, sorted_inputs, length);

    uint i = 0;
    while (i < length) {
        for (uint j = 0; j < length; j++) {
            if (inputs[i] == sorted_inputs[j]) {
                ids[i] = j;
                break;
            }
        }
        i += 1;
    }
}

uint add(uint a, uint b) {
    return a + b;
}

// uint get_lcm(uint a, uint b){
//     /* Least Common Multiple of two integers a and b */
//     return a*b / get_gcd(a,b);
// }


// uint LCM(uint nums[], uint length) {
//     /* Least Common Multiple from an array of integers of given length */
//     return reduce(get_lcm, nums, length);
// }

// uint get_gcd(uint a, uint b){ 
//     /* Greatest Common Denominator of two integers a and b */
//     if (b == 0)
//         return a;
//     else
//         return get_gcd(b, a % b);
// }

// uint GCD(uint nums[], uint length){
//     /* Greatest Common Denominator from an array of integers of given length */
//     return reduce(get_gcd, nums, length);
// }

// void zeros(uint results[], uint length) {
//     // turns nums into an array of 0's up to given length
//     for (uint i = 0; i < length; i++) {
//         results[i] = 0;
//     }
// }

// void repeat(uint x, uint results[]) {
//     // fills array results[] with values x, up to length x
//     for (uint i = 0; i < x; i++) {
//         results[i] = x;
//     }
// }
