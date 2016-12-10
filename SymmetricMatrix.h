/*
* author:haohaosong
* date:2016/12/10
*/ 
#pragma once

#include<iostream>
using namespace std;

template<typename T>
class Symmetric
{
public:
	Symmetric(T* a, size_t n)
		:_a(new T[n*(n + 1) / 2])
		, _n(n)
	{
		int index = 0;
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j <= i; ++j)
			{
				_a[index] = a[i*_n + j];
				++index;
			}
		}
	}
	~Symmetric()
	{
		if (_a != NULL)
		{
			delete[] _a;
			_a = NULL;
		}
	}
	T& Access(size_t i, size_t j)
	{
		if (i < j)
		{
			std::swap(i, j);
		}
		return _a[i*(i+1)/2+ j];
	}
	void Display()
	{
		for (size_t i = 0; i < _n; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				cout << Access(i, j) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
protected:
	T* _a;
	size_t _n;
};

void TestSymmetricMatrix()
{
	int arr[5][5] = {
		{ 0, 1, 2, 3, 4 },
		{ 1, 0, 3, 4, 3 },
		{ 2, 3, 0, 3, 2 },
		{ 3, 4, 3, 0, 1 },
		{ 4, 3, 2, 1, 0 },
	};
	Symmetric<int> s((int*)arr, 5);
	s.Display();
}
