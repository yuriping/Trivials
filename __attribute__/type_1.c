#include <stdio.h>

int main(void)
{
	unsigned short idx;
	unsigned char * pch;

	typedef struct SUB_T
	{
		char sc;
		int ss;
	} /*__attribute__((packed))*/ SUB_T;

	typedef struct UNPACKED_T
	{
		char s1;
		char ch1;
		SUB_T st;
		short s2;
	} UNPACKED_T;
	UNPACKED_T up = {0x12, 0x56, {0x33, 0x44556677}, 0x789a};
	printf("up = {%02x, %02x, {%02x, %08x}, %04x}\n",
			up.s1, up.ch1, up.st.sc, up.st.ss, up.s2);
	for (pch = (unsigned char *)&up, idx = 0; idx < sizeof(UNPACKED_T); idx ++)
		printf("%02x-", pch[idx]);
	printf("\n");

    return 0;
}

