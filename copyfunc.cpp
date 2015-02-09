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
using namespace std;

class simpleClass
{
	private:
		char *m_buf;
		int m_nSize;
		int *m_count;
	public:
		simpleClass(int n=1)
		{
			m_buf = new char[n];
			m_nSize = n;

			m_count = new int;
			*m_count = 1;
			printf("count is : %d\n", *m_count);
		};

		simpleClass(const simpleClass &s)
		{
			m_nSize = s.m_nSize;
			m_buf = s.m_buf;
			m_count = s.m_count;

			(*m_count)++;
			printf("copy count is:%d \n", *m_count);
		};

		~simpleClass()
		{
			(*m_count)--;
			printf("~~~ count is : %d,size:%d \n", *m_count, m_nSize);

			if(*m_count == 0)
			{
				printf("~~ %d is deleted at %x\n", m_nSize, m_buf);
				delete[] m_buf;
				delete m_count;
			}
		};

		char* GetBuf()
		{
			return m_buf;
		};

		simpleClass& operator=(const simpleClass& s)
		{
			if(m_buf == s.m_buf)
				return *this;

			(*m_count)--;

			if(*m_count == 0)
			{
				printf("~~ in === %d is deleted at %x\n", m_nSize, m_buf);
				delete[] m_buf;
				delete m_count;
			}
			m_nSize = s.m_nSize;
			m_buf = s.m_buf;
			m_count = s.m_count;
			(*m_count)++;
		}
};

void foo()
{
	simpleClass a(10);
	char *p = a.GetBuf();

	strcpy(p, "hello");
	printf("%s\n", p);

	simpleClass b=a;
	printf("b=%s\n", b.GetBuf());


	printf("c begin~~~~~~~~~~~\n");
	simpleClass c(20);
	c = a;
	printf("c=%s\n", c.GetBuf());

}

int main(int argc, char** argv)
{
	foo();
	printf("exit main....\n");
	return 0;
}


