/* dev/arena.h - Simple memory arena allocator
 *
 * This header defines a simple memory arena allocator for efficient
 * memory management in C programs. It allows for fast allocation
 * and deallocation of memory blocks.
 *
 * Usage:
 * 1. Inside one C file, define ARENA_IMPLEMENTATION before including this
 * header #define ARENA_IMPLEMENTATION #include "dev/arena.h"
 * 2. In other files, simply include the header
 * #include "dev/arena.h"
 * 3. Use the provided functions to manage memory arenas.
 * Author: Klaus Schneider
 *
 */

#ifndef DEV_ARENA_H
#define DEV_ARENA_H

#include <stddef.h>
#include <stdlib.h>

typedef struct Arena
{
  struct Arena *next;
  size_t capacity;
  size_t offset;
  unsigned char buffer[];
} Arena;

Arena *arena_init(size_t capacity);                  // Initialize a new arena with given capacity
void *arena_alloc(Arena *arena, size_t size); // Allocate memory from arena
void arena_free(Arena *arena);                // Free entire arena
void arena_reset(Arena *arena);               // Clear memory but keep allocations

// Helper to allocate a specific struct
#define arena_push_struct(arena, type) (type *)arena_alloc(arena, sizeof(type))

// Helper to allocate an array
#define arena_push_array(arena, type, count) \
  (type *)arena_alloc(arena, sizeof(type) * (count))

#endif /* DEV_ARENA_H */

#ifdef ARENA_IMPLEMENTATION

Arena *arena_init(size_t capacity)
{
  Arena *arena = (Arena *)malloc(sizeof(*arena) + capacity);

  if (arena == NULL)
  {
    return NULL;
  }
  else
  {
    arena->next = NULL;
    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
  }
}

void *arena_alloc(Arena *arena, size_t size)
{
  if (arena->offset + size > arena->capacity)
  {
    /*
      Arena has ran out of space, we can go to the next one
    */
    if (arena->next == NULL)
    {
      arena->next = arena_init(arena->capacity);
    }
    return arena_alloc(arena->next, size);
  }
  else
  {
    arena->offset += size;
    return &arena->buffer[(arena->offset) - size];
  }
}

void arena_free(Arena *arena)
{
  if (arena == NULL)
  {
    return;
  }

  arena_free(arena->next);
  free(arena);
}

void arena_reset(Arena *arena) {
  if (arena == NULL)
  {
    return;
  }

  arena_reset(arena->next);
  arena->offset = 0;
}

#endif /* ARENA_IMPLEMENTATION */