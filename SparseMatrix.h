/*
* author:haohaosong
* date:2016/12/10
*/

#pragma once 

#include<iostream>
using namespace std;
#include<vector>

template<typename T>
struct Triple
{
	size_t _row;//行
	size_t _col;//列
	T _value;

	Triple(size_t row = 0, size_t col = 0, const T &value = T())
		:_row(row)
		, _col(col)
		, _value(value)
	{}
};

template<typename T >
class SparseMatrix
{
public:
	SparseMatrix(T* arr = 0, size_t m = 0, size_t n = 0, const T& invalid = T())
		:_m(m)
		, _n(n)
		,_invalid(invalid)
	{
		for (size_t i = 0; i < _m; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				if (arr[i*n + j] != _invalid)
				{
					_a.push_back({ i, j, arr[i*n + j] });
				}
			}
		}
	}
	void Display()
	{
		size_t index = 0;
		for (size_t i = 0; i < _m; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				if (index<_a.size()
					&& i == _a[index]._row
					&& j == _a[index]._col)
				{
					cout << _a[index]._value<<" ";
					++index;
				}
				else
				{
					cout << _invalid << " ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
	SparseMatrix<T> Transport()
	{
		SparseMatrix<T> tmp;
		tmp._invalid = _invalid;
		tmp._m = _n;
		tmp._n = _m;
		tmp._a.reserve(_a.size());
		
		for (size_t index = 0; tmp._a.size() < _a.size(); ++index)
		{
			for (size_t i = 0; i < _a.size(); ++i)
			{
				if (_a[i]._col == index)
				{
					tmp._a.push_back({ _a[i]._col, _a[i]._row, _a[i]._value });
				}
			}
		}

		return tmp;
	}
	SparseMatrix<T> FastTransport()
	{
		int* count = new int[_n];
		int* start = new int[_n];
		
		memset(count, 0, sizeof(int)*_n);

		size_t index = 0;
		while (index < _n)
		{
			count[_a[index]._col]++;
			index++;
		}

		start[0] = 0;
		index = 1;
		while (index < _n)
		{
			start[index] = start[index - 1] + count[index - 1];
			++index;
		}

		SparseMatrix<T> tmp;
		tmp._m = _n;
		tmp._n = _m;
		tmp._invalid = _invalid;
		tmp._a.resize(_a.size());
		
		index = 0;
		while (index < _a.size())
		{
			tmp._a[start[_a[index]._col]++] = { _a[index]._col,_a[index]._row,_a[index]._value };
			index++;
		}

		delete[] start;
		delete[] count;
		start = NULL;
		count = NULL;

		return tmp;
	}
protected:
	vector<Triple<T>> _a;
	size_t _m;//行
	size_t _n;//列
	T _invalid;
};

void TestSparseMatrix()
{
	int arr[6][5] =
	{
		{ 1, 0, 3, 0, 5 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 2, 0, 4, 0, 6 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	};
	SparseMatrix<int> s((int*)arr,6,5,0);
	s.Display();
	SparseMatrix<int> ret =  s.Transport();
	ret.Display();
	ret =  s.FastTransport();
	ret.Display();
}
