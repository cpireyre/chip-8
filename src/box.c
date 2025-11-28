void    boot(Box *b)              { b->pc = 0x200; }
void    poke(Box *b, u16 a, u8 v) { b->ram[a] = v; }
u16     here(Box *b)              { return (b->pc); }
void    next(Box *b)              { b->pc += 2; }

u16 peek(Box *b) {
	return (b->ram[b->pc] << 8 | b->ram[b->pc + 1]);
}

u8	draw(Box *b, u8 x, u8 y,
		u16 addr, u8 num_sprite_rows) {
	u8	n, tmp, flag, col;
	u8 *sprite = b->ram + addr;

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
