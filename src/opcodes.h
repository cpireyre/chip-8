#ifndef OPCODES_H
# define OPCODES_H

# include "box.h"

typedef void (*opcode)(Box *, u16);

void CLS  (Box *box, u16 code);
void RET  (Box *box, u16 code);
void JP   (Box *box, u16 code);
void CALL (Box *box, u16 code);
void SE   (Box *box, u16 code);
void SNE  (Box *box, u16 code);
void SEXY (Box *box, u16 code);
void LD   (Box *box, u16 code);
void ADD  (Box *box, u16 code);
void LDXY (Box *box, u16 code);
void OR   (Box *box, u16 code);
void AND  (Box *box, u16 code);
void XOR  (Box *box, u16 code);
void ADDXY(Box *box, u16 code);
void SUBXY(Box *box, u16 code);
void SHR  (Box *box, u16 code);
void SUBN (Box *box, u16 code);
void SHL  (Box *box, u16 code);
void SNEXY(Box *box, u16 code);
void LDI  (Box *box, u16 code);
void JP0  (Box *box, u16 code);
void RND  (Box *box, u16 code);
void DRW  (Box *box, u16 code);
void DT2VX(Box *box, u16 code);
void VX2DT(Box *box, u16 code);
void VX2ST(Box *box, u16 code);
void ADDI (Box *box, u16 code);
void VX2I (Box *box, u16 code);

#endif /* OPCODES_H */
