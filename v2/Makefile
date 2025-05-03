CFLAGS := -Wall -Wextra -Werror -Wimplicit-fallthrough
CFLAGS += -std=c89 -pedantic
CFLAGS += -fsanitize=address,undefined -g

PHONY: all
all: main.c box.c test.c io.c opcodes.c
	clang $(CFLAGS) $^ -o chip8
	./chip8 "1-chip8-logo.ch8"
