#define ARENA_IMPLEMENTATION
#include "arena.h"

#include <stdio.h>

// A simple linked list node
typedef struct Node {
    int value;
    struct Node *next;
} Node;

int main(void) {
    // 1. Initialize the arena with 1KB of memory
    Arena *arena = arena_init(1024);
    if (arena == NULL) {
        fprintf(stderr, "Failed to initialize arena.\n");
        return 1;
    }

    printf("Arena initialized. Creating a linked list...\n");

    // 2. allocate nodes efficiently without calling malloc() for each one
    Node *head = NULL;
    Node *current = NULL;

    for (int i = 0; i < 5; i++) {
        // Use the macro to allocate a struct directly from the arena
        Node *new_node = arena_push_struct(arena, Node);
        
        new_node->value = i * 10;
        new_node->next = NULL;

        if (head == NULL) {
            head = new_node;
            current = head;
        } else {
            current->next = new_node;
            current = new_node;
        }
        
        printf("  Allocated Node %d at offset: %zu\n", i, arena->offset);
    }

    // 3. Print the list to prove it works
    printf("\nTraversing list:\n");
    current = head;
    while (current != NULL) {
        printf("  Node Value: %d\n", current->value);
        current = current->next;
    }

    // 4. Free EVERYTHING at once.
    // Instead of iterating through the list and freeing each node,
    // we just destroy the arena. Fast and prevents memory leaks.
    printf("\nFreeing arena...\n");
    arena_free(arena);
    printf("Done.\n");

    return 0;
}