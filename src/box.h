#ifndef BOX_H
# define BOX_H

typedef unsigned char   u8;
typedef unsigned short  u16;

typedef struct
{
  u8  ram[4096];
  u8  pix[256];
  u8  V[16];
  u16 I;
  u16 pc;
  u16 stack[12];
  u8  top;
  u8  lag;
  u8  hum;
} Box;

void     boot(Box *box);
u16 peek(Box *box);
void     poke(Box *box, u16 addr, u8 byte);
u16 here(Box *box);
void     next(Box *box);
void     wipe(Box *box);
u8  draw(Box *box, u8 x, u8 y, u16 addr, u8 size);

/* IO */

void    show(Box *box);

#endif /* BOX_H */
