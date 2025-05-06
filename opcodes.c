#include "box.h"
#include <string.h>
#include <stdlib.h>

# define NNN(code) (code & 0x0fff)
# define KK(code) (code & 0x00ff)
# define X(code) ((code & 0x0f00) >> 0x8)
# define Y(code) ((code & 0x00f0) >> 0x4)
# define N(code) (code & 0x000f)

void CLS(Box *b, uint16_t code) {
  (void)code;
  bzero(b->pix, sizeof(b->pix));
}

void RET(Box *b, uint16_t code) {
  (void)code;
  b->pc = b->stack[b->top--];
}

void JP(Box *b, uint16_t code) {
  b->pc = NNN(code);
}

void CALL(Box *b, uint16_t code) {
  b->stack[++b->top] = b->pc;
  b->pc = NNN(code);
}

void SE(Box *b, uint16_t c) {
  if (b->V[X(c)] == KK(c)) b->pc += 2;
}

void SNE(Box *b, uint16_t c) {
  if (b->V[X(c)] != KK(c)) b->pc += 2;
}

void SEXY(Box *b, uint16_t code) {
  if (b->V[X(code)] == b->V[Y(code)]) b->pc += 2;
}

void LD(Box *b, uint16_t code) {
  b->V[X(code)] = KK(code);
}

void ADD(Box *b, uint16_t code) {
  b->V[X(code)] += KK(code);
}

void LDXY(Box *b, uint16_t code) {
  b->V[X(code)] = b->V[Y(code)];
}

void OR(Box *b, uint16_t code) {
  b->V[X(code)] |= b->V[Y(code)];
}

void AND(Box *b, uint16_t code) {
  b->V[X(code)] &= b->V[Y(code)];
}

void XOR(Box *b, uint16_t code) {
  b->V[X(code)] ^= b->V[Y(code)];
}

void ADDXY(Box *b, uint16_t code) {
  const uint16_t sum = b->V[X(code)] + b->V[Y(code)];
  b->V[0xF] 		     = sum > 0xFF;
  b->V[X(code)]  	   = (uint8_t)sum;
}

void SUBXY(Box *b, uint16_t code) {
  const uint8_t  vx0  = b->V[X(code)];
  const uint8_t  vy0  = b->V[Y(code)];
  const uint16_t diff = vx0 - vy0;

  b->V[0xF] 	  = vx0 >= vy0;
  b->V[X(code)] = (uint8_t)diff;
}

void SHR(Box *b, uint16_t code) {
  b->V[0xF]       = b->V[X(code)] & 1;
  b->V[X(code)] >>= 1;
}

void SUBN(Box *b, uint16_t code) {
  const uint8_t  vx0  = b->V[X(code)];
  const uint8_t  vy0  = b->V[Y(code)];
  const uint16_t diff = (uint16_t)vy0 - (uint16_t)vx0;

  b->V[0xF]     = vy0 >= vx0;
  b->V[X(code)] = (uint8_t)diff;
}

void SHL(Box *b, uint16_t code) {
  b->V[0xF]       = (b->V[X(code)] >> 7) & 1;
  b->V[X(code)] <<= 1;
}

void SNEXY(Box *b, uint16_t code) {
  if (b->V[X(code)] != b->V[Y(code)]) b->pc += 2;
}

void LDI(Box *b, uint16_t code) {
  b->I = NNN(code);
}

void JP0(Box *b, uint16_t code) {
  b->pc = NNN(code) + b->V[0];
}

void RND(Box *b, uint16_t code) {
  b->V[X(code)] = rand() & KK(code);
}

void DRW(Box *b, uint16_t code) {
  const uint8_t col = b->V[X(code)];
  const uint8_t row = b->V[Y(code)];
  const uint8_t collision = draw(b, col, row,
      b->I, N(code));
  b->V[0xF] = collision;
}

/* void SKP(Box *b, uint16_t code) { /1* TODO *1/ } */
/* void SKNP(Box *b, uint16_t code) { /1* TODO *1/ } */
/* void LDK(Box *b, uint16_t code) { /1* TODO *1/ } */
void DT2VX(Box *b, uint16_t code) {
  b->V[X(code)] = b->lag;
}

void VX2DT(Box *b, uint16_t code) {
  b->lag = b->V[X(code)];
}

void VX2ST(Box *b, uint16_t code) {
  b->V[X(code)] = b->hum;
}

void ADDI(Box *b, uint16_t code) {
  b->I += b->V[X(code)];
}

void VX2I(Box *b, uint16_t code) {
  b->I = b->V[X(code)];
}
