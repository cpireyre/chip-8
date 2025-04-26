CFLAGS := -Wall -Wextra -Werror -pedantic
CFLAGS += -fsanitize=address,undefined
CFLAGS += --std=c89


.PHONY: all
all: main.c
	cc $(CFLAGS) $^ -o chip8
	./chip8 "IBM Logo.ch8"
