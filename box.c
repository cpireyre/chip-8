#include "box.h"
#include <string.h>
#include <stdlib.h>

void boot(Box *b) { bzero(b, sizeof(*b)); b->pc = 0x200; }
void poke(Box *b, uint16_t a, uint8_t v) { b->ram[a] = v; }
uint16_t here(Box *b) { return (b->pc); }
void		 next(Box *b) { b->pc += 2; }

uint16_t peek(Box *b) {
	return (b->ram[b->pc] << 8 | b->ram[b->pc + 1]);
}

uint8_t	draw(Box *b, uint8_t x, uint8_t y,
		uint16_t addr, uint8_t num_sprite_rows) {
	uint8_t	n, tmp, flag, col;
	uint8_t *sprite = b->ram + addr;

	x &= 63;
	y &= 31;
	flag = 0;
	while (num_sprite_rows--) {
		for (n = 0; n < 8; n++) {
			if ((*sprite >> (7 - n)) & 1) {
				col = x + n;
				if (0 <= col && col <= 63 && 0 <= y && y <= 31) {
					tmp = b->pix[col >> 3 | y << 3];
					b->pix[col >> 3 | y << 3] ^= 1 << (col & 7);
					flag |= b->pix[col >> 3 | y << 3] < tmp;
				}
			}
		}
		sprite++;
		y++;
	}
	return (flag);
}
