/*
* author:haohaosong
* date:2017/1/8
* note:搜索二叉树的实现，需要判断是否删除为根节点的情况 
*/

#pragma once 

#include<iostream>
using namespace std;

template<typename T>
struct ResearchBinaryTreeNode
{
	ResearchBinaryTreeNode<T>* _left;
	ResearchBinaryTreeNode<T>* _right;
	T _key;

	ResearchBinaryTreeNode(const T& key)
		:_key(key)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<typename K>
class ResearchBinaryTree
{
	typedef ResearchBinaryTreeNode<K> Node;
public:
	ResearchBinaryTree()
		:_root(NULL)
	{}

	~ResearchBinaryTree()
	{
		Destory(_root);
	}

	bool Insert(const K& key)
	{
		if (_root == NULL)
		{
			_root = new Node(key);
			return true;
		}

		Node* cur = _root;
		Node* parent = cur;

		//找到需要插入节点的父亲节点
		while (cur)
		{
			parent = cur;

			if (cur->_key < key)
				cur = cur->_right;
			else if (cur->_key>key)
				cur = cur->_left;
			else
				return false; 
		}

		//parent为需要插入节点的父亲节点
		if (parent->_key > key)
			parent->_left = new Node(key);
		else if (parent->_key<key)
			parent->_right = new Node(key);

		return true;
	}

	Node* Find(const K& key)
	{
		Node* cur = _root; 
		
		//根据搜索二叉树的特点来进行查找
		while (cur)
		{
			if (key < cur->_key)
				cur = cur->_left;
			else if (key>cur->_key)
				cur = cur->_right;
			else
				return cur;
		}

		return NULL;
	}

	bool Remove(const K& key)
	{
		Node* cur = _root;
		Node* parent = NULL;
		Node* delNode = NULL;

		//找出要删除的节点以及其父亲节点
		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key >cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				break;
			}
		}
	
		if (cur == NULL)
			return false;

		//如果删除的是根节点，那么parent的值为NULL 
		//cur此时是要删除的节点
		if (cur->_left == NULL)
		{
			delNode = cur;

			//cur是父亲节点的左孩子的话，就把cur的右孩子赋给父亲节点的左孩子
			//否则，将cur的右孩子赋给父亲节点的右孩子
			if (parent == NULL)
				_root = cur->_right;
			else if (parent->_left == cur)
				parent->_left = cur->_right;
			else
				parent->_right = cur->_right;
		}
		else if (cur->_right == NULL)
		{
			delNode = cur;

			//cur是父亲节点的左孩子的话，就把cur的右孩子赋给父亲节点的左孩子
			//否则，将cur的右孩子赋给父亲节点的右孩子
			if (parent == NULL)
				_root = cur->_left;

			else if(parent->_left == cur)	
				parent->_left = cur->_left;
			else
				parent->_right = cur->_left;
		}
		else
		{
			//都不为空的情况，需要采用替换法来解决
			Node* subLeft = NULL;//定义右子树的最左节点

			//循环找到右子树的最左节点
			//这里subLeft不可能为空
			subLeft = cur->_right;
			parent = cur;
			while (subLeft->_left)
			{
				parent = subLeft;
				subLeft = subLeft->_left;
			}

			cur->_key = subLeft->_key;

			if (parent->_left == subLeft)
				parent->_left = subLeft->_right;
			else
				parent->_right = subLeft->_right;

			delNode = subLeft;
		}

		delete delNode;
		delNode = NULL;

		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
protected:
	void Destory(Node* root)
	{
		if (root == NULL)
			return;
		Destory(root->_left);
		Destory(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
protected:
	Node* _root;
};
