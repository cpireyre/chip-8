#ifndef BOX_H
# define BOX_H

# include <stdint.h>

typedef struct
{
  uint8_t  ram[4096];
  uint8_t  pix[256];
  uint8_t  V[16];
  uint16_t I;
  uint16_t pc;
  uint16_t stack[12];
  uint8_t  top;
  uint8_t  lag;
  uint8_t  hum;
} Box;

void     boot(Box *box);
uint16_t peek(Box *box);
void     poke(Box *box, uint16_t addr, uint8_t byte);
uint16_t here(Box *box);
void     next(Box *box);
void     wipe(Box *box);
uint8_t  draw(Box *box, uint8_t x, uint8_t y, uint16_t addr, uint8_t size);

/* IO */

void    show(Box *box);

#endif /* BOX_H */
