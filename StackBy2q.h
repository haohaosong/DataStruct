#pragma once

/*
* author:haohaosong
* data:2017/2/15
* note:两个队列实现一个栈 
*/ 

#include<iostream>
using namespace std;

#include<queue>
#include<assert.h>

//两个队列实现一个栈
template<typename T>
class StackBTQ
{
public:
	void Push(const T& x)
	{
		//入栈的元素直接push到qin里面
		qin.push(x);
	}

	void Pop()
	{
		assert(!qin.empty() || !qout.empty());
		//qin队列为空
		if (qin.empty())
		{
			while (qout.size() > 1)
			{
				qin.push(qout.front());
				qout.pop();
			}
			qout.pop();
		}
		else//qin队列不为空
		{
			while (qin.size() > 1)
			{
				qout.push(qin.front());
				qin.pop();
			}
			qin.pop();
		}
	}

	T&Top()
	{
		assert(!qin.empty() || !qout.empty());
		//qin队列为空
		if (qin.empty())
		{
			while (qout.size() > 1)
			{
				qin.push(qout.front());
				qout.pop();
			}
			return qout.front();
		}
		else//qin队列不为空
		{
			while (qin.size() > 1)
			{
				qout.push(qin.front());
				qin.pop();
			}
			return qin.front();
		}
	}
protected:
	queue<T> qin;
	queue<T> qout;
};

//两个队列实现一个栈
void TestForStackByTwoQueue()
{
	StackBTQ<int> s;
	s.Push(1);
	cout << s.Top() << endl;
	s.Push(2);
	cout << s.Top() << endl;
	s.Pop();
	cout << s.Top() << endl;
	s.Push(3);
	cout << s.Top() << endl;
	s.Pop();
	cout << s.Top() << endl;
}
