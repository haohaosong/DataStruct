#pragma once 

/*
* author:haohaosong
* date:2016/12/24
* node:二叉搜索树，它可以把二叉树搜索化为自己想要的序列，在此后的遍历中，不在需要递归
*      二叉搜索树的链接方法，后继很好找，前驱的话需要通过时光机来完成~
*/

#include<iostream>
using namespace std;

//定义一个枚举类型，用于判断节点是否被搜索过
enum PointerTag
{
	LINK,
	THREDA,
};

//定义二叉搜索树的节点
template<typename T>
struct BinaryTreeThdNode
{
	typedef BinaryTreeThdNode Node;

	T _data;

	Node* _left;
	Node* _right; 

	//标记，用于判断孩子有没有被搜索过
	PointerTag _leftTag;
	PointerTag _rightTag;

	BinaryTreeThdNode(T& x)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
		, _leftTag(LINK)
		, _rightTag(LINK)
	{}
};

template<typename T,typename Ref,typename Ptr>
struct  BinaryTreeTheIterator
{
	typedef BinaryTreeTheIterator<T, T&, T*> Self; //简化命名为Self
	typedef BinaryTreeThdNode<T> Node;

	Node* _node;

	BinaryTreeTheIterator(Node* node = NULL)
		:_node(node)
	{}

	//重载 == 和 != 
	bool operator == (const Self& s)
	{
		return _node == s._node; 
	}
	bool operator != (const Self& s)
	{
		return _node != s._node;
	}

	//重载解引用和箭头
	Ref operator*()
	{
		return (*_node)._data;
	}
	Ref operator->()
	{
		return &(operator*());
	}

	//重载++
	Self& operator++()
	{
		//如果右孩子是被搜索过的，直接找后继
		if (_node->_rightTag == THREDA)
		{
			_node = _node->_right;
		}
		else
		{
			//先找到右孩子
			if (_node)
				_node = _node->_right;
			
			//在右孩子中找最左边的节点
			if (_node)
			{
				while (_node->_leftTag == LINK)
					_node = _node->_left;
			}
		}
		return *this;
	}
	Self operator++(int)//后置++
	{
		Self tmp(*this);
		++(*this);
		return tmp;
	}

	// -- 暂时没有完成

	//Self& operator--()
	//{
	//	if (_node->_leftTag == THREDA)
	//	{
	//		_node = _node->_left;
	//	}
	//	else
	//	{
	//		Node* cur = _root;

	//		while (cur)
	//		{
	//			//找起始节点
	//			while (cur->_leftTag == LINK)
	//			{
	//				cur = cur->_left;
	//			}

	//			//循环找到this的上一个
	//			while (cur != *this)
	//			{
	//				_node = cur;
	//				++cur;
	//			}
	//		}
	//	}
	//	return *this;
	//}
	//Ref operator--(int)
	//{
	//	Self tmp(*this);
	//	--(*this);
	//	return tmp;
	//}
};

//定义二叉搜索树
template<typename T>
class BinaryTreeThd
{
	typedef BinaryTreeThdNode<T> Node;//简化命名
public:	
	typedef BinaryTreeTheIterator<T, T&, T*> Iterator;
	typedef BinaryTreeTheIterator<T, const T&, const T*> ConstIterator;
public:
	BinaryTreeThd()
		:_root(NULL)
	{}

	BinaryTreeThd(T* arr, size_t sz, T invalid = T())
	{
		size_t index = 0;
		_root = CreateTree(arr, sz, index, invalid);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	void InOrderThreadding()
	{
		Node* prev = NULL;
		_InOrderThreadding(_root, prev);
	}

	void InOrderThd()
	{
		Node* cur = _root;

		while (cur)
		{ 
			//循环找到最左边的节点
			while (cur->_leftTag == LINK)
			{
				cur = cur->_left;
			}

			cout << cur->_data << " ";

			//判断右边是不是可以直接利用后继，需要是while循环，否则无法连续跳
			while (cur->_rightTag == THREDA)
			{
				cur = cur->_right;
				//只要cur存在，那么可以直接访问
				if (cur)
					cout << cur->_data << " ";
			}

			//右孩子是LINK节点，没有被搜索，那么进行访问
			cur = cur->_right;
		}
		cout << endl;
	}

	Iterator Begin()
	{
		Node* cur = _root;
		//从根开始，找最左边的节点
		while (cur->_leftTag == LINK)
			cur = cur->_left;

		return cur;
	}

	Iterator End()
	{
		//左闭右开，End()为空
		return NULL;
	}

protected:
	Node* CreateTree(T* arr, const size_t sz, size_t& index, T& invalid)
	{
		if (index < sz && arr[index] != invalid)
		{
			Node* root = new Node(arr[index]);
			root->_left = CreateTree(arr,sz,++index,invalid);
			root->_right = CreateTree(arr, sz,++index, invalid);
			return root;
		}
		return NULL;
	}

	void _InOrder(Node* root)
	{
		//节点为空，返回
		if (root == NULL)
			return;

		//如果节点没有被搜索，那么递归
		if (root->_leftTag == LINK)
			_InOrder(root->_left);
		cout << root->_data << " ";
		if (root->_rightTag == LINK)
			_InOrder(root->_right);
	}

	//二叉树的中序搜索化
	void _InOrderThreadding(Node* cur, Node*& prev)
	{
		if (cur == NULL)
			return;
		
		//中序遍历，先进行递归，找最左边的节点
		_InOrderThreadding(cur->_left, prev);

		//左孩子为空
		if (cur->_left == NULL)
		{
			cur->_left = prev;//将上一个节点赋给左孩子
			cur->_leftTag = THREDA;//更改标记为已搜索
		}

		//判断上一个节点的右孩子，如果存在且为空
		if (prev && prev->_right == NULL)
		{
			prev->_right = cur;//上一个节点的右孩子指向自己
			prev->_rightTag = THREDA;//更改标记
		}

		//这一步要放在循环的外面
		prev = cur;

		//最后访问右孩子
		_InOrderThreadding(cur->_right, prev);
	}
protected:
	//定义根节点
	Node* _root;
};

void TestBinaryTreeThd()
{
	int arr[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	BinaryTreeThd<int> t(arr, sizeof(arr) / sizeof(arr[0]),'#');

	t.InOrder();
	t.InOrderThreadding();

	t.InOrder();
	t.InOrderThd();

	BinaryTreeThd<int>::Iterator it = t.Begin();
	while (it != t.End())
	{
		cout << *it << " ";
		it++;
	}
	/*size_t count = 0;
	while (it != t.End() && count <3)
	{
		it++;
	}
	while (it != NULL)
	{
		cout << *it << " ";
		--it;
	}*/
}
