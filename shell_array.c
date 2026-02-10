#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
#include "sequence.h"

long *Array_Load_From_File(char *filename, int *size) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        *size = 0;
        return NULL;
    }

    // Determine file size
    fseek(fp, 0, SEEK_END);
    long file_bytes = ftell(fp);
    rewind(fp);

    int num_longs = file_bytes / sizeof(long);
    *size = num_longs;

    if (num_longs == 0) {
        fclose(fp);
        return (long *)malloc(0);
    }

    long *array = (long *)malloc(sizeof(long) * num_longs);
    if (array == NULL) {
        *size = 0;
        fclose(fp);
        return NULL;
    }

    long read_count = fread(array, sizeof(long), num_longs, fp);
    if (read_count != num_longs) {
        free(array);
        *size = 0;
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return array;
}

int Array_Save_To_File(char *filename, long *array, int size) {
    if (filename == NULL) {
        return -1; // array can be NULL if size is 0
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }

    int written = fwrite(array, sizeof(long), size, fp);
    fclose(fp);
    return written;
}

void Array_Shellsort(long *array, int size, long *n_comp) {
    *n_comp = 0;
    if (array == NULL || size <= 1) {
        return;
    }

    int seq_size = 0;
    long *seq = Generate_2p3q_Seq(size, &seq_size);

    // If sequence generation fails, use standard insertion sort
    
    if (seq == NULL && seq_size == 0 && size > 1) {
        for (int i = 1; i < size; i++) {
            long temp = array[i];
            int j = i;
            while (j >= 1) {
                // comparison to do: array[j-1] > temp
                *n_comp += 1;
                if (array[j - 1] > temp) {
                    array[j] = array[j - 1];
                    j -= 1;
                } else {
                    break;
                }
            }
            array[j] = temp;
        }
        return;
    }

    // Iterate through sequence in reverse order (largest gap first)
    for (int s = seq_size - 1; s >= 0; s--) {
        int k = (int)seq[s];
        
        // Insertion sort with gap k
        for (int j = k; j < size; j++) {
            long temp = array[j];
            int i = j;
            while (i >= k) {
                // Comparison involving temp and array[i-k]
                *n_comp += 1;
                if (array[i - k] > temp) {
                    array[i] = array[i - k];
                    i = i - k;
                } else {
                    break;
                }
            }
            array[i] = temp;
        }
    }

    free(seq);
}

