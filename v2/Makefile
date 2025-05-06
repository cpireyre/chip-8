CFLAGS := -Wall -Wextra -Werror
CFLAGS += -std=c89 -pedantic
CFLAGS += -fsanitize=address,undefined -g

PHONY: all
all: main.c box.c io.c opcodes.c
	clang $(CFLAGS) $^ -o chip8

PHONY: test
test: all
	./chip8 "3-corax+.ch8"
