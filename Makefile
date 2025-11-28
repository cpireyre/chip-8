CFLAGS := -Wall -Wextra -Werror
CFLAGS += -std=c89 -pedantic
CFLAGS += -fsanitize=address,undefined -g
CPPFLAGS := -Iinclude/

bin := chip8
PHONY: all
all: src/main.c
	cc $(CPPFLAGS) $(CFLAGS) $^ -o $(bin)

clean:
	$(RM) $(bin)
