#include <stdio.h>

int main(void)
{
	unsigned short idx;
	unsigned char * pch;

	typedef struct SUB_T
	{
		char sc;
		int ss;
	} SUB_T;

	typedef struct UNPACKED_T
	{
		int  i1;
		short s1;
		char ch1;
		char sa[3];
		SUB_T st;
		int  i2;
	} UNPACKED_T;
	UNPACKED_T up = {0x12345678, 0x9abc, 0xa9, {0x33, 0x44, 0x55}, {0x21, 0x78908866}, 0xbcdef012};
	printf("al = {%08x, %04x, %02x, {%02x, %02x, %02x}, {%02x, %08x},  %08x}\n",
			up.i1, up.s1, up.ch1, up.sa[0], up.sa[1], up.sa[2], up.st.sc, up.st.ss , up.i2);
	for (pch = (unsigned char *)&up, idx = 0; idx < sizeof(UNPACKED_T); idx ++)
		printf("%02x-", pch[idx]);
	printf("\n");

	typedef struct ALIGNED_T
	{
		int  i1;
		short s1;
		char ch1;
		char sa[3];
		SUB_T st;
		int  i2;
	} __attribute__((aligned(32))) ALIGNED_T;
	ALIGNED_T al = {0x12345678, 0x9abc, 0xa9, {0x33, 0x44, 0x55}, {0x21, 0x78908866}, 0xbcdef012};
	printf("al = {%08x, %04x, %02x, {%02x, %02x, %02x}, {%02x, %08x},  %08x}\n",
			al.i1, al.s1, al.ch1, al.sa[0], al.sa[1], al.sa[2], al.st.sc, al.st.ss , al.i2);
	for (pch = (unsigned char *)&al, idx = 0; idx < sizeof(ALIGNED_T); idx ++)
		printf("%02x-", pch[idx]);
	printf("\n");

	typedef struct PACKED_T
	{
		int  i1;
		short s1;
		char ch1;
		char sa[3];
		SUB_T st;
		int  i2;
	} __attribute__((packed)) PACKED_T;
	PACKED_T pa = {0x12345678, 0x9abc, 0xa9, {0x33, 0x44, 0x55}, {0x21, 0x78908866}, 0xbcdef012};
	printf("al = {%08x, %04x, %02x, {%02x, %02x, %02x}, {%02x, %08x},  %08x}\n",
			pa.i1, pa.s1, pa.ch1, pa.sa[0], pa.sa[1], pa.sa[2], pa.st.sc, pa.st.ss , pa.i2);
	for (pch = (unsigned char *)&pa, idx = 0; idx < sizeof(PACKED_T); idx ++)
		printf("%02x-", pch[idx]);
	printf("\n");

	printf("unpacked = %d, aligned = %d, packed = %d\n", sizeof(UNPACKED_T), sizeof(ALIGNED_T), sizeof(PACKED_T));

        return 0;
}

