#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

# define SIZE	4096

typedef struct
{
	uint8_t		mem[SIZE];
	uint8_t		screen[64 * 32];
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
static void	show(uint64_t *screen);

int	main(int argc, char **argv)
{
	Chip	chip;
	int		err;

	if (argc != 2) { printf("Usage: %s <rom.ch8>\n", argv[0]); return (1); }
	printf("I want to display the memory.\n");

	boot(&chip);
	err = load(chip.mem, argv[1]);
	if (err)
		return (1);
	dump(&chip);
	show(chip.screen);
	return (0);
}

static void	show(uint64_t *screen)
{
	(void)screen;
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
	printf("We need to load the code in %s\n", path);

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
	printf("Loaded code into memory. May have truncated.\n");
	return (0);
}
