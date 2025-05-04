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

void	run(Box *box)
{
	uint16_t	code;

	while (here(box) < 0x3f1)
	{
		code = fetch(box);
		switch (code) {
			case 0x00E0: CLS(box, code); break;
			case 0x00EE: RET(box, code); break;
		}
		switch (nib(code)) {
			case 0xA: LDI(box, code); break;
			case 0x2: CALL(box, code); continue;
			case 0x3: SE(box, code); break;
			case 0x4: SNE(box, code); break;
			case 0x5: SEXY(box, code); break;
			case 0x6: LD(box, code); break;
			case 0x7: ADD(box, code); break;
			case 0x8: {
						  switch (last(code)) {
							  case 0x0: LDXY(box, code); break;
							  case 0x1: OR(box, code); break;
							  case 0x2: AND(box, code); break;
							  case 0x3: XOR(box, code); break;
							  case 0x4: ADDXY(box, code); break;
							  case 0x5: SUBXY(box, code); break;
							  case 0x6: SHR(box, code); break;
							  case 0x7: SUBN(box, code); break;
							  case 0xE: SHL(box, code); break;
						  }
					  }
								break;
			case 0x1: JP(box, code); continue;
			case 0xD: DRW(box, code); show(box); break;
			case 0x9: SNEXY(box, code); break;
			case 0xB: JP0(box, code); break;
			case 0xC: RND(box, code); break;
		}
		next(box);
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

