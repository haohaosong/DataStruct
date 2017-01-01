/*
* author:haohaosong
* date:2017/1/1
* note:哈弗曼树 
*/ 

#pragma once

#include<iostream>
using namespace std;
#include"heap.h"

template<typename T>
struct HuffmanTreeNode
{
	typedef HuffmanTreeNode<T> Node;

	T _weight;    //权重

	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;//三叉链，便于寻找父亲节点 

	HuffmanTreeNode(const T& x)
		:_left(0)
		,_right(0)
		,_parent(0)
		, _weight(x)
	{}

	//重载+
	Node operator + (const Node& t)
	{
		return new Node(_weight + t._weight);
	}
};

template<typename T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	{}

	HuffmanTree(T* arr, size_t n, T& invalid)
	{
		//定义仿函数，用于huffman树节点的比较 
		struct compare
		{
			bool operator()(Node* l,Node* r)
			{
				return l->_weight < r->_weight;
			}
		};

		//将数组初始化成一个堆
		Heap<Node*,compare> hp; 
		for (size_t i = 0; i<n; ++i)
		{
			if (arr[i] != invalid)
			{
				hp.Push(new Node(arr[i]));
			}
		}

		//进行哈弗曼树的构建，堆顶元素是最小的 
		while (hp.Size()>1)
		{
			Node* left = hp.Top();
			hp.Pop();
			Node* right = hp.Top();
			hp.Pop();
			Node* parent = new Node(left->_weight + right->_weight);

			parent->_left = left;
			parent->_right = right;
			left->_parent = parent; 
			right->_parent = parent;

			hp.Push(parent);
		}
		_root = hp.Top();
	}

	~HuffmanTree()
	{
		Destory(_root);
		_root = NULL;
	}
protected:
	Node* _root;

	void Destory(Node* root)
	{
		if (root == NULL)
			return;

		Destory(root->_left);
		Destory(root->_right);

		delete root;
		root = NULL;
	}
	//防拷贝 
	HuffmanTree(const HuffmanTree<T>& t);
	HuffmanTree& operator=(const HuffmanTree<T>& t);
};

void TestHuffman()
{
	int arr[5] = { 1, 2, 3, 4 };
	HuffmanTree<int> huff(arr,4,0);
}
