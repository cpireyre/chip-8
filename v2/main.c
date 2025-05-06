#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "box.h"
#include "decode.h"

static int			slurp(Box *box, const char *path);

void	run(Box *box) {
	const uint8_t num_ops = sizeof(ops) / sizeof(*ops);
	uint16_t			code;
	uint8_t				i;

	while (here(box) < 0x3f1) {
		code = peek(box);
    for (i = 0; i < num_ops; ++i)
        if ((code & ops[i].mask) == ops[i].pattern) break;
		if (i < num_ops) ops[i].fn(box, code);
		if (i == num_ops || ops[i].next == true)
			jump(box, here(box) + 2);
		show(box);
	}
}

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

static int	slurp(Box *box, const char *path)
{
	int			fd;
	int			ret;
	uint8_t		byte;
	uint16_t	total;

	fd = open(path, O_RDONLY);
	if (fd < 0) { perror("open"); return (1); }

	total = 0;
	while ((ret = read(fd, &byte, 1)) > 0)
		poke(box, 0x200 + total++, byte);
	if (ret < 0) { perror("read"); return (1); }
	if (total <= 0) printf("Empty program?\n"); 
	else printf("Loaded ROM successfully.\n"); 
	return (0);
}
