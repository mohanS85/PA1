#include <stdlib.h>
#include <stdio.h>
#include "sequence.h"

static void insertion_sort(long *array, int size) {
    int i, j;
    long temp;
    for (i = 1; i < size; i++) {
        temp = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > temp) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;
    }
}

long *Generate_2p3q_Seq(int n, int *seq_size) {
    if (n <= 1) {
        *seq_size = 0;
        // Even if empty, allocate 0 bytes as per spec (allocating 0 usually returns a non-null pointer or NULL depending on implementation, 
        // but the spec says "allocate a space of 0 elements").
        return (long *)malloc(0); 
    }

    // 1. Count the number of valid elements
    int count = 0;
    long p2 = 1; 
    while (p2 < n) {
        long p3 = 1;
        while (p2 * p3 < n) {
            count++;
            if (n / 3 < p2 * p3) break; // Check for overflow before multiplying
            p3 *= 3;
        }
        if (n / 2 < p2) break; // Check for overflow
        p2 *= 2;
    }

    *seq_size = count;
    long *seq = (long *)malloc(sizeof(long) * count);
    if (seq == NULL) {
        *seq_size = 0;
        return NULL;
    }

    // 2. Populate the array
    int idx = 0;
    p2 = 1;
    while (p2 < n) {
        long p3 = 1;
        while (p2 * p3 < n) {
            seq[idx++] = p2 * p3;
            if (n / 3 < p2 * p3) break;
            p3 *= 3;
        }
        if (n / 2 < p2) break;
        p2 *= 2;
    }

    // 3. Sort the sequence
    insertion_sort(seq, count);

    return seq;
}

