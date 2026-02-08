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
        // Array Mode
        int size = 0;
        long *array = Array_Load_From_File(input_file, &size);
        
        // Check if load failed (array is NULL and size is 0 is valid for empty file, 
        // but if load failed due to file error, we should exit.
        // Spec says "return NULL... only if you could not open file or fail to allocate".
        // However, empty file also returns size 0. We need to distinguish inside Load?
        // Actually, Load sets size=0 on error too.
        // A robust check: try to open file to see if it exists?
        // Let's rely on the pointer. If NULL and size=0, it might be empty or error.
        // But spec 1.2 says return NULL on error.
        // It says "If input file is empty... return valid address (non-null? spec says 'array of size 0' - malloc(0) might be non-null)".
        // Wait, spec 1.2: "return NULL address ... only if could not open or fail alloc".
        // So if array is NULL, it is a failure.
        
        if (array == NULL && size != 0) { // If size was somehow set but array is null (unlikely)
             return EXIT_FAILURE;
        }
        // If array is NULL and size is 0, it might be an error or just malloc(0) returning NULL (implementation dependent).
        // Let's re-read spec 1.2 carefully. 
        // "return NULL... ONLY IF could not open... or fail alloc". 
        // "If input file is empty... array of size 0 created".
        // So valid empty file -> non-NULL (ideally) or NULL with specific logic.
        // To be safe, we check if file opens. But Load does that.
        // Let's assume Load returns NULL on failure.
        // But malloc(0) might return NULL on some systems.
        // However, standard says malloc(0) is implementation defined.
        // Let's rely on the fact that for this assignment, we should assume NULL = error for array load.
        
        // Actually, simpler check: if we can't open the file inside Load, we return NULL. 
        // We can just assume NULL is failure for the purpose of the assignment unless the file is actually 0 bytes.
        // Let's trust the Load function logic.
        
        if (array == NULL) {
            // Check if it was actually an empty file by trying to open it?
            // Or just check strictly.
            // Let's allow a NULL return if the file size is 0.
            FILE *fp_check = fopen(input_file, "rb");
            if (!fp_check) return EXIT_FAILURE;
            fseek(fp_check, 0, SEEK_END);
            long fsize = ftell(fp_check);
            fclose(fp_check);
            if (fsize > 0) return EXIT_FAILURE; // Should have loaded something
            // If fsize == 0, we proceed with NULL array
        }

        Array_Shellsort(array, size, &n_comp);
        
        fprintf(stdout, "%ld\n", n_comp);

        int saved_count = Array_Save_To_File(output_file, array, size);
        
        // Free memory
        if (array) free(array);

        if (saved_count == -1) {
            return EXIT_FAILURE;
        }

    } else if (strcmp(mode, "-l") == 0) {
        // List Mode
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