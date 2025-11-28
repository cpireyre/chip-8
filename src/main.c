#include <fcntl.h> /* open */
#include <stdio.h> /* printf */
#include "box.h"
#include "decode.h"
#include "opcodes.h"
#include "box.c"
#include "opcodes.c"
#include "io.c"

static int	slurp(Box *box, const char *path);
static void	run(Box *box);

int main(int argc, char **argv)
{
	Box	box;
	int	err;

	if (argc < 2) {
		printf("Usage: %s <rom.ch8>\n", *argv);
		return (1);
	}
	boot(&box);
	err = slurp(&box, argv[1]);
	if (err != 0) return (1);
	run(&box);
}

static void	run(Box *box) {
	const u8 n_ops = sizeof(ops) / sizeof(*ops);
	u16			code;
	u8				i;

	while (here(box) < 0x3f1) {
		code = peek(box);
		i = 0;
		while(i < n_ops && (code & ops[i].mask) != ops[i].bits)
			i++;
		if (i < n_ops) ops[i].fn(box, code);
		if (i == n_ops || ops[i].advance == true) next(box);
		show(box);
	}
}

static int	slurp(Box *box, const char *path)
{
	u8				 byte;
	u16			 total;
	int 					 fd, ret;
	const u16 ram_cap = 4096 - 0x200;

	fd = open(path, O_RDONLY);
	if (fd < 0) { perror("open"); return (1); }

	total = 0;
	while (total < ram_cap && (ret = read(fd, &byte, 1)) > 0)
		poke(box, 0x200 + total++, byte);

	if (ret < 0) { perror("read"); return (1); }
	if (total == 0) { printf("Empty ROM\n"); return (1); }
	return (0);
}
