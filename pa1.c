#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sequence.h"
#include "shell_array.h"
#include "shell_list.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return EXIT_FAILURE;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];

    long n_comp = 0;

    if (strcmp(mode, "-a") == 0) {
        // Array mode
        int size = 0;
        long *array = Array_Load_From_File(input_file, &size);
        
        // Check if load failed
        if (array == NULL && size != 0) {
             return EXIT_FAILURE;
        }
        // If file cannot be opened, return NULL
        if (array == NULL) {
            FILE *fp_check = fopen(input_file, "rb");
            if (!fp_check) {
                return EXIT_FAILURE;
            }
            fseek(fp_check, 0, SEEK_END);
            long fsize = ftell(fp_check);
            fclose(fp_check);
            if (fsize > 0) {
                return EXIT_FAILURE;
            }
            // If fsize is 0, continue with NULL array
        }

        Array_Shellsort(array, size, &n_comp);
        
        fprintf(stdout, "%ld\n", n_comp);

        int saved_count = Array_Save_To_File(output_file, array, size);
        
        // Free memory
        if (array) {
            free(array);
        }

        if (saved_count == -1) {
            return EXIT_FAILURE;
        }

    } else if (strcmp(mode, "-l") == 0) {
        // List mode
        int status = 0;
        Node *list = List_Load_From_File(input_file, &status);

        if (status != 0) {
            return EXIT_FAILURE;
        }

        list = List_Shellsort(list, &n_comp);

        fprintf(stdout, "%ld\n", n_comp);

        int saved_count = List_Save_To_File(output_file, list);

        // Free list
        while (list != NULL) {
            Node *temp = list;
            list = list->next;
            free(temp);
        }

        if (saved_count == -1) {
            return EXIT_FAILURE;
        }

    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}