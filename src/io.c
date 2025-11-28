#include <unistd.h> /* write */
struct display_string {
	char    *xs;
	u8      sz;
};

void	show(Box *box) {
	struct display_string color[2] = {
		{" ", 1},
		{"█", 3}
	};
	u8		row, col;
	u8		index;
	u8		fill;

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

