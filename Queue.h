/*
* author:haohaosong
* date:2016/11/27
*/ 

#pragma once

#include<iostream>
using namespace std;

#include<assert.h>

template<typename T>
struct QueueNode
{
	T* _value;
	QueueNode* next;
	QueueNode(const T& t)
	  :_value((T*)t)
	  , next(NULL)
	{}
};

template<typename T>
class Queue
{
	typedef QueueNode<T> Node;
public:
	Queue()
	  :_head(NULL)
	  , _tial(NULL)
   	{}
	~Queue()
	{
		Node* cur = _head;
		while (cur)
		{
			Node* del = cur;
			cur = cur->next;
			delete[] del;
			del = NULL;
		}
	}
	void Push(const T& t)
	{
		if (_head == NULL)
		{
			_head = new Node(t);
			_tial = _head;
		}
		else
		{
			_tial->next = new Node(t);
			_tial = _tial->next;
			_tial->next = NULL;
		}
	}
	void Pop()
	{
		assert(_head);
		if (_head == _tial)
		{
			delete[] _head;
			_head = _tial = NULL;
		}
		else
		{
			Node* del = _head;
			_head = _head->next;
			delete[] del;
			del = NULL;
		}
	}
	T& Front()
	{
		assert(_head);
		return (T&)_head->_value;
	}
	const T& Front()const
	{
		assert(_head);
		return _head->_value;
	}
	
	T& Back()
	{
		assert(_tial);
		return _tial->_value;
	}
	const T& Back()const
	{
		assert(_tial);
		return _tial->_value;
	}
	size_t Size()
	{
		size_t count = 0;
		Node* cur = _head;
		while (cur)
		{
			cur = cur->next;
			count++;
		}
		return count;
	}
	bool Empty()
	{
		return _head == NULL;
	}
protected:
	Node*_head;
	Node* _tial;
};

void TestQueue()
{
	Queue<int> q;
	q.Push(1);
	q.Push(2);
	q.Push(3);
	q.Push(4);

	while (!q.Empty())
	{
		cout << q.Front() << " ";
		q.Pop();
	}

	cout << endl;
}
