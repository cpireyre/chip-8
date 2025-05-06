#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "box.h"
#include "opcodes.h"

void			test(Box *box);
static void		next(Box *box);
static uint16_t	fetch(Box *box);
static int		slurp(Box *box, const char *path);

# define nib(code) ((code & 0xf000) >> 12)
# define last(code) (code & 0x000f)
# define false 0
# define true  1

typedef struct {
	uint16_t	mask;
	uint16_t	pattern;
	opcode		fn;
	uint8_t		next;		
} Instr;

static const Instr table[] = {
	{0xFFFF, 0x00E0, CLS, true},
	{0xFFFF, 0x00EE, RET, true},
	{0xF000, 0x1000, JP, false},
	{0xF000, 0x2000, CALL, false},
	{0xF000, 0x3000, SE, true},
	{0xF000, 0x4000, SNE, true},
	{0xF000, 0x5000, SEXY, true},
	{0xF000, 0x6000, LD, true},
	{0xF000, 0x7000, ADD, true},
	{0xF00F, 0x8000, LDXY, true},
	{0xF00F, 0x8001, OR, true},
	{0xF00F, 0x8002, AND, true},
	{0xF00F, 0x8003, XOR, true},
	{0xF00F, 0x8004, ADDXY, true},
	{0xF00F, 0x8005, SUBXY, true},
	{0xF00F, 0x8006, SHR, true},
	{0xF00F, 0x8007, SUBN, true},
	{0xF00F, 0x800E, SHL, true},
	{0xF000, 0x9000, SNEXY, true},
	{0xF000, 0xA000, LDI, true},
	{0xF000, 0xB000, JP0, false},
	{0xF000, 0xC000, RND, true},
	{0xF000, 0xD000, DRW, true},
	{0xF0FF, 0xF007, DT2VX, true},
	{0xF0FF, 0xF015, VX2DT, true},
	{0xF0FF, 0xF018, VX2ST, true},
	{0xF0FF, 0xF01E, ADDI, true},
	{0xF0FF, 0xF029, VX2I, true}
};

void	run(Box *box)
{
	uint16_t	code;
	uint32_t	i;
	const uint8_t num_ops = sizeof(table) / sizeof(*table);

	while (here(box) < 0x3f1)
	{
		code = fetch(box);
    for (i = 0; i < num_ops; ++i)
        if ((code & table[i].mask) == table[i].pattern)
						break;
		if (i < num_ops)
			table[i].fn(box, code);
		if (i == num_ops || table[i].next == true) next(box);
		show(box);
	}
}

int main(int argc, char **argv)
{
	Box	box;
	int	err;

	if (argc < 2)
	{
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

static uint16_t	fetch(Box *box)
{
	const uint16_t pc = here(box);
	const uint8_t lhs = peek(box, pc);
	const uint8_t rhs = peek(box, pc + 1);
	return (lhs << 8 | rhs);
}

static void		next(Box *box)
{
	const uint16_t pc = here(box);
	jump(box, pc + 2);
}

