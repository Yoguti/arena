# make examples, make test and make clean
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -I.

# Define the output directory
BIN_DIR = bin

# Update paths to point to the BIN_DIR
TEST_SRC = tests/tests.c
TEST_BIN = $(BIN_DIR)/tests

EXAMPLES_SRC = $(wildcard examples/*.c)
# Change the target list from "examples/name" to "bin/name"
EXAMPLES_BIN = $(patsubst examples/%.c, $(BIN_DIR)/%, $(EXAMPLES_SRC))

.PHONY: all test examples clean directories

all: directories test examples

# Rule to ensure bin/ directory exists
directories:
	@mkdir -p $(BIN_DIR)

# Link directories rule to test so it runs first
test: $(TEST_SRC) | directories
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_BIN)
	./$(TEST_BIN)

examples: $(EXAMPLES_BIN)

# Generic rule: How to build bin/% from examples/%.c
$(BIN_DIR)/%: examples/%.c | directories
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)