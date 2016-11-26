#include<iostream>
using namespace std;
/*递归
/*时间复杂度:2^N*/
/*空间复杂度:N*/
/*
long long Fib(long long  n)
{
	return (n < 2) ? n : (Fib(n-1) + Fib(n-2));
}

/*时间复杂度:N*/
/*空间复杂度:1*/
/*
long long Fib(long long n)
{
	if(n<2)
		return n;
	int frist = 0;
	int second = 1;
	int third = 0;
	for(long long i=2; i<=n; ++i)
	{
		third = frist+second;
		frist = second;
		second = third;
	}
	return third;
}*/

int main()
{
	cout<<Fib(10)<<endl;
	return 0;
}
