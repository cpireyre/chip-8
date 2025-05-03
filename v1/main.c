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
	uint8_t		ram[SIZE];
	uint8_t		pix[256];
	uint8_t		bay[18];
	uint16_t	eye;
	uint16_t	now;
	uint16_t	jar[16];
	uint8_t		top;
} Box;

void	boot(Box *box);
int	load(uint8_t *ram, const char *path);
void dump(Box *box);
void	show(uint8_t *pix);
void	peek(Box *box);
uint8_t	draw(uint8_t *pix, uint8_t col, uint8_t row, uint8_t *sprite, uint8_t size);


void	run(Box *box)
{
	uint16_t	code;
	uint8_t		bay;
	uint16_t	val;
	uint8_t		size;
	uint8_t		x, y;

	while (box->now < 0x3f1)
	{
		/* peek(box); */
		code = box->ram[box->now] << 8 | box->ram[box->now + 1];
		if (code == 0x00e0) /* CLS */
		{
			bzero(box->pix, sizeof(box->pix));
		}
		else if ((code & 0xf000) == 0xa000) /* LDeye */
		{
			box->eye = code & 0x0fff;
		}
		else if ((code & 0xf000) == 0x6000) /* LDV */
		{
			val = code & 0x00ff;
			bay = (code & 0x0f00) >> 8;
			box->bay[bay] = val;
		}
		else if ((code & 0xf000) == 0xd000) /* DRW */
		{
			bay = (code & 0x0f00) >> 8;
			x = box->bay[bay];
			bay = (code & 0x00f0) >> 4;
			y = box->bay[bay];
			size = code & 0x000f;
			box->bay[0xf] = draw(box->pix, x, y, box->ram + box->eye, size);
			/* printf("D(%d, %d)\n", x, y); */
		}
		else if ((code & 0xf000) == 0x7000) /* ADD */
		{
			bay = (code & 0x0f00) >> 8;
			val = code & 0x00ff;
			box->bay[bay] += val;
		}
		else if ((code & 0xf000) == 0x1000) /* JP */
		{
			val = code & 0x0fff;
			box->now = val;
			continue;
		}
		else if (code == 0x00ee) /* RET */
		{
			box->now = box->jar[box->top];
			box->top--;
		}
		else if ((code & 0xf000) == 0x2000) /* CALL */
		{
			box->jar[++box->top] = box->now;
			box->now = code & 0x0fff;
		}
		else if ((code & 0xf000) == 0x3000) /* SE */
		{
			val = code & 0x00ff;
			bay = (code & 0x0f00) >> 8;
			box->now += 2 * (box->bay[bay] == val);
		}
		else if ((code & 0xf000) == 0x4000) /* SNE */
		{
			val = code & 0x00ff;
			bay = (code & 0x0f00) >> 8;
			box->now += 2 * (box->bay[bay] != val);
		}
		else if ((code & 0xf000) == 0x5000) /* SExy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			box->now += 2 * (box->bay[x] == box->bay[y]);
		}
		else if ((code & 0xf000) == 0x8000) /* LDxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			box->bay[x] = box->bay[y];
		}
		else if ((code & 0xf00f) == 0x8001) /* ORxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			box->bay[x] |= box->bay[y];
		}
		else if ((code & 0xf00f) == 0x8002) /* ANDxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			box->bay[x] &= box->bay[y];
		}
		else if ((code & 0xf00f) == 0x8003) /* XORxy */
		{
			x = (code & 0x0f00) >> 8;
			y = (code & 0x00f0) >> 4;
			box->bay[x] ^= box->bay[y];
		}
		else if ((code & 0xf00f) == 0x8004) /* ADDxy */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			y = box->bay[(code & 0x00f0) >> 4];
			val = x + y;
			box->bay[0xf] = val < x | val < y;
			box->bay[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x8005) /* SUBxy */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			y = box->bay[(code & 0x00f0) >> 4];
			val = x - y;
			box->bay[0xf] = x > y;
			box->bay[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x8006) /* SHRx */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			box->bay[0xf] = x & 1;
			box->bay[(code & 0x0f00) >> 8] >>= 1;
		}
		else if ((code & 0xf00f) == 0x8007) /* SUBNxy */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			y = box->bay[(code & 0x00f0) >> 4];
			val = x - y;
			box->bay[0xf] = y > x;
			box->bay[(code & 0x0f00) >> 8] = val;
		}
		else if ((code & 0xf00f) == 0x800E) /* SHLx */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			box->bay[0xf] = x & 1;
			box->bay[(code & 0x0f00) >> 8] <<= 1;
		}
		else if ((code & 0xf000) == 0x9000) /* SNExy */
		{
			x = box->bay[(code & 0x0f00) >> 8];
			y = box->bay[(code & 0x00f0) >> 4];
			box->now += 2 * (x != y);
		}
		else if ((code & 0xf000) == 0xB000) /* JP V0 */
		{
			box->now = box->bay[0] + (code & 0x0fff);
		}
		else if ((code & 0xf000) == 0xC000) /* RND Vx */
		{
			val = rand() & (code & 0x00ff);
			box->bay[code & 0x0f00] = val;
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
		box->now += 2;
	}
}

