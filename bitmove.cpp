// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void print_2(int val2)
{
	unsigned char *p = (unsigned char*)&val2 + 3;
	for(int k = 0; k <= 3; k++)
	{
		int val2 = *(p-k);
		for (int i = 7; i >= 0; i--)
		{
			if(val2 & (1 << i))
				printf("1");
			else
				printf("0");
		}
		printf(" ");
	}
	printf("\n");
}
 
int main(int argc, char *argv[])
{
	unsigned nRet = 12;
	//nRet = 1 << 27;

	unsigned nRet2 = nRet << 5;

	print_2(nRet2);
	print_2(nRet);

    return 0;
}

