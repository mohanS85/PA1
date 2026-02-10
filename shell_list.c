#include <stdio.h>
#include <stdlib.h>
#include "shell_list.h"
#include "sequence.h"

Node *List_Load_From_File(char *filename, int *status) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        *status = 1;
        return NULL;
    }

    // Determine file size
    fseek(fp, 0, SEEK_END);
    long file_bytes = ftell(fp);
    rewind(fp);

    int num_longs = file_bytes / sizeof(long);

    if (num_longs == 0) {
        *status = 0;
        fclose(fp);
        return NULL;
    }

    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < num_longs; i++) {
        long val;
        if (fread(&val, sizeof(long), 1, fp) != 1) {
            // Read error, cleanup
            while (head) {
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            *status = 1;
            fclose(fp);
            return NULL;
        }

        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL) {
            // Alloc error, cleanup
            while (head) {
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            *status = 1;
            fclose(fp);
            return NULL;
        }
        new_node->value = val;
        new_node->next = NULL;

        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    *status = 0;
    fclose(fp);
    return head;
}

int List_Save_To_File(char *filename, Node *list) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }

    int count = 0;
    Node *curr = list;
    while (curr != NULL) {
        if (fwrite(&(curr->value), sizeof(long), 1, fp) != 1) {
            break; 
        }
        count++;
        curr = curr->next;
    }

    fclose(fp);
    return count;
}

// Helper to count nodes
static int get_list_size(Node *list) {
    int count = 0;
    while (list) {
        count++;
        list = list->next;
    }
    return count;
}

// Helper to swap nodes in a list
// Returns the new head of the list
static Node* swap_nodes(Node *head, Node *prevA, Node *A, Node *prevB, Node *B) {
    if (A == B) {
        return head;
    }

    Node *nextA = A->next;
    Node *nextB = B->next;

    // Check if adjacent
    if (A->next == B) {
        // Link before A to B
        if (prevA) {
            prevA->next = B;
        }
        else {
            head = B;
        }

        B->next = A;
        A->next = nextB;
    } else {
        // Link before A to B
        if (prevA) {
            prevA->next = B;
        }
        else {
            head = B;
        }

        // Link before B to A
        if (prevB) {
            prevB->next = A;
        }

        B->next = nextA;
        A->next = nextB;
    }

    return head;
}

Node *List_Shellsort(Node *list, long *n_comp) {
    *n_comp = 0;
    if (list == NULL || list->next == NULL) {
        return list;
    }

    int size = get_list_size(list);
    int seq_size = 0;
    long *seq = Generate_2p3q_Seq(size, &seq_size);

    // If malloc fails, use bubble sort (gap = 1)
    int use_fallback = (seq == NULL && seq_size == 0);
    
    // If not fallback, iterate the sequence 
    // If fallback, iterate once with k=1
    int loops = use_fallback ? 1 : seq_size;

    for (int s = loops - 1; s >= 0; s--) {
        int k;
        if (use_fallback) {
            k = 1;
        }
        else {
            k = (int)seq[s];
        }

        int sorted = 0;
        while (!sorted) {
            sorted = 1;
            Node *prev_i = NULL;
            Node *node_i = list;
            
            // Bubble sort pass
            while (node_i != NULL) {
                // Find node_j which is k steps ahead of node_i
                Node *prev_j = node_i;
                Node *node_j = node_i->next;
                int steps = 1;
                
                // Advance to the k-th neighbor
                while (steps < k && node_j != NULL) {
                    prev_j = node_j;
                    node_j = node_j->next;
                    steps++;
                }

                if (node_j == NULL) {
                    // No k-th neighbor exists, end of this pass
                    break;
                }

                *n_comp += 1;
                if (node_i->value > node_j->value) {
                    // Swap needed
                    list = swap_nodes(list, prev_i, node_i, prev_j, node_j);
                    sorted = 0;
                    prev_i = node_j; 
                    node_i = node_j->next;
                } else {
                    // No swap, just advance
                    prev_i = node_i;
                    node_i = node_i->next;
                }
            }
        }
    }

    if (seq) {
        free(seq);
    }
    return list;
}

