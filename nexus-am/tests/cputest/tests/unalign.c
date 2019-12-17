#include "trap.h"

volatile unsigned x = 0xffffffff;
volatile unsigned char buf[16];
unsigned char dst[20]={0xff,0xff,0xff,0xff,0xff,0xff,0xff};

int main() {

	for(int i = 0; i < 4; i++) {
		*((volatile unsigned*)(buf + 3)) = 0xaabbccdd;

		x= *(volatile unsigned*)(dst+1) = *((volatile unsigned*)(buf + 3));
		for(int i=0;i<7;i++) printf("%x ",dst[i]);
		printf("\n");
		nemu_assert(x == 0xaabbccdd);

		buf[0] = buf[1] = 0;
	}

	return 0;
}
