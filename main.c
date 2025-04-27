#include <stdio.h>
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

# define CLS 0x00e0
uint16_t swap(uint16_t x) { return (x >> 8 | x << 8); }

static void	boot(Chip *chip);
static int	load(uint8_t *mem, const char *path);
static void dump(Chip *chip);
static void	show(uint8_t *screen);

int	main(int argc, char **argv)
{
	Chip	chip;
	int		err;

	if (argc != 2) { printf("Usage: %s <rom.ch8>\n", argv[0]); return (1); }

	boot(&chip);
	err = load(chip.mem, argv[1]);
	if (err)
		return (1);
	(void)dump;
	/* dump(&chip); */
	show(chip.screen);
	return (0);
}

static void	blink(uint8_t *screen, uint8_t x, uint8_t y)
{
	screen[x >> 3 | y << 3] ^= 1 << (x & 7);
}

static void	show(uint8_t *screen)
{
	uint8_t		row, col;
	uint8_t		index;
	uint32_t	distance;
	uint8_t		centerx, centery;
	uint8_t		radius;
	uint8_t		square;

	centerx = 18;
	centery = 17;
	radius = 4;
	square = radius * radius;
	for (row = 0; row < 32; row++)
	{
		for (col = 0; col < 64; col++)
		{
			distance = (col - centerx) * (col - centerx) + (row - centery) * (row - centery);
			if (0 <= distance - square && distance - square <= 16)
				blink(screen, col, row);
		}
	}
	for (row = 0; row < 32; row++)
	{
		for (col = 0; col < 64; col++)
		{
			index = col >> 3 | row << 3;
			if ((screen[index] >> (col & 7)) & 1)
				write(1, "1", 1);
			else
				write(1, "0", 1);
		}
		write(1, "\n", 1);
	}
}

static void	boot(Chip *chip)
{
	bzero(chip, sizeof(*chip));
	chip->pc = 0x200;
}

static void dump(Chip *chip)
{
    int 					i, j;
    uint16_t 				value;
    static const uint8_t	zeros[16];
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

static int	load(uint8_t *mem, const char *path)
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
