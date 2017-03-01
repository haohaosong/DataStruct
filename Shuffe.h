#pragma once

/*
* author:haohaosong
* date:2017/3/1
* note:洗牌算法的实现
*/

#include<iostream>
using namespace std;

#include<time.h>

//打印函数
void Print(int* a, size_t n)
{
	for (size_t i = 0; i<n; ++i)
		cout << a[i] << " ";

	cout << endl;
}

//进行洗牌
void Shuffe(int* a, int n)
{
	for (int i = n - 1; i > 0; --i)
	{
		srand((unsigned)time(NULL));//不加此语句的话，每次随机的数会不变
		swap(a[i], a[rand() % (i + 1)]);
	}
}

//测试洗牌算法
void TestShuffe()
{
	int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Shuffe(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}