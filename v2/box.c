#include "box.h"
#include <string.h>
#include <stdlib.h>

void boot(Box *b) { bzero(b, sizeof(*b)); b->now = 0x200; }
void poke(Box *b, uint16_t a, uint8_t v) { b->ram[a] = v; }
void dock(Box *b, uint8_t x, uint8_t v) { b->bay[x] = v; }
uint8_t  vx(Box *box, uint8_t x) { return (box->bay[x]); }
uint16_t look(Box *box) { return (box->eye); }
void 		 keep(Box *box, uint16_t addr) { box->eye = addr; }
uint16_t here(Box *box) { return (box->now); }
void 		 jump(Box *box, uint16_t addr) { box->now = addr; }
void 		 give(Box *b, uint16_t a) { b->jar[++b->top] = a; }
uint16_t grab(Box *box) { return (box->jar[box->top--]); }
void		 wipe(Box *b) { bzero(b->pix, sizeof(b->pix)); }
uint8_t	 roll(void) { return ((uint8_t)rand()); }
void		 hold(Box *box, uint8_t lag) { box->lag = lag; }
uint8_t	 ping(Box *box) { return (box->lag); }
void		 sing(Box *box, uint8_t hum) { box->hum = hum; }
uint8_t	 hear(Box *box) { return (box->hum); }
void		 next(Box *box) { jump(box, here(box) + 2); }

uint16_t peek(Box *b) {
	return (b->ram[b->now] << 8 | b->ram[b->now + 1]);
}

uint8_t	draw(Box *box, uint8_t x, uint8_t y,
		uint16_t addr, uint8_t num_sprite_rows) {
	uint8_t	n, tmp, flag, col;
	uint8_t *sprite = box->ram + addr;

	x &= 63;
	y &= 31;
	flag = 0;
	while (num_sprite_rows--) {
		for (n = 0; n < 8; n++) {
			if ((*sprite >> (7 - n)) & 1) {
				col = x + n;
				if (0 <= col && col <= 63 && 0 <= y && y <= 31) {
					tmp = box->pix[col >> 3 | y << 3];
					box->pix[col >> 3 | y << 3] ^= 1 << (col & 7);
					flag |= box->pix[col >> 3 | y << 3] < tmp;
				}
			}
		}
		sprite++;
		y++;
	}
	return (flag);
}
