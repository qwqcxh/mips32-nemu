#include "trap.h"

volatile unsigned x = 0xffffffff;
volatile unsigned char buf[16];

int main() {

	for(int i = 0; i < 4; i++) {
		*((volatile unsigned*)(buf + 1)) = 0xaabbccdd;
		*((volatile unsigned*)(buf + 3)) = 0x66778899;

		x = *((volatile unsigned*)(buf + 1));
		nemu_assert(x == 0x8899ccdd);
		x = *((volatile unsigned*)(buf + 3));
		nemu_assert(x == 0x66778899);
		buf[0] = buf[1] = 0;
	}

	return 0;
}
