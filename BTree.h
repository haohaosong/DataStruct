#pragma once

#include<iostream>
using namespace std;

/*
* author:haohaosong
* date:2017/2/23
* note:B树的实现 
*/

//B树的增删查改的时间复杂度 O(logM(N))

//定义B树的节点
template<typename K,typename V,size_t M = 3>
struct BTreeNode
{
	pair<K,V> _kv[M];
	
	BTreeNode<K, V> *_subs[M+1];
	BTreeNode<K, V> *_parent;

	//表示存储了多少个Key值
	size_t _size;

	//节点的构造函数
	BTreeNode()
		:_size(0)
		, _parent(NULL)
	{
		for (size_t i = 0; i <= M; ++i)
		{
			_subs[i] = NULL;
		}
	}
};

//B树的定义
template<typename K, typename V,size_t M = 3>
class BTree
{
	typedef BTreeNode<K, V> Node;
public:
	//构造函数
	BTree()
		:_root(NULL)
	{}

	//查找函数，其返回值的设计实现了Insert的复用
	pair<Node*,int> Find(const K& key)
	{
		//若查询不到，则用parent记录需要插入的节点的父节点
		//实现了Insert的复用
		Node* cur = _root;
		Node* parent = NULL;

		while (cur)
		{
			size_t index = 0;
			while (index < cur->_size)
			{
				if (key > cur->_kv[index].first)
				{
					index++;
				}
				else if (key == cur->_kv[index].first)
				{
					//查询到，返回查询的节点
					return make_pair(cur, index);
				}
				else
				{
					break;
				}
			}
			parent = cur;
			cur = cur->_subs[index];
		}

		//没有查询到,返回需要插入节点的父亲节点
		//实现了Insert函数的复用
		return make_pair(parent, -1);
	}

	bool Insert(const pair<K, V>& kv)
	{
		//当根节点为空的时候，需要对_root进行赋值
		if (_root == NULL)
		{
			_root = new Node;
			_root->_size = 1;
			_root->_kv[0] = kv;
			return true;
		}

		//不为空，则找到插入的位置
		//如果second的值不为-1，则表示已经存在，不需要插入
		pair<Node*, int> ret = Find(kv.first); 
		if (ret.second != -1)
			return false;

		//进行插入
		Node* cur = ret.first;
		Node* temp = cur;
		pair<K, V> newKV = kv;
		Node* sub = NULL;

		while (1)
		{
			//在cur节点插入kv和sub
			_Insert(cur, newKV,sub);

			//如果cur没满，则表示插入成功
			//否则需要进行分裂
			if (cur->_size < M)
				return true;

			//构造一个新节点tmp
			Node* tmp = new Node;
			size_t mid = M >> 1;
			size_t i = mid+1;
			size_t j = 0;

			//将右半区间的元素移到tmp中
			while (i < cur->_size)
			{
				tmp->_kv[j] = cur->_kv[i];
				cur->_kv[i] = pair<K, V>();

				tmp->_subs[j] = cur->_subs[i];
				if (cur->_subs[i])
					cur->_subs[i]->_parent = tmp;

				//调整
				i++;
				j++;
				tmp->_size++;
				cur->_size--;
			}

			//拷走最后一个右孩子
			tmp->_subs[j] = cur->_subs[i];
			cur->_subs[i] = NULL;
			if (cur->_subs[i])
				cur->_subs[i]->_parent = tmp;

			//如果cur没有父亲节点，需要创建
			//如果cur有父亲节点，那么需要将中间元素插入到父亲节点里
			if (cur->_parent)
			{
				//继续向上调整，判别是否parent会满
				newKV = cur->_kv[mid];
				sub = tmp;
				cur->_size--;
				cur = cur->_parent;
			}
			else
			{
				//定义一个新根节点，将中间元素放入该节点中
				Node* newRoot = new Node;
				newRoot->_kv[0] = cur->_kv[mid];
				newRoot->_size = 1;

				//设置新根节点的两个孩子cur和tmp
				//并修改他们的父亲节点
				newRoot->_subs[0] = cur;
				cur->_parent = newRoot;
				newRoot->_subs[1] = tmp;
				tmp->_parent = newRoot;

				cur->_size--;

				//将根节点重新赋值
				_root = newRoot;
				return true;
			}
		}
	}

	//中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
protected:
	void _Insert(Node* cur,const pair<K,V>& newKV,Node* sub)
	{
		//这里要用int，不能用size_t，会影响while循环的判断条件
		int index = (cur->_size)-1;
		while (index >= 0)
		{
			//找到合适的插入位置，跳出
			if (newKV.first > cur->_kv[index].first)
				break;

			//将key值向后移动，并且将孩子也移动
			cur->_kv[index + 1] = cur->_kv[index];
			cur->_subs[index + 2] = cur->_subs[index + 1];
			index--;
		}

		//插入新的KV以及孩子
		cur->_kv[index+1] = newKV;
		cur->_subs[index+2] = sub;

		//孩子存在，给其父亲赋值
		if (sub)
			sub->_parent = cur;

		cur->_size++;
	}

	//中序遍历递归函数
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		//访问每一个key值及其左孩子
		size_t index = 0;
		while (index < root->_size)
		{
			_InOrder(root->_subs[index]);
			cout << root->_kv[index].first<<" ";
			index++;
		}

		//打印最后一个key值的右孩子
		_InOrder(root->_subs[root->_size]);
	}
protected:
	Node* _root;
};

void TestBtree()
{
	BTree<int, int> t;
	int a[] = { 53, 75, 139, 49, 145, 36, 101 };
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(make_pair(a[i], i));
		t.InOrder();
	}

	t.InOrder();
}
