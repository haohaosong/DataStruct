#pragma once

/*
* author:haohaosong
* date:2017/2/21
* note:位图的实现 
*/ 

#include<iostream>
using namespace std;

#include<vector>

//定义位图
class BitMap
{
public:
	//位图的构造函数
	BitMap(size_t size = 1024)
		:_size(0)
	{
		//一个无符号整形(size_t)可以有32个位分别表示32个数
		_v.resize(size / 32 + 1);
	}

	//放入，将num的所在位置为1
	void Set(size_t num)
	{
		//index用来判断在vector的第几个元素中
		//value表示在该元素中从低位向高位数具体的哪一位
		//index用右移5位，相当于除32，用移位运算效率高
		int index = num >> 5;
		int value = num % 32;

		//利用或运算，将该位置为1
		_v[index] |= 1 << (value-1);
		_size++;
	}

	//移出，将num的所在位置为0
	void Reset(size_t num)
	{
		//index用来判断在vector的第几个元素中
		//value表示在该元素中从低位向高位数具体的哪一位
		//index用右移5位，相当于除32，用移位运算效率高
		int index = num >> 5;
		int value = num % 32;

		//利用异或运算，将该位置为0
		_v[index] ^= 1 << (value-1);
		_size--;
	}

	//用来判断一个数是否已经存入位图中
	bool Find(size_t num)
	{
		int index = num >> 5;
		int value = num % 32;

		//判断该位是否为1
		return (_v[index] >> (value-1)) & 1;
	}
protected:
	vector<size_t> _v;
	size_t _size;
};

void TestBitMap()
{
	BitMap bp;
	bp.Set(1);
	cout << "1:" << bp.Find(1) << endl;

	bp.Set(2);
	bp.Set(3);

	cout << "1:" << bp.Find(1) << endl;
	cout << "2:" << bp.Find(2) << endl;
	cout << "5:" << bp.Find(5) << endl;
}
