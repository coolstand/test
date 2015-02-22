/*
 *This is a test
 *author: junkizou
 *
 *
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <zmq.h>
#include <assert.h>
#include <unistd.h>
#include <iomanip>
using namespace std;

int main(int argc, char** argv)
{
	int nRet = 0;

	const int n=11;
	int i,j,a[n][n];
	for(i=1;i<n;i++)
	{
		a[i][i]=1;
		a[i][1]=1;
	}
	for(i=3;i<n;i++)
	{
		for(j=2;j<=i-1;j++)  
			a[i][j]=a[i-1][j-1]+a[i-1][j];  
	}
	for(i=1;i<n;i++)
	{
		for(j=1;j<=i;j++)
			cout<<setw(5)<<a[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;

return 0;
}



