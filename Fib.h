#include<iostream>
using namespace std;

/*
* author:haohaosong
* date:2016/11/21
*/


/*递归
/*时间复杂度:O(2^N)*/
/*空间复杂度:O(N)*/

long long Fib(long long  n)
{
	return (n < 2) ? n : (Fib(n-1) + Fib(n-2));
}

/*时间复杂度:O(N)*/
/*空间复杂度:O(1)*/

long long Fib(long long n)
{
	if(n<2)
		return n;
	long long frist = 0;
	long long second = 1;
	long long third = 0;
	for(long long i = 2; i<=n; ++i)
	{
		third = frist+second;
		frist = second;
		second = third;
	}
	return third;
}

void FunTest()
{
	cout<<Fib(10)<<endl;
}
