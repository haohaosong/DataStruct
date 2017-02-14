/*
* author:haohaosong
* date:2016/11/27
*/ 

#pragma once

#include<iostream>
using namespace std;
#include<assert.h>

template<typename T>
class Stack
{
public:
	Stack()
	  :_p(NULL)
	  , _size(0)
	  , _capacity(0)
	{}
	~Stack()
	{
		if (_p != NULL)
		{
			delete _p;
			_p = NULL;
			_size = _capacity = 0;
		}
	}
	void Push(const T& t)
	{
		CheckCapacity();
		_p[_size++] = t;
	}
	void Pop()
	{
		assert(_size);
		--_size;
	}
	T& Top()
	{
		return _p[_size - 1];
	}
	const T& Top()const
	{
		return _p[size - 1];
	}
	size_t Size()
	{
		return _size;
	}
	bool Empty()
	{
		return _size == 0;
	}
protected:
	T *_p;
	size_t _size;
	size_t _capacity;
	void CheckCapacity()
	{
		if (_size >= _capacity)
		{
			size_t NewCapacity = _capacity * 2 + 3;
			T* tmp = new T[NewCapacity];
			for (size_t idx = 0; idx < _capacity; ++idx)
			{
				tmp[idx] = _p[idx];
			}
			delete[] _p;
			_p = tmp;
			_capacity = NewCapacity;
		}
	}
};

void FunTest()
{
	Stack<int> s;
	s.Push(1);
	s.Push(2);
	s.Push(3);
	s.Push(4);
	
	while (!s.Empty())
	{
		cout << s.Top() << " ";
		s.Pop();
	}
	cout << endl;
}

