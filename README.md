# Simple Arena Allocator

memory arena for C99.

This is a **single-header library** to make integration effortless.

## Project Structure

```text
.
├── arena.h         # The library itself
├── Makefile        # Standard makefile for building tests/examples
├── examples/       # Sample code showing how to use it
├── tests/          # Unit tests to make sure it works
└── bin/            # Where the compiled binaries go
```

## Integration

1. Copy `arena.h` to your project.
2. In **exactly one** source file (e.g., `main.c`), define the implementation:

```c
#define ARENA_IMPLEMENTATION
#include "arena.h"
```

3. In all other files, just `#include "arena.h"`.

## API Usage

```c
// 1. Initialize (starts with 1KB, expands automatically if needed)
Arena *mem = arena_init(1024);

// 2. Allocate (returns casted pointers)
MyStruct *data = arena_push_struct(mem, MyStruct);
int *array = arena_push_array(mem, int, 100);

// 3. Reset (keeps memory mapped, resets offset to 0)
arena_reset(mem);

// 4. Teardown (frees all memory blocks at once)
arena_free(mem);
```

## Building & Testing

The project comes with a `Makefile` to build the included tests and examples.

```bash
make        # Compiles examples and tests to ./bin
make test   # Compiles and runs the test suite immediately
make clean  # Removes the ./bin directory
```
