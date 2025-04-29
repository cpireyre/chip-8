#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

# define SIZE	4096

typedef struct
{
	uint8_t		mem[SIZE];
	uint8_t		screen[256];
	uint8_t		reg[18];
	uint16_t	I;
	uint16_t	pc;
	uint16_t	stack[16];
	uint8_t		top;
} Chip;

void	boot(Chip *chip);
int	load(uint8_t *mem, const char *path);
void dump(Chip *chip);
void	show(uint8_t *screen);
void	peek(Chip *chip);
uint8_t	draw(uint8_t *screen, uint8_t col, uint8_t row, uint8_t *sprite, uint8_t size);


void	run(Chip *chip)
{
	uint16_t	code;
	uint8_t		reg;
	uint16_t	val;
	uint8_t		size;
	uint8_t		x, y;

	while (chip->pc < 0x3f1)
	{
		/* peek(chip); */
		code = chip->mem[chip->pc] << 8 | chip->mem[chip->pc + 1];
		if (code == 0x00e0) /* CLS */
		{
			bzero(chip->screen, sizeof(chip->screen));
		}
		else if ((code & 0xf000) == 0xa000) /* LDI */
		{
			chip->I = code & 0x0fff;
		}
		else if ((code & 0xf000) == 0x6000) /* LDV */
		{
			val = code & 0x00ff;
			reg = (code & 0x0f00) >> 8;
			chip->reg[reg] = val;
		}
		else if ((code & 0xf000) == 0xd000) /* DRW */
		{
			reg = (code & 0x0f00) >> 8;
			x = chip->reg[reg];
			reg = (code & 0x00f0) >> 4;
			y = chip->reg[reg];
			size = code & 0x000f;
			chip->reg[0xf] = draw(chip->screen, x, y, chip->mem + chip->I, size);
			/* printf("D(%d, %d)\n", x, y); */
		}
		else if ((code & 0xf000) == 0x7000) /* ADD */
		{
			reg = (code & 0x0f00) >> 8;
			val = code & 0x00ff;
			chip->reg[reg] += val;
		}
		else if ((code & 0xf000) == 0x1000) /* JP */
		{
			val = code & 0x0fff;
			chip->pc = val;
			continue;
		}
		else if (code == 0x00ee) /* RET */
		{
			chip->pc = chip->stack[chip->top];
			chip->top--;
		}
		else if ((code & 0xf000) == 0x2000) /* CALL */
		{
			chip->stack[++chip->top] = chip->pc;
			chip->pc = code & 0x0fff;
		}
		else if ((code & 0xf000) == 0x3000) /* SE */
		{
			val = code & 0x00ff;
			reg = (code & 0x0f00) >> 8;
			chip->pc += 2 * (chip->reg[reg] == val);
		}
		else if ((code & 0xf000) == 0x4000) /* SNE */
		{
			val = code & 0x00ff;
			reg = (code & 0x0f00) >> 8;
			chip->pc += 2 * (chip->reg[reg] != val);
		}
		else if ((code & 0xf000) == 0x5000) /* SExy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			chip->pc += 2 * (chip->reg[x] == chip->reg[y]);
		}
		else if ((code & 0xf000) == 0x8000) /* LDxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			chip->reg[x] = chip->reg[y];
		}
		else if ((code & 0xf00f) == 0x8001) /* ORxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			chip->reg[x] |= chip->reg[y];
		}
		else if ((code & 0xf00f) == 0x8002) /* ANDxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			chip->reg[x] &= chip->reg[y];
		}
		else if ((code & 0xf00f) == 0x8003) /* XORxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			chip->reg[x] ^= chip->reg[y];
		}
		else if ((code & 0xf00f) == 0x8004) /* ADDxy */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			y = chip->reg[(code & 0x00f0) >> 4];
			val = x + y;
			chip->reg[0xf] = val < x | val < y;
			chip->reg[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x8005) /* SUBxy */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			y = chip->reg[(code & 0x00f0) >> 4];
			val = x - y;
			chip->reg[0xf] = x > y;
			chip->reg[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x8006) /* SHRx */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			chip->reg[0xf] = x & 1;
			chip->reg[(code & 0x0f00) >> 8] >>= 1;
		}
		else if ((code & 0xf00f) == 0x8007) /* SUBNxy */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			y = chip->reg[(code & 0x00f0) >> 4];
			val = x - y;
			chip->reg[0xf] = y > x;
			chip->reg[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x800E) /* SHLx */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			chip->reg[0xf] = x & 1;
			chip->reg[(code & 0x0f00) >> 8] <<= 1;
		}
		else if ((code & 0xf000) == 0x9000) /* SNExy */
		{
			x = chip->reg[(code & 0x0f00) >> 8];
			y = chip->reg[(code & 0x00f0) >> 4];
			chip->pc += 2 * (x != y);
		}
		else if ((code & 0xf000) == 0xB000) /* JP V0 */
		{
			chip->pc = chip->reg[0] + (code & 0x0fff);
		}
		else if ((code & 0xf000) == 0xC000) /* RND Vx */
		{
			val = rand() & (code & 0x00ff);
			chip->reg[code & 0x0f00] = val;
		}
		else if ((code & 0xf0ff) == 0xE09E) /* SKP Vx */
		{
			/* TODO */
		}
		else if ((code & 0xf0ff) == 0xE0A1) /* SKNP Vx */
		{
			/* TODO */
		}
		else
		{
		}
		chip->pc += 2;
	}
}

