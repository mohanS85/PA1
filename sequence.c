#include <stdlib.h>
#include <stdio.h>
#include "sequence.h"

static void insertion_sort(long *array, int size) {
    // Insertion sort algorithm
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
    // Even if empty, allocate 0 bytes
    if (n <= 1) {
        *seq_size = 0;
        return (long *)malloc(0); 
    }

    // Count number of valid elements
    int count = 0;
    long p2 = 1; 
    while (p2 < n) {
        long p3 = 1;
        while (p2 * p3 < n) {
            count++;
            // Check for overflow before multiplying
            if (n / 3 < p2 * p3) {
                break;
            }
            p3 *= 3;
        }
        // Check for overflow before multiplying
        if (n / 2 < p2) {
            break;
        }
        p2 *= 2;
    }

    *seq_size = count;
    long *seq = (long *)malloc(sizeof(long) * count);
    if (seq == NULL) {
        *seq_size = 0;
        return NULL;
    }

    // Populate array
    int idx = 0;
    p2 = 1;
    while (p2 < n) {
        long p3 = 1;
        while (p2 * p3 < n) {
            seq[idx++] = p2 * p3;
            if (n / 3 < p2 * p3) {
                break;
            }
            p3 *= 3;
        }
        if (n / 2 < p2) {
            break;
        }
        p2 *= 2;
    }

    // Sort sequence with insertion sort
    insertion_sort(seq, count);

    return seq;
}

