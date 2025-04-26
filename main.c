#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

# define RAM_SIZE_BYTES	4096
# define CLS 0x00e0

static void	show(uint8_t *mem);
static int	load(uint8_t *mem, const char *path);

int	main(int argc, char **argv)
{
	uint8_t	mem[RAM_SIZE_BYTES];
	int		err;

	if (argc != 2) { printf("Usage: %s <rom.ch8>\n", argv[0]); return (1); }
	printf("I want to display the memory.\n");

	err = load(mem, argv[1]);
	if (err)
		return (1);
	show(mem);
	return (0);
}

uint16_t	swap(uint16_t x) { return (x >> 8 | x << 8); }

static void show(uint8_t *mem)
{
    int 					i, j;
    uint16_t 				value;
    static const uint8_t	zeros[16];

    for (i = 0x200; i < 0x400; i += 16)
    {
        if (memcmp(mem + i, zeros, 16) == 0)
            continue;

        printf("%08x: ", i);
        for (j = 0; j < 16; j += 2)
        {
            value = (mem[i + j] << 8) | mem[i + j + 1];
			if (value == CLS)
				printf("\x1b[0;32m");
			printf("%04x ", value);
			printf("\x1b[0m");
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
	bzero(mem, 4096);
	program_data_bytes = read(program_fd, mem + 512, RAM_SIZE_BYTES - 512);
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
