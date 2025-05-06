#ifndef BOX_H
# define BOX_H

# include <stdint.h>

/* TODO: Forward declare to make implementation opaque? */
typedef struct
{
	uint8_t		ram[4096];
	uint8_t		pix[256];
	uint8_t		bay[16];
	uint16_t	eye;
	uint16_t	now;
	uint16_t	jar[12];
	uint8_t		top;
	uint8_t		lag;
	uint8_t		hum;
} Box;

typedef void (*opcode)(Box *, uint16_t);

void		boot(Box *box);
uint16_t peek(Box *box);
void		poke(Box *box, uint16_t addr, uint8_t byte);
void		dock(Box *box, uint8_t x, uint8_t val);
uint8_t		vx(Box *box, uint8_t x);
uint16_t	look(Box *box);
void		keep(Box *box, uint16_t addr);
uint16_t 	here(Box *box);
void 		jump(Box *box, uint16_t addr);
void 		give(Box *box, uint16_t addr);
uint16_t 	grab(Box *box);
void		wipe(Box *box);
uint8_t		draw(Box *box, uint8_t x, uint8_t y,
				uint16_t addr, uint8_t size);
uint8_t		roll(void);
void		hold(Box *box, uint8_t lag);
uint8_t		ping(Box *box);
void		sing(Box *box, uint8_t hum);
uint8_t		hear(Box *box);

/* IO */

void	show(Box *box);

#endif /* BOX_H */
