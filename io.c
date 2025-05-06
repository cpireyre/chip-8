#include "box.h"
#include <unistd.h>

struct display_string {
	char		*xs;
	uint8_t	sz;
};

void	show(Box *box) {
	struct display_string color[2] = {
		{" ", 1},
		{"█", 3}
	};
	uint8_t		row, col;
	uint8_t		index;
	uint8_t		fill;

	write(1, "\x1b[H", 3);
	for (row = 0; row < 32; row++) {
		for (col = 0; col < 64; col++) {
			index = col >> 3 | row << 3;
			fill = (box->pix[index] >> (col & 7)) & 1;
			write(1, color[fill].xs, color[fill].sz);
		}
		write(1, "\n", 1);
	}
}

