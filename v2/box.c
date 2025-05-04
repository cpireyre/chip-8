#include "box.h"
#include <string.h>
#include <stdlib.h>

void	boot(Box *box) {
	bzero(box, sizeof(*box));
	box->now = 0x200;
}

uint8_t peek(Box *box, uint16_t addr) {
	return (box->ram[addr]);
}

void poke(Box *box, uint16_t addr, uint8_t byte) {
	box->ram[addr] = byte;
}

void dock(Box *box, uint8_t x, uint8_t val) {
	box->bay[x] = val;
}

uint8_t vx(Box *box, uint8_t x) {
	return (box->bay[x]);
}

uint16_t look(Box *box) {
	return (box->eye);
}

void keep(Box *box, uint16_t addr) {
	box->eye = addr;
}

uint16_t here(Box *box) {
	return (box->now);
}

void jump(Box *box, uint16_t addr) {
	box->now = addr;
}

void give(Box *box, uint16_t addr) {
	box->top += 1;
	box->jar[box->top] = addr;
}

uint16_t grab(Box *box) {
	const uint16_t val = box->jar[box->top];
	box->top -= 1;
	return (val);
}

uint8_t	draw(Box *box, uint8_t x, uint8_t y,
		uint16_t addr, uint8_t size)
{
	uint8_t	n;
	uint8_t	tmp;
	uint8_t	flag;
	uint8_t	col;
	uint8_t *sprite = box->ram + addr;

	x &= 63;
	y &= 31;
	flag = 0;
	while (size--)
	{
		for (n = 0; n < 8; n++)
		{
			if ((*sprite >> (7 - n)) & 1)
			{
				col = x + n;
				if (0 <= col && col <= 63 && 0 <= y && y <= 31)
				{
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

void	wipe(Box *box)
{
	bzero(box->pix, sizeof(box->pix));
}

uint8_t	roll(void) { return ((uint8_t)rand()); }

void	hold(Box *box, uint8_t lag) { box->lag = lag; }
uint8_t	ping(Box *box) { return (box->lag); }
void	sing(Box *box, uint8_t hum) { box->hum = hum; }
uint8_t	hear(Box *box) { return (box->hum); }
