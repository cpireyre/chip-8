#include "box.h"

# define nnn(code) (code & 0x0fff)
# define kk(code) (code & 0x00ff)
# define x(code) ((code & 0x0f00) >> 0x8)
# define y(code) ((code & 0x00f0) >> 0x4)
# define n(code) (code & 0x000f)

void CLS(Box *box, uint16_t code) { (void)code; wipe(box); }

void RET(Box *box, uint16_t code) {
	(void) code;
	jump(box, grab(box));
}

void JP(Box *box, uint16_t code) { jump(box, nnn(code)); }

void CALL(Box *box, uint16_t code) {
	give(box, here(box));
	jump(box, nnn(code));
}

void SE(Box *box, uint16_t code) {
	if (vx(box, x(code)) == kk(code))
		jump(box, here(box) + 2);
}

void SNE(Box *box, uint16_t code) {
	if (vx(box, x(code)) != kk(code))
		jump(box, here(box) + 2);
}

void SEXY(Box *box, uint16_t code) {
	if (vx(box, x(code)) == vx(box, y(code)))
		jump(box, here(box) + 2);
}

void LD(Box *box, uint16_t code) {
	dock(box, x(code), kk(code));
}

void ADD(Box *box, uint16_t code) {
	const uint8_t sum = vx(box, x(code)) + kk(code);
	dock(box, x(code), sum);
}

void LDXY(Box *box, uint16_t code) {
	const uint8_t val = vx(box, y(code));
	dock(box, x(code), val);
}

void OR(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code)) | vx(box, y(code));
	dock(box, x(code), val);
}

void AND(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code)) & vx(box, y(code));
	dock(box, x(code), val);
}

void XOR(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code)) ^ vx(box, y(code));
	dock(box, x(code), val);
}

void ADDXY(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code)) + vx(box, y(code));
	const uint8_t carry = val < x(code) | val < y(code);
	dock(box, 0xF, carry);
	dock(box, x(code), val);
}

void SUBXY(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code)) - vx(box, y(code));
	const uint8_t borrow = vx(box, x(code)) <= vx(box, y(code));
	dock(box, 0xF, !borrow);
	dock(box, x(code), val);
}

void SHR(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code));
	dock(box, 0xF, val & 1);
	dock(box, x(code), val >> 1);
}

void SUBN(Box *box, uint16_t code) {
	const uint8_t val = vx(box, y(code)) - vx(box, x(code));
	const uint8_t borrow = vx(box, y(code)) <= vx(box, x(code));
	dock(box, 0xF, !borrow);
	dock(box, x(code), val);
}

void SHL(Box *box, uint16_t code) {
	const uint8_t val = vx(box, x(code));
	dock(box, 0xF, (val >> 7) & 1);
	dock(box, x(code), val << 1);
}

void SNEXY(Box *box, uint16_t code) {
	if (vx(box, x(code)) != vx(box, y(code)))
		jump(box, here(box) + 2);
}

void LDI(Box *box, uint16_t code) {
	keep(box, nnn(code));
}

void JP0(Box *box, uint16_t code) {
	const uint16_t addr = nnn(code) + vx(box, 0x0);
	jump(box, addr);
}

void RND(Box *box, uint16_t code) {
	const uint8_t val = roll() & kk(code);
	dock(box, x(code), val);
}

void DRW(Box *box, uint16_t code) {
	const uint8_t col = vx(box, x(code));
	const uint8_t row = vx(box, y(code));
	const uint8_t collision = draw(box, col, row,
			look(box), n(code));
	dock(box, 0xF, collision);
}
