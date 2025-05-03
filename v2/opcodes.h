#ifndef OPCODES_H
# define OPCODES_H

# include "box.h"

void CLS(Box *box, uint16_t code);
void RET(Box *box, uint16_t code);
void JP(Box *box, uint16_t code);
void CALL(Box *box, uint16_t code);
void SE(Box *box, uint16_t code);
void SNE(Box *box, uint16_t code);
void SEXY(Box *box, uint16_t code);
void LD(Box *box, uint16_t code);
void ADD(Box *box, uint16_t code);
void LDXY(Box *box, uint16_t code);
void OR(Box *box, uint16_t code);
void AND(Box *box, uint16_t code);
void XOR(Box *box, uint16_t code);
void ADDXY(Box *box, uint16_t code);
void SUBXY(Box *box, uint16_t code);
void SHR(Box *box, uint16_t code);
void SUBN(Box *box, uint16_t code);
void SHL(Box *box, uint16_t code);
void SNEXY(Box *box, uint16_t code);
void LDI(Box *box, uint16_t code);
void JP0(Box *box, uint16_t code);
void RND(Box *box, uint16_t code);
void DRW(Box *box, uint16_t code);

#endif /* OPCODES_H */
