// #include "trap.h"

// char *s[] = {
// 	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 
// 	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
// 	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
// 	", World!\n",
// 	"Hello, World!\n",
// 	"#####"
// };

// char str1[] = "Hello";
// char str[20];

// int main() {
// 	nemu_assert(strcmp(s[0], s[2]) == 0);
// 	nemu_assert(strcmp(s[0], s[1]) == -1);
// 	nemu_assert(strcmp(s[0] + 1, s[1] + 1) == -1);
// 	nemu_assert(strcmp(s[0] + 2, s[1] + 2) == -1);
// 	nemu_assert(strcmp(s[0] + 3, s[1] + 3) == -1);

// 	nemu_assert(strcmp( strcat(strcpy(str, str1), s[3]), s[4]) == 0);

// 	nemu_assert(memcmp(memset(str, '#', 5), s[5], 5) == 0);

// 	return 0;
// }

#include "trap.h"

char *s[] = {
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
	", World!\n",
	"Hello, World!\n",
	"#####"
};

char str1[] = "Hello";
char str[20];

char cpy1[] ={0xaa,0xb6,0x69,0xab,0xd7,0xa4,0xad,0x20,0x20,0x20,0x20,0xaa,0xb6,0x69,0xab,0xd7,0xa4,0xad,0x20,0x20,0x20,0x20};
char cpy2[100];
int main() {
	// nemu_assert(strcmp(s[0], s[2]) == 0);
	// nemu_assert(strcmp(s[0], s[1]) == -1);
	// nemu_assert(strcmp(s[0] + 1, s[1] + 1) == -1);
	// nemu_assert(strcmp(s[0] + 2, s[1] + 2) == -1);
	// nemu_assert(strcmp(s[0] + 3, s[1] + 3) == -1);

	// nemu_assert(strcmp( strcat(strcpy(str, str1), s[3]), s[4]) == 0);

	// nemu_assert(memcmp(memset(str, '#', 5), s[5], 5) == 0);
	memcpy(cpy2+1,((char*)cpy1)+3,10);
	for(int i=0;i<10;i++) printf("%x ",((char*)cpy2)[i+1]);
	return 0;
}