int	main(int argc, char **argv)
{
	Chip	chip;
	int		err;

	if (argc != 2) { printf("Usage: %s <rom.ch8>\n", argv[0]); return (1); }

	boot(&chip);
	err = load(chip.mem, argv[1]);
	if (err)
		return (1);
	/* dump(&chip); */
	(void)dump;
	run(&chip);
	return (0);
}

uint8_t blink(uint8_t *screen, uint8_t x, uint8_t y)
{
	const uint8_t	fore = screen[x >> 3 | y << 3];

	if (0 <= x && x <= 63 && 0 <= y && y <= 31)
	{
		screen[x >> 3 | y << 3] ^= 1 << (x & 7);
		return (screen[x >> 3 | y << 3] < fore);
	}
	return (0);
}

uint8_t	draw(uint8_t *screen, uint8_t col, uint8_t row, uint8_t *sprite, uint8_t size)
{
	uint8_t	n;

	col &= 63;
	row &= 31;
	while (size--)
	{
		for (n = 0; n < 8; n++)
			if ((*sprite >> (7 - n)) & 1)
				blink(screen, col + n, row);
		sprite++;
		row++;
	}
	show(screen);
	return (0);
}

void	show(uint8_t *screen)
{
	uint8_t		row, col;
	uint8_t		index;

	printf("\x1b[H");
	for (row = 0; row < 32; row++)
	{
		for (col = 0; col < 64; col++)
		{
			index = col >> 3 | row << 3;
			if ((screen[index] >> (col & 7)) & 1)
			{
				/* write(1, "*", 1); */
				write(1, "█", 3);
			}
			else
				write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
}

void	boot(Chip *chip)
{
	bzero(chip, sizeof(*chip));
	chip->pc = 0x200;
}

void dump(Chip *chip)
{
	int 					i, j;
	uint16_t 				value;
	const uint8_t	zeros[16];
	uint8_t					*mem;

	mem = chip->mem;
	for (i = 0x200; i < 0x400; i += 16)
	{
		if (memcmp(mem + i, zeros, 16) == 0)
			continue;

		printf("%08x: ", i);
		for (j = 0; j < 16; j += 2)
		{
			value = (mem[i + j] << 8) | mem[i + j + 1];
			if (i + j == chip->pc)
				printf("\x1b[0;7m");
			printf("%04x", value);
			if (i + j == chip->pc)
				printf("\x1b[0;27m");
			printf(" ");
		}
		printf("\n");
	}
}

void	peek(Chip *chip)
{
	uint8_t					j;
	uint16_t 				value;
	uint8_t					*mem;
	uint16_t				offset;

	mem = chip->mem;
	offset = chip->pc & 0xfff0;
	printf("%08x: ", offset);
	for (j = 0; j < 16; j += 2)
	{
		value = (mem[offset + j] << 8) | mem[offset + j + 1];
		if (offset + j == chip->pc)
			printf("\x1b[0;7m");
		printf("%04x", value);
		if (offset + j == chip->pc)
			printf("\x1b[0;27m");
		printf(" ");
	}
	printf("\t");

}

int	load(uint8_t *mem, const char *path)
{
	int program_fd;
	int program_data_bytes;

	program_fd = open(path, O_RDONLY);
	if (program_fd < 0)
	{
		perror("open");
		return (1);
	}
	program_data_bytes = read(program_fd, mem + 512, SIZE - 512);
	if (program_data_bytes < 0)
	{
		perror("read");
		close(program_fd);
		return (1);
	}
	close(program_fd);
	return (0);
}
