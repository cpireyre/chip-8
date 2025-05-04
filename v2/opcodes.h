#ifndef OPCODES_H
# define OPCODES_H

# include "box.h"

void CLS(Box *box, uint16_t code);		/* 0x00E0 */
void RET(Box *box, uint16_t code);		/* 0x00EE */
void JP(Box *box, uint16_t code);			/* 0x1nnn */
void CALL(Box *box, uint16_t code);		/* 0x2nnn */
void SE(Box *box, uint16_t code);			/* 0x3xkk */
void SNE(Box *box, uint16_t code);		/* 0x4xkk */
void SEXY(Box *box, uint16_t code);		/* 0x5xy0 */
void LD(Box *box, uint16_t code);			/* 0x6xkk */
void ADD(Box *box, uint16_t code);		/* 0x7xkk */
void LDXY(Box *box, uint16_t code);		/* 0x8xy0 */
void OR(Box *box, uint16_t code);			/* 0x8xy1 */
void AND(Box *box, uint16_t code);		/* 0x8xy2 */
void XOR(Box *box, uint16_t code);		/* 0x8xy3 */
void ADDXY(Box *box, uint16_t code);	/* 0x8xy4 */
void SUBXY(Box *box, uint16_t code);	/* 0x8xy5 */
void SHR(Box *box, uint16_t code);		/* 0x8xy6 */
void SUBN(Box *box, uint16_t code);		/* 0x8xy7 */
void SHL(Box *box, uint16_t code);		/* 0x8xyE */
void SNEXY(Box *box, uint16_t code);	/* 0x9xy0 */
void LDI(Box *box, uint16_t code);		/* 0xAnnn */
void JP0(Box *box, uint16_t code);		/* 0xBnnn */
void RND(Box *box, uint16_t code);		/* 0xCxkk */
void DRW(Box *box, uint16_t code);		/* 0xDxyn */
void DT2VX(Box *box, uint16_t code);	/* 0xFx07 */
void VX2DT(Box *box, uint16_t code);	/* 0xFx15 */
void VX2ST(Box *box, uint16_t code);	/* 0xFx18 */
void ADDI(Box *box, uint16_t code);		/* 0xFx1E */
void VX2I(Box *box, uint16_t code);		/* 0xFx29 */
#endif /* OPCODES_H */
