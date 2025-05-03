#include "box.h"
#include <unistd.h>

void	show(Box *box)
{
	uint8_t		row, col;
	uint8_t		index;

	write(1, "\x1b[H", 3);
	for (row = 0; row < 32; row++)
	{
		for (col = 0; col < 64; col++)
		{
			index = col >> 3 | row << 3;
			if ((box->pix[index] >> (col & 7)) & 1)
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

