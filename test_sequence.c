#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int seq_size = 0;

    printf("Generating sequence for n = %d...\n", n);

    long *seq = Generate_2p3q_Seq(n, &seq_size);

    printf("Sequence size: %d\n", seq_size);
    printf("Sequence elements: ");
    
    // Print the sequence to verify order and values
    for (int i = 0; i < seq_size; i++) {
        printf("%ld ", seq[i]);
    }
    printf("\n");

    // Verification logic
    // 1. Check if sorted
    for (int i = 0; i < seq_size - 1; i++) {
        if (seq[i] > seq[i+1]) {
            printf("ERROR: Sequence is not sorted at index %d (%ld > %ld)\n", i, seq[i], seq[i+1]);
        }
    }

    // 2. Check if all elements are less than n
    for (int i = 0; i < seq_size; i++) {
        if (seq[i] >= n) {
             printf("ERROR: Element %ld is not smaller than n (%d)\n", seq[i], n);
        }
    }

    // Free the memory
    free(seq);

    return 0;
}