int	main(int argc, char **argv)
{
	Box	box;
	int		err;

	if (argc != 2) { printf("Usage: %s <rom.ch8>\n", argv[0]); return (1); }

	boot(&box);
	err = load(box.ram, argv[1]);
	if (err)
		return (1);
	/* dump(&box); */
	(void)dump;
	run(&box);
	return (0);
}

uint8_t blink(uint8_t *pix, uint8_t x, uint8_t y)
{
	const uint8_t	fore = pix[x >> 3 | y << 3];

	if (0 <= x && x <= 63 && 0 <= y && y <= 31)
	{
		pix[x >> 3 | y << 3] ^= 1 << (x & 7);
		return (pix[x >> 3 | y << 3] < fore);
	}
	return (0);
}

uint8_t	draw(uint8_t *pix, uint8_t col, uint8_t row, uint8_t *sprite, uint8_t size)
{
	uint8_t	n;

	col &= 63;
	row &= 31;
	while (size--)
	{
		for (n = 0; n < 8; n++)
			if ((*sprite >> (7 - n)) & 1)
				blink(pix, col + n, row);
		sprite++;
		row++;
	}
	show(pix);
	return (0);
}

void	show(uint8_t *pix)
{
	uint8_t		row, col;
	uint8_t		index;

	printf("\x1b[H");
	for (row = 0; row < 32; row++)
	{
		for (col = 0; col < 64; col++)
		{
			index = col >> 3 | row << 3;
			if ((pix[index] >> (col & 7)) & 1)
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

void	boot(Box *box)
{
	bzero(box, sizeof(*box));
	box->now = 0x200;
}

void dump(Box *box)
{
	int 					i, j;
	uint16_t 				value;
	const uint8_t	zeros[16];
	uint8_t					*ram;

	ram = box->ram;
	for (i = 0x200; i < 0x400; i += 16)
	{
		if (memcmp(ram + i, zeros, 16) == 0)
			continue;

		printf("%08x: ", i);
		for (j = 0; j < 16; j += 2)
		{
			value = (ram[i + j] << 8) | ram[i + j + 1];
			if (i + j == box->now)
				printf("\x1b[0;7m");
			printf("%04x", value);
			if (i + j == box->now)
				printf("\x1b[0;27m");
			printf(" ");
		}
		printf("\n");
	}
}

void	peek(Box *box)
{
	uint8_t					j;
	uint16_t 				value;
	uint8_t					*ram;
	uint16_t				offset;

	ram = box->ram;
	offset = box->now & 0xfff0;
	printf("%08x: ", offset);
	for (j = 0; j < 16; j += 2)
	{
		value = (ram[offset + j] << 8) | ram[offset + j + 1];
		if (offset + j == box->now)
			printf("\x1b[0;7m");
		printf("%04x", value);
		if (offset + j == box->now)
			printf("\x1b[0;27m");
		printf(" ");
	}
	printf("\t");

}

int	load(uint8_t *ram, const char *path)
{
	int program_fd;
	int program_data_bytes;

	program_fd = open(path, O_RDONLY);
	if (program_fd < 0)
	{
		perror("open");
		return (1);
	}
	program_data_bytes = read(program_fd, ram + 512, SIZE - 512);
	if (program_data_bytes < 0)
	{
		perror("read");
		close(program_fd);
		return (1);
	}
	close(program_fd);
	return (0);
}
