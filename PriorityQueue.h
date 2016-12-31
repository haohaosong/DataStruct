/*
* author:haohaosong
* date:2016/12/31
* note:利用堆实现优先级队列 
*/ 

#pragma once 

#include<iostream>
using namespace std;

#include<vector>

template<typename T>
struct Less
{
	bool operator()(T& t1,T& t2)
	{
		return t1 < t2;
	}
};

template<typename T>
struct Greater
{
	bool operator()(T& t1, T& t2)
	{
		return t1>t2;
	}
};

//用堆实现优先级队列
template<typename T,typename Com = Greater<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(int* a,size_t n)
		:v(a,a+n)
	{
		for (int i = (n - 2) / 2; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		v.push_back(x);
		_AdjustUp(v.size()-1);
	}

	void Pop()
	{
		swap(v[0], v[v.size() - 1]);
		v.pop_back();
		_AdjustDown(0);
	}

	const T& Top()
	{
		return v[0];
	}

	size_t Size()
	{
		return v.size();
	}

	bool Empty()
	{
		return v.empty();
	}
protected:
	vector<T> v;
protected:
	void _AdjustDown(int root)
	{
		int parent = root;
		int child = parent * 2 + 1;

		while (child < v.size())
		{
			if (child + 1 < v.size()&& Com()(v[child + 1],v[child]))
				child++;

			if (Com()(v[child],v[parent]))
			{
				swap(v[child],v[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	void _AdjustUp(int i)
	{
		int child = i;
		int parent = (i - 1) / 2;

		while (child >= 0)
		{
			if (Com()(v[child],v[parent]))
			{
				swap(v[child], v[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}

	}
};

template<typename T>
class PriorityQueue
{
public:
	PriorityQueue()
	{}

	PriorityQueue(int* arr, size_t n)
		:hp(arr, n)
	{}
public:
	void Push(const T& x)
	{
		hp.Push(x);
	}
	void Pop()
	{
		hp.Pop();
	}
	bool Empty()
	{
		return hp.Empty();
	}
	size_t Size()
	{
		return hp.Size();
	}
	const T& Top()
	{
		return hp.Top();
	}
protected:
	Heap<T> hp;
};

void TestPriorityQueue()
{
	int a[10] = {10,20,30,40,15,16,39,14,19,26};
	PriorityQueue<int> p(a,10);
	p.Push(100);
	cout << p.Top() << endl;
	p.Pop();
	cout << "Empty:" << p.Empty() << endl;
	cout << "Size:"<<p.Size() << endl;
	cout << "Top:" << p.Top() << endl;
}
