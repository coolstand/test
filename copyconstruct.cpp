/*
 *This is a test
 *author: junkizou
 *
 *
 */
#include <iostream>
//#include "test.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
using namespace std;

class simpleClass
{
	friend const simpleClass operator+(const simpleClass& a, const simpleClass& b);
	public:
		simpleClass(void)
		{
			printf("default construct!\n");
		}

		simpleClass(const simpleClass& simple)
		{
			printf("copy construct!\n");
		}

		simpleClass& operator=(const simpleClass& simple)
		{
			printf("operator ========\n");
			return *this;
		}
};

const simpleClass operator+(const simpleClass& a, const simpleClass& b)
{
	printf("operator +++++ begin\n");
	simpleClass c;
	printf("operator +++++ end\n");
	return c;
}

int main(int argc, char** argv)
{
	simpleClass b,d;
	printf("b&d created!!!\n");
	simpleClass c;
	c = b + d;
	return 0;
}


