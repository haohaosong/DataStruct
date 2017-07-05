#pragma once

/*
* note:所谓迭代器，就是为了让这些容器可以用类似指针的功能来访问元素
*      在C语言中，利用指针的++，--，解引用可以方便的操作、获取数据
*	   而C++中的迭代器，就是模拟实现类似指针的功能
*/

#include<iostream>
using namespace std;

//定义链表节点的结构体
template<typename T>
struct __ListNode
{
	__ListNode<T>* _next;
	__ListNode<T>* _prev;
	T _data;

	__ListNode(const T& data)//构建节点的构造函数
		:_next(NULL)
		, _prev(NULL)
		, _data(data)
	{}
};

//定义迭代器,模板参数为T,T的引用,T类型的指针
template<typename T, typename Ref, typename Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;//将链表节点重命名为 Node
	typedef __ListIterator<T, T&, T*> self; //将自己的类型重命名为 self，简化命名
	Node* _node;

	__ListIterator(){}
	__ListIterator(Node* node)
		:_node(node)
	{}

	//重载 !=、==操作符
	bool operator != (const self& s)
	{
		return _node != s._node;
	}
	bool operator == (const self& s)
	{
		return _node == s._node;
	}

	//重载 ++、-- 操作符
	self& operator ++()
	{
		_node = _node->_next;
		return (self&)_node;
	}
	self operator ++ (int)
	{
		Node* tmp = _node;
		++_node;
		return tmp;
	}
	self& operator --()
	{
		_node = _node->_prev;
		return (self&)_node;
	}
	self operator -- (int)
	{
		Node* tmp = _node;
		--_node;
		return tmp;
	}

	//重载*、->操作符
	Ref operator*()
	{
		return (*_node)._data;
	}
	Ref operator->()
	{
		return *_node;
	}
};

//定义反向迭代器,模板参数为T,T的引用,T类型的指针
template<typename T, typename Ref, typename Ptr>
struct __ListReverseIterator
{
	typedef __ListNode<T> Node;//将链表节点重命名为 Node
	typedef __ListReverseIterator<T, T&, T*> self;//将自己的类型重命名为self，简化命名

	Node* _node;
	__ListReverseIterator()
	{}
	__ListReverseIterator(Node* node)
		:_node(node)
	{}

	//重载 ++、-- 操作符
	self& operator++()
	{
		_node = _node->_prev;//注意：这里是反向迭代器，++应该指向前一个
							//这是反向迭代器最关键的地方
		return (self&)_node;
	}
	self operator++(int)
	{
		Node* tmp = _node;
		_node = _node->_prev;
		return tmp;
	}
	self& operator--()
	{
		_node = _node->_next;
		return (self&)_node;
	}
	self operator--(int)
	{
		Node* tmp = _node;
		_node = _node->_next;
		return tmp;
	}

	//重载 !=、==操作符
	bool operator==(const self& s)
	{
		return _node->_data == s._node->_data;
	}
	bool operator!=(const self& s)
	{
		return _node->_data != s._node->_data;
	}

	//重载*、->操作符
	Ref operator*()
	{
		return (*_node)._data;
	}
	Ref operator->()
	{
		return *_node;
	}
};

//定义链表
template<typename T>
class List
{
	typedef __ListNode<T> Node;
public:
	typedef __ListIterator<T, T&, T*> Iterator;//重命名为迭代器
	typedef __ListReverseIterator<T, T&, T*> ReverseIterator;//重命名为反向迭代器
	
	//申请一个新的节点
	Node* BuyNewNode(const T& t)
	{
		return new Node(t);
	}
	//构造函数
	List()
		:_head(BuyNewNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	//在尾部插入，删除一个元素
	void PushBack(const T& x)
	{
		Node* tail = _head->_prev;
		Node* tmp = BuyNewNode(x);
		tail->_next = tmp;
		tmp->_prev = tail;
		tmp->_next = _head;
		_head->_prev = tmp;
	}

	//定义返回迭代器类型的Begin()和End()
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}
	Iterator End()
	{
		return Iterator(_head);
	}

	//定义返回迭代器类型的rBegin()和rEnd()
	ReverseIterator rBegin()
	{
		return ReverseIterator(_head->_prev);
	}
	ReverseIterator rEnd()
	{
		return ReverseIterator(_head);
	}
protected:
	Node* _head;
};

//测试链表
void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);

	//打印l中的元素
	List<int> ::Iterator it1 = l.Begin();//定义迭代器it1为l的起始部分
	while (it1 != l.End())
	{
		cout << *it1 << " ";//重载了解引用操作符，可以访问it1中的元素
		++it1;//重载了++运算符，可以访问下一个元素
	}
	cout << endl;
}

void TestReverseIterator()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);

	List<int>::ReverseIterator it = l.rBegin();//定义反向迭代器it为反向迭代器的第一个元素
	while (it != l.rEnd())
	{
		cout << *it << " ";//重载了解引用操作符，访问元素当前内容
		it++;//重载了++运算符，访问后面的元素
	}
	cout << endl;
}
