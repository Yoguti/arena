#define ARENA_IMPLEMENTATION
#include "arena.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

// Helper to print test status
void print_status(const char *test_name) {
    printf("[RUNNING] %-30s", test_name);
}

void print_pass() {
    printf("\t\033[0;32mPASSED\033[0m\n"); // Green text for pass
}

// 1. Test that we can create an arena
void test_initialization() {
    print_status("Initialization");
    
    size_t cap = 1024;
    Arena *a = arena_init(cap);
    
    assert(a != NULL);
    assert(a->capacity == cap);
    assert(a->offset == 0);
    assert(a->next == NULL);
    
    arena_free(a);
    print_pass();
}

// 2. Test simple allocation of integers
void test_basic_allocation() {
    print_status("Basic Allocation");
    
    Arena *a = arena_init(1024);
    
    // Allocate an int
    int *num = (int *)arena_alloc(a, sizeof(int));
    assert(num != NULL);
    *num = 42;
    
    // Allocate another int
    int *num2 = (int *)arena_alloc(a, sizeof(int));
    assert(num2 != NULL);
    *num2 = 84;
    
    // Check values
    assert(*num == 42);
    assert(*num2 == 84);
    
    // Check pointer arithmetic logic (offsets)
    // Note: This assumes specific behavior of your allocator
    assert(a->offset == sizeof(int) * 2);
    
    arena_free(a);
    print_pass();
}

// 3. Test the macros provided in the header
void test_macros() {
    print_status("Macros (Struct & Array)");
    
    Arena *a = arena_init(1024);
    
    typedef struct { int x; int y; } Point;
    
    // Test push_struct
    Point *p = arena_push_struct(a, Point);
    p->x = 10;
    p->y = 20;
    assert(p->x == 10 && p->y == 20);
    
    // Test push_array
    int count = 5;
    int *arr = arena_push_array(a, int, count);
    for(int i = 0; i < count; i++) arr[i] = i * 10;
    
    assert(arr[4] == 40);
    
    arena_free(a);
    print_pass();
}

// 4. Test that the arena creates a new node when it runs out of space
void test_overflow() {
    print_status("Overflow / Expansion");
    
    // Create a small arena (100 bytes)
    Arena *a = arena_init(100);
    
    // Allocate 80 bytes (Fits)
    void *p1 = arena_alloc(a, 80);
    assert(p1 != NULL);
    assert(a->next == NULL); // Should still be just one arena
    
    // Allocate 40 bytes (80 + 40 > 100, so it shouldn't fit)
    // This should trigger the creation of 'next'
    void *p2 = arena_alloc(a, 40);
    assert(p2 != NULL);
    
    // Verify a new block was linked
    assert(a->next != NULL);
    assert(a->next->capacity == 100);
    assert(a->next->offset == 40);
    
    arena_free(a);
    print_pass();
}

// 5. Test arena_reset (clearing offsets but keeping memory)
void test_reset() {
    print_status("Arena Reset");
    
    Arena *a = arena_init(1024);
    
    // Allocate something
    arena_alloc(a, 100);
    assert(a->offset == 100);
    
    // Reset
    arena_reset(a);
    
    // Offset should be back to 0
    assert(a->offset == 0);
    
    // Should be able to write to that memory again
    int *num = arena_push_struct(a, int);
    *num = 999;
    assert(*num == 999);
    
    arena_free(a);
    print_pass();
}

int main(void) {
    printf("\n Running Arena Tests \n");
    
    test_initialization();
    test_basic_allocation();
    test_macros();
    test_overflow();
    test_reset();

    printf("\n All Tests Passed \n");
    return 0;
}