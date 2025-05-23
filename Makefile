CFLAGS := -Wall -Wextra -Werror
CFLAGS += -std=c89 -pedantic
CFLAGS += -fsanitize=address,undefined -g

PHONY: all
all: main.c box.c io.c opcodes.c
	cc $(CFLAGS) $^ -o chip8
