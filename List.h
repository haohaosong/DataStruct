#pragma once

template<typename T>
struct __ListNode
{
	__ListNode<T>* _next;
	__ListNode<T>* _prev;
	T _data;
	__ListNode(const T& data)
		:_next(NULL)
		, _prev(NULL)
		, _data(data)
	{}
};

template<typename T,typename Ref,typename Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;
	typedef __ListIterator<T, T&, T*> self;
	Node* _node;
	__ListIterator(){}
	__ListIterator(Node* node)
		:_node(node)
	{}
	bool operator != (const self& s)
	{
		return _node != s._node;
	}
	bool operator == (const self& s)
	{
		return _node == s._node;
	}
	self& operator ++()
	{
		_node = _node->_next;
		return (self)_node;
	}
	self operator ++ (int)
	{
		Node* tmp = _node;
		++_node;
		return tmp;
	}
	Ref operator*()
	{
		return (*_node)._data;
	}
	Ref operator->()
	{
		return *_node;
	}
};

template<typename T>
class List
{
	typedef __ListNode<T> Node;
public:
	typedef __ListIterator<T, T&, T*> Iterator;
	Node* BuyNewNode(const T& t)
	{
		return new Node(t);
	}
	List()
		:_head(BuyNewNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}
	void PushBack(const T& x)
	{
		Node* tail = _head->_prev;
		Node* tmp = BuyNewNode(x);
		tail->_next = tmp;
		tmp->_prev = tail;
		tmp->_next = _head;
		_head->_prev = tmp;
	}
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}
	Iterator End()
	{
		return Iterator(_head);
	}
protected:
	Node* _head;
};

void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);

	List<int> ::Iterator it1 = l.Begin();
	while (it1 != l.End())
	{
		cout << *it1 << " ";
		++it1;
	}
	cout << endl;

}
