/*
* author:haohaosong
* dete:2016/12/14
*/ 

#pragma once 

#include<assert.h>
#include<queue>
template<typename T>
struct BinaryTreeNode
{
	T _data;
	BinaryTreeNode<T> *_left;
	BinaryTreeNode<T> *_right;

	BinaryTreeNode(const T& x)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<typename T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree()
		:_root(NULL)
	{}
	BinaryTree(T* arr, const size_t size,const T& invalid = T())
	{
		assert(arr);
		size_t index = 0;
		_root = CreateTree(arr, size, index, invalid);
	}
	BinaryTree(const BinaryTree& b)
	{
		_root = Copy(b._root);
	}
	BinaryTree&operator=(BinaryTree t)
	{
		if (this != &t)
		{
			std::swap(t._root, _root);
		}
		return *this;
	}
	~BinaryTree()
	{
		if (_root != NULL)
		{
			Destory(_root);
			_root = NULL;
		}
	}
	void PrevOrder()
	{
		_PrevOrder(_root);
		cout << endl;
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
	void PostOrder()
	{
		_PostOrder(_root);
		cout << endl;
	}
	void LevelOrder()
	{
		if (_root == NULL)
			return;
		queue<Node*> q;
		q.push(_root);
		while (q.empty() == false)
		{
			Node* tmp = q.front();
			cout << tmp->_data << " ";
			q.pop();
			if (tmp->_left != NULL)
				q.push(tmp->_left);
			if (tmp->_right != NULL)
				q.push(tmp->_right);
		}
		cout << endl;
	}
	size_t Size()
	{
		return _Size(_root);
	}
	size_t Depth()
	{
		return _Depth(_root);
	}
	size_t GetLeafSize()
	{
		size_t count = 0;
		_GetLeafSize(_root,count);
		return count;
	}
	size_t GetKLevelSize(size_t k)
	{
		assert(k > 0);
		return _GetKLevelSize(_root,k);
	}
protected:
	Node* CreateTree(T* arr,const size_t size, size_t& index,const T& invalid = T())
	{
		if (index < size && arr[index]!=invalid)
		{
			Node* root = new Node(arr[index]);
			root->_left = CreateTree(arr,size,++index,invalid);
			root->_right = CreateTree(arr, size, ++index, invalid);
			return root;
		}
		return NULL;
	}
	void Destory(Node* root)
	{
		assert(root);
		if (root->_left != NULL)
			Destory(root->_left);
		root->_left = NULL;
		if (root->_right != NULL)
			Destory(root->_right);
		root->_right = NULL;

		delete[] root;
		root = NULL;
		return;
	}
	Node* Copy(Node* root)
	{
		if (root == NULL)
			return NULL;
		Node* newnode = new Node(root->_data);
		newnode->_left = Copy(root->_left);
		newnode->_right = Copy(root->_right);
		return newnode;
	}
	void _PrevOrder(Node* root)
	{
		if (root == NULL)
			return;
		cout << root->_data << " ";
		_PrevOrder(root->_left);
		_PrevOrder(root->_right);
	}
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;
		_InOrder(root->_left);
		cout << root->_data << " ";
		_InOrder(root->_right);
	}
	void _PostOrder(Node* root)
	{
		if (root == NULL)
			return;
		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_data << " ";
	}
	size_t _Size(Node* root)
	{
		if (root == NULL)
			return 0;
		return _Size(root->_left) + _Size(root->_right) + 1;
	}
	size_t _Depth(Node* root)
	{
		if (root == NULL)
			return 0;
		size_t leftDepth = _Depth(root->_left);
		size_t rightDepth = _Depth(root->_right);

		return leftDepth > rightDepth ? leftDepth + 1: rightDepth + 1;
	}
	void _GetLeafSize(Node* root,size_t &count)
	{
		if (root->_left == NULL && root->_right == NULL)
		{
			count++;
			return;
		}
		if (root->_left != NULL)
			_GetLeafSize(root->_left,count);
		if (root->_right!=NULL)
			_GetLeafSize(root->_right,count);
	}
	size_t _GetKLevelSize(Node* root ,size_t k)
	{
		if (root == NULL)
			return 0;
		if (k == 1)
			return 1;

		return _GetKLevelSize(root->_left,k-1) + _GetKLevelSize(root->_right,k-1);
	}
protected:
	Node* _root;
};

void TestBinaryTree()
{	
	int arr[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	//int arr[] = { 1, 2, '#', 3, '#','#', 4,5, '#', 6 ,'#', 7,'#','#',8};
	BinaryTree<int> t1(arr,sizeof(arr)/sizeof(arr[0]),'#');
	t1.InOrder();
	t1.PostOrder();
	t1.PrevOrder();

	t1.LevelOrder();
	cout << t1.Size() << endl;
	cout << t1.Depth() << endl;
	cout << t1.GetLeafSize() << endl;
	cout << t1.GetKLevelSize(2) << endl;
	
	BinaryTree<int> t2(t);
	t2.LevelOrder();
	
	BinaryTree<int> t3;
	t3 = t;
	t3.PrevOrder();
}
