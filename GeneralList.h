/*
* author:haohaosong
* date:2016/12/11
* note:广义表 
*/
#pragma once 

#include<iostream>
using namespace std;
#include<assert.h>

//定义一个枚举类型，用于判断是头还是值，还是有子链 
enum Type
{
	HEAD,
	VALUE,
	SUB,
};

//定义广义表的结果体 
struct GeneralNode
{
	Type _type;
	GeneralNode *next;
	union
	{
		char _value;
		GeneralNode* _SubLink;
	};

	GeneralNode(Type type, const char value = ' ')
		:_type(type)
		, next(NULL)
	{
		if (_type == VALUE)
			_value = value;
		else if (type == SUB)
			_SubLink = NULL;
	}
};

//定义广义表 
class GeneralList
{
	//类型重命名 
	typedef GeneralNode Node;
public:
	//构造方法 
	GeneralList()
	{
		_head = new Node(HEAD);
	}
	GeneralList(const char *s)
	{
		_head = _CreatList(s);
	}
	
	//拷贝构造函数 
	GeneralList(const GeneralList& g)
	{
		_head = _Copy(g._head);
	}
	
	//赋值运算符重载 
	GeneralList& operator=(GeneralList &g)
	{
		if (this != &g)
		{
			GeneralList tmp(g);
			std::swap(_head, g._head);
		}
		return *this;
	}
	
	//析构函数 
	~GeneralList()
	{
		Destory();
		_head = NULL;
	}
	void Print()
	{
		_Print(_head);
		cout << endl;
	}
	
	//求深度，节点个数 
	size_t Size()
	{
		return _Size(_head);
	}
	size_t Depth()
	{
		return _Depth(_head);
	}
	void Destory()
	{
		_Destory(_head);
	}
protected:
	Node* _head;
protected:
	//Create调用_Create 
	Node* _CreatList(const char *& s)
	{
		assert(s);
		Node* newhead = new Node(HEAD);
		s++;
		Node* tail = newhead;
		while (s)
		{
			if (_IsValue(*s))
			{
				Node* newnode = new Node(VALUE, *s);
				tail->next = newnode;
				tail = tail->next;
				s++;
			}
			else if (*s == '(')
			{
				Node* newnode = new Node(SUB);
				newnode->_SubLink = _CreatList(s);
				tail->next = newnode;
				tail = tail->next;
			}
			else if (*s == ')')
			{
				s++;
				return newhead;
			}
			else
				s++;
		}
		return newhead;
	}
	
	//判断是不是值 
	bool _IsValue(char ch)
	{
		if ((ch >= '0'&&ch <= '9')
			|| (ch >= 'a'&&ch <= 'z')
			|| (ch >= 'A'&&ch <= 'Z'))
		{
			return true;
		}
		return false;
	}
	
	//Print调用_Print
	void _Print(Node *head)
	{
		assert(head && head->_type == HEAD);
		Node* cur = head;
		while (cur)
		{
			if (cur->_type == HEAD)
				cout << '(';
			else if (cur->_type == VALUE)
			{
				cout << cur->_value;
				if (cur->next && cur->next->_type == VALUE)
					cout << ',';
			}
			else if (cur->_type == SUB)
			{
				cout << ',';
				_Print(cur->_SubLink);
				if (cur->next&&cur->next->_type == VALUE)
					cout << ',';
			}
			cur = cur->next;
		}
		cout << ')';
	}
	
	//Size调用 _Size() 
	size_t _Size(Node* head)
	{
		assert(head && head->_type == HEAD);
		Node* cur = head;
		size_t count = 0;
		while (cur)
		{
			if (cur->_type == VALUE)
				++count;
			else if (cur->_type == SUB)
				count += _Size(cur->_SubLink);
			cur = cur->next;
		}
		return count;
	}
	
	//Depth调用_Depth 
	size_t _Depth(Node* head)
	{
		assert(head && head->_type == HEAD);
		Node* cur = head;
		size_t maxdepth = 1;
		size_t tmp = 0;
		while (cur)
		{
			if (cur->_type == SUB)
			{
				tmp = _Depth(cur->_SubLink);
				if (maxdepth < tmp + 1)
					maxdepth = tmp + 1;
			}
			cur = cur->next;
		}
		return maxdepth;
	}
	
	//Copy调用_Copy 
	Node* _Copy(Node* head)
	{
		assert(head&&head->_type == HEAD);
		Node* newhead = new Node(HEAD);
		Node* tail = newhead;
		Node* cur = head->next;
		while (cur)
		{
			if (cur->_type == SUB)//生成一个SUB节点 
			{
				Node* newnode = new Node(SUB);
				newnode->_SubLink = _Copy(cur->_SubLink);
				tail->next = newnode;
			}
			else if (cur->_type == VALUE)//生成一个值节点 
				tail->next = new Node(VALUE, cur->_value);
			else
				assert(false);
			
			//分别指向下一个 
			cur = cur->next;
			tail = tail->next;
		}
		return newhead;
	}
	void _Destory(Node* head)
	{
		assert(head&&head->_type == HEAD);
		Node* cur = head;
		while (cur)
		{
			//用cur记录下一个节点 
			Node* del = cur;
			cur = cur->next;
			
			//递归删除一个链 
			if (del->_type == SUB)
				_Destory(del->_SubLink);
			
			delete del;
			del = NULL;
		}
	}
};

//测试函数 
void TestGeneral()
{
	GeneralList g1("(a,b,(c,d))");
	g1.Print();
	cout << "g1:size = "<<g1.Size() << endl;
	cout << "g1.depth = " << g1.Depth() << endl;

	GeneralList g2("(a,b,(c,d),(f,(g),h))");
	g2.Print();
	cout << "g2:size = " << g2.Size() << endl;
	cout << "g2.depth = " << g2.Depth() << endl;

	GeneralList g3(g2);
	g3.Print();
	cout << "g3:size = " << g3.Size() << endl;
	cout << "g3.depth = " << g3.Depth() << endl;

	GeneralList g4;
	g4 = g2;
	g4.Print();
	cout << "g4:size = " << g4.Size() << endl;
	cout << "g4.depth = " << g4.Depth() << endl;
}
