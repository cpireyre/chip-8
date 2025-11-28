#include <stdlib.h> /* rand */
# define NNN(code) (code & 0x0fff)
# define KK(code) (code & 0x00ff)
# define X(code) ((code & 0x0f00) >> 0x8)
# define Y(code) ((code & 0x00f0) >> 0x4)
# define N(code) (code & 0x000f)

void CLS(Box *b, u16 code) {
  (void)code;
  bzero(b->pix, sizeof(b->pix));
}

void RET(Box *b, u16 code) {
  (void)code;
  b->pc = b->stack[b->top--];
}

void JP(Box *b, u16 code) {
  b->pc = NNN(code);
}

void CALL(Box *b, u16 code) {
  b->stack[++b->top] = b->pc;
  b->pc = NNN(code);
}

void SE(Box *b, u16 c) {
  if (b->V[X(c)] == KK(c)) b->pc += 2;
}

void SNE(Box *b, u16 c) {
  if (b->V[X(c)] != KK(c)) b->pc += 2;
}

void SEXY(Box *b, u16 code) {
  if (b->V[X(code)] == b->V[Y(code)]) b->pc += 2;
}

void LD(Box *b, u16 code) {
  b->V[X(code)] = KK(code);
}

void ADD(Box *b, u16 code) {
  b->V[X(code)] += KK(code);
}

void LDXY(Box *b, u16 code) {
  b->V[X(code)] = b->V[Y(code)];
}

void OR(Box *b, u16 code) {
  b->V[X(code)] |= b->V[Y(code)];
}

void AND(Box *b, u16 code) {
  b->V[X(code)] &= b->V[Y(code)];
}

void XOR(Box *b, u16 code) {
  b->V[X(code)] ^= b->V[Y(code)];
}

void ADDXY(Box *b, u16 code) {
  const u16 sum = b->V[X(code)] + b->V[Y(code)];
  b->V[0xF] 		     = sum > 0xFF;
  b->V[X(code)]  	   = (u8)sum;
}

void SUBXY(Box *b, u16 code) {
  const u8  vx0  = b->V[X(code)];
  const u8  vy0  = b->V[Y(code)];
  const u16 diff = vx0 - vy0;

  b->V[0xF] 	  = vx0 >= vy0;
  b->V[X(code)] = (u8)diff;
}

void SHR(Box *b, u16 code) {
  b->V[0xF]       = b->V[X(code)] & 1;
  b->V[X(code)] >>= 1;
}

void SUBN(Box *b, u16 code) {
  const u8  vx0  = b->V[X(code)];
  const u8  vy0  = b->V[Y(code)];
  const u16 diff = (u16)vy0 - (u16)vx0;

  b->V[0xF]     = vy0 >= vx0;
  b->V[X(code)] = (u8)diff;
}

void SHL(Box *b, u16 code) {
  b->V[0xF]       = (b->V[X(code)] >> 7) & 1;
  b->V[X(code)] <<= 1;
}

void SNEXY(Box *b, u16 code) {
  if (b->V[X(code)] != b->V[Y(code)]) b->pc += 2;
}

void LDI(Box *b, u16 code) {
  b->I = NNN(code);
}

void JP0(Box *b, u16 code) {
  b->pc = NNN(code) + b->V[0];
}

void RND(Box *b, u16 code) {
  b->V[X(code)] = rand() & KK(code);
}

void DRW(Box *b, u16 code) {
  const u8 col = b->V[X(code)];
  const u8 row = b->V[Y(code)];
  const u8 collision = draw(b, col, row,
      b->I, N(code));
  b->V[0xF] = collision;
}

/* void SKP(Box *b, u16 code) { /1* TODO *1/ } */
/* void SKNP(Box *b, u16 code) { /1* TODO *1/ } */
/* void LDK(Box *b, u16 code) { /1* TODO *1/ } */
void DT2VX(Box *b, u16 code) {
  b->V[X(code)] = b->lag;
}

void VX2DT(Box *b, u16 code) {
  b->lag = b->V[X(code)];
}

void VX2ST(Box *b, u16 code) {
  b->V[X(code)] = b->hum;
}

void ADDI(Box *b, u16 code) {
  b->I += b->V[X(code)];
}

void VX2I(Box *b, u16 code) {
  b->I = b->V[X(code)];
}
