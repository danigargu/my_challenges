#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

//gcc -m32 -Wall -Wextra -zexecstack -fno-stack-protector -o test test.c

typedef int (*t_valid_flag) (char *flag);

unsigned char code[] = {
	0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x44, 0x56, 0xC7, 0x45, 0xF4, 0x01, 0x00, 0x00, 0x00, 0x8B, 0x45,
	0x08, 0x89, 0x45, 0xF8, 0x8B, 0x4D, 0x08, 0x89, 0x4D, 0xF0, 0x8B, 0x55, 0xF0, 0x83, 0xC2, 0x01,
	0x89, 0x55, 0xC0, 0x8B, 0x45, 0xF0, 0x8A, 0x08, 0x88, 0x4D, 0xFF, 0x83, 0x45, 0xF0, 0x01, 0x80,
	0x7D, 0xFF, 0x00, 0x75, 0xEE, 0x8B, 0x55, 0xF0, 0x2B, 0x55, 0xC0, 0x89, 0x55, 0xBC, 0x83, 0x7D,
	0xBC, 0x28, 0x75, 0x09, 0xC7, 0x45, 0xEC, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xEC,
	0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xF4, 0x23, 0x45, 0xEC, 0x89, 0x45, 0xF4, 0xB9, 0x04, 0x00,
	0x00, 0x00, 0x6B, 0xD1, 0x00, 0x8B, 0x45, 0xF8, 0x81, 0x3C, 0x10, 0x6E, 0x6E, 0x37, 0x65, 0x75,
	0x09, 0xC7, 0x45, 0xE8, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xE8, 0x00, 0x00, 0x00,
	0x00, 0x8B, 0x4D, 0xF4, 0x23, 0x4D, 0xE8, 0x89, 0x4D, 0xF4, 0xBA, 0x04, 0x00, 0x00, 0x00, 0x6B,
	0xC2, 0x00, 0xB9, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xD1, 0x09, 0x8B, 0x4D, 0xF8, 0x8B, 0x75, 0xF8,
	0x8B, 0x04, 0x01, 0x33, 0x04, 0x16, 0x3D, 0x1D, 0x4F, 0x16, 0x18, 0x75, 0x09, 0xC7, 0x45, 0xE4,
	0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x4D, 0xF4,
	0x23, 0x4D, 0xE4, 0x89, 0x4D, 0xF4, 0xBA, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC2, 0x09, 0x8B, 0x4D,
	0xF8, 0x8B, 0x14, 0x01, 0x81, 0xEA, 0x0F, 0xA6, 0xAE, 0x49, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xC1,
	0xE0, 0x00, 0x8B, 0x4D, 0xF8, 0x3B, 0x14, 0x01, 0x75, 0x09, 0xC7, 0x45, 0xE0, 0x01, 0x00, 0x00,
	0x00, 0xEB, 0x07, 0xC7, 0x45, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x55, 0xF4, 0x23, 0x55, 0xE0,
	0x89, 0x55, 0xF4, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xD1, 0xE0, 0x8B, 0x4D, 0xF8, 0x8B, 0x14, 0x01,
	0xC1, 0xEA, 0x0E, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xD1, 0xE0, 0x8B, 0x4D, 0xF8, 0x8B, 0x04, 0x01,
	0xC1, 0xE0, 0x12, 0x0B, 0xD0, 0x81, 0xFA, 0xC8, 0xCD, 0xD9, 0xCD, 0x75, 0x09, 0xC7, 0x45, 0xDC,
	0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x4D, 0xF4,
	0x23, 0x4D, 0xDC, 0x89, 0x4D, 0xF4, 0xBA, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC2, 0x03, 0x8B, 0x4D,
	0xF8, 0x8B, 0x14, 0x01, 0xC1, 0xE2, 0x10, 0xB8, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC8, 0x03, 0x8B,
	0x45, 0xF8, 0x8B, 0x0C, 0x08, 0xC1, 0xE9, 0x10, 0x0B, 0xD1, 0x81, 0xFA, 0x47, 0x5F, 0x31, 0x6E,
	0x75, 0x09, 0xC7, 0x45, 0xD8, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xD8, 0x00, 0x00,
	0x00, 0x00, 0x8B, 0x55, 0xF4, 0x23, 0x55, 0xD8, 0x89, 0x55, 0xF4, 0xB8, 0x04, 0x00, 0x00, 0x00,
	0xC1, 0xE0, 0x02, 0x8B, 0x4D, 0xF8, 0x8B, 0x14, 0x01, 0xC1, 0xE2, 0x12, 0xB8, 0x04, 0x00, 0x00,
	0x00, 0xC1, 0xE0, 0x02, 0x8B, 0x4D, 0xF8, 0x8B, 0x04, 0x01, 0xC1, 0xE8, 0x0E, 0x0B, 0xD0, 0xC1,
	0xEA, 0x04, 0xB9, 0x04, 0x00, 0x00, 0x00, 0xC1, 0xE1, 0x02, 0x8B, 0x45, 0xF8, 0x8B, 0x0C, 0x08,
	0xC1, 0xE1, 0x12, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xC1, 0xE0, 0x02, 0x8B, 0x75, 0xF8, 0x8B, 0x04,
	0x06, 0xC1, 0xE8, 0x0E, 0x0B, 0xC8, 0xC1, 0xE1, 0x1C, 0x0B, 0xD1, 0x81, 0xFA, 0x0C, 0xDB, 0x1C,
	0xDA, 0x75, 0x09, 0xC7, 0x45, 0xD4, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xD4, 0x00,
	0x00, 0x00, 0x00, 0x8B, 0x4D, 0xF4, 0x23, 0x4D, 0xD4, 0x89, 0x4D, 0xF4, 0xBA, 0x04, 0x00, 0x00,
	0x00, 0x6B, 0xC2, 0x05, 0x8B, 0x4D, 0xF8, 0x8B, 0x14, 0x01, 0xF7, 0xD2, 0x81, 0xFA, 0x93, 0x9C,
	0xCF, 0x9B, 0x75, 0x09, 0xC7, 0x45, 0xD0, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xD0,
	0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xF4, 0x23, 0x45, 0xD0, 0x89, 0x45, 0xF4, 0xB9, 0x04, 0x00,
	0x00, 0x00, 0x6B, 0xD1, 0x05, 0xB8, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC8, 0x06, 0x8B, 0x45, 0xF8,
	0x8B, 0x75, 0xF8, 0x8B, 0x14, 0x10, 0x2B, 0x14, 0x0E, 0xF7, 0xD2, 0x81, 0xFA, 0xF8, 0x0F, 0x2F,
	0xCD, 0x75, 0x09, 0xC7, 0x45, 0xCC, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xCC, 0x00,
	0x00, 0x00, 0x00, 0x8B, 0x45, 0xF4, 0x23, 0x45, 0xCC, 0x89, 0x45, 0xF4, 0xB9, 0x04, 0x00, 0x00,
	0x00, 0x6B, 0xD1, 0x00, 0x8B, 0x45, 0xF8, 0x8B, 0x0C, 0x10, 0xC1, 0xE1, 0x08, 0xBA, 0x04, 0x00,
	0x00, 0x00, 0x6B, 0xC2, 0x00, 0x8B, 0x55, 0xF8, 0x8B, 0x04, 0x02, 0xC1, 0xE8, 0x18, 0x0B, 0xC8,
	0xBA, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC2, 0x07, 0x8B, 0x55, 0xF8, 0x03, 0x0C, 0x02, 0x81, 0xF9,
	0xD8, 0xCD, 0xE2, 0x68, 0x75, 0x09, 0xC7, 0x45, 0xC8, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7,
	0x45, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xF4, 0x23, 0x45, 0xC8, 0x89, 0x45, 0xF4, 0xB9,
	0x04, 0x00, 0x00, 0x00, 0xC1, 0xE1, 0x03, 0xBA, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC2, 0x09, 0x8B,
	0x55, 0xF8, 0x8B, 0x75, 0xF8, 0x8B, 0x0C, 0x0A, 0x23, 0x0C, 0x06, 0x6B, 0xD1, 0xFE, 0xB8, 0x04,
	0x00, 0x00, 0x00, 0xC1, 0xE0, 0x03, 0xB9, 0x04, 0x00, 0x00, 0x00, 0x6B, 0xC9, 0x09, 0x8B, 0x75,
	0xF8, 0x8B, 0x04, 0x06, 0x8B, 0x75, 0xF8, 0x03, 0x04, 0x0E, 0x03, 0xD0, 0x81, 0xFA, 0x17, 0x10,
	0x11, 0x08, 0x75, 0x09, 0xC7, 0x45, 0xC4, 0x01, 0x00, 0x00, 0x00, 0xEB, 0x07, 0xC7, 0x45, 0xC4,
	0x00, 0x00, 0x00, 0x00, 0x8B, 0x4D, 0xF4, 0x23, 0x4D, 0xC4, 0x89, 0x4D, 0xF4, 0x8B, 0x45, 0xF4,
	0xEB, 0x05, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x5E, 0x8B, 0xE5, 0x5D, 0xC3 
};

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: %s <flag>", argv[0]);
		return 0;
	}
	t_valid_flag valid_flag = (t_valid_flag)code;
	if (valid_flag(argv[1]))
		printf("valid!\n");
	else
		printf("invalid flag\n");

	return 0;
}
