#include "box.h"
#include <assert.h>

void	test(Box *box)
{
	boot(box);
	assert(peek(box, 0x123) == 0);
	poke(box, 0x42, 10);
	assert(peek(box, 0x42) == 10);
	dock(box, 3, 56);
	assert(vx(box, 3) == 56);
	keep(box, 0x200);
	assert(look(box) == 0x200);
	poke(box, 0x200, 63);
	assert(peek(box, look(box)) == 63);
	assert(here(box) == 0x200);
	jump(box, 0x420);
	assert(here(box) == 0x420);
	give(box, 0x201);
	give(box, 0x202);
	give(box, 0x203);
	assert(grab(box) == 0x203);
	assert(grab(box) == 0x202);
	assert(grab(box) == 0x201);
}
