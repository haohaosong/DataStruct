/*
* author:haohaosong
* date:2017/1/8
* note:搜索二叉树的实现，需要判断是否删除为根节点的情况 
*/

#pragma once 
#include<assert.h> 
#include<iostream>
using namespace std;

template<typename K,typename V>
struct ResearchBinaryTreeNode
{
	ResearchBinaryTreeNode<K, V>* _left;
	ResearchBinaryTreeNode<K, V>* _right;

	K _key;
	V _value;

	ResearchBinaryTreeNode(const K& key,const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<typename K,typename V>
class ResearchBinaryTree
{
	typedef ResearchBinaryTreeNode<K,typename V> Node;
public:
	ResearchBinaryTree()
		:_root(NULL)
	{}

	~ResearchBinaryTree()
	{
		Destory(_root);
		_root = NULL; 
	}

	bool Insert(const K& key,const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key，value);
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
			parent->_left = new Node(key,value);
		else if (parent->_key<key)
			parent->_right = new Node(key,value);

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
	
	Node* FindR(const K& key)
	{
		return _FindR(_root, key);
	}

	bool InsertR(const K& key, const V& value)
	{
		return _InsertR(_root, key, value);
	}

	bool RemoveR(const K&key)
	{
		return _RemoveR(_root, key);
	}
protected:
	bool _RemoveR(Node* root,const K& key)
	{
		if (root == NULL)
			return false;

		//递归，找到要删除的节点
		if (root->_key < key)
			return _RemoveR(root->_right, key);
		else if (root->_key > key)
			return _RemoveR(root->_left, key);
		else
		{
			Node* delNode = root;

			//删除节点的左为空
			if (root->_left == NULL)
				root = root->_right; 
			else if (root->_right == NULL)
				root = root->_left;
			else//左右都不为空的情况
			{
				Node* parent = root;
				Node* subLeft = root->_right;

				while (subLeft->_left)
				{
					parent = subLeft;
					subLeft = subLeft->_left;
				}

				delNode = subLeft;

				//若为左子树，代表走了while循环，否则没有走循环
				//要删除的节点是subLeft

				root->_key = subLeft->_key;

				if (parent->_left == subLeft)
					parent->_left = subLeft->_right;
				else
					parent->_right = subLeft->_right;

				delete delNode;
				return true;
			}
		}
	}

	bool _InsertR(Node*& root, const K& key, const V& value)
	{
		//构建新节点
		if (root == NULL)
		{
			root = new Node(key, value);
			return true;
		}
		
		if (key < root->_key)
			return _InsertR(root->_left, key, value);
		else if (key > root->_key)
			return _InsertR(root->_right, key, value);
		else
			return false;
	}

	Node* _FindR(Node* root, const K& key)
	{
		if (root == NULL)
			return NULL;

		if (key < root->_key)
			return _FindR(root->_left, key);
		else if (key>root->_key)
			return _FindR(root->_right, key);
		else
			return root;
	}
	
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
