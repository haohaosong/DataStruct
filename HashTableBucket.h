#pragma once 

/*
* author:haohaosong
* date:2017/2/19
* note:哈希桶的实现 
*/

#include<iostream>
using namespace std;

#include<vector>

//定义哈希桶的一个节点
template<typename K,typename V>
struct HashTableBucketNode
{
	K _key;
	V _value;

	HashTableBucketNode<K, V>* _next;

	HashTableBucketNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _next(NULL)
	{}
};

//使用素数表的对齐作为哈希表的容量，可以降低哈希冲突 
const int _PrimeSize = 28;
static const unsigned long _PrimeList[_PrimeSize] =
{
	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul,
	786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul,
	25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul,
	805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

//定义仿函数，求一个类型的key值
template<typename K>
struct __HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

//string的特化版本
template<>
struct __HashFunc<string>
{
	size_t operator()(const string& s)
	{
		string::const_iterator it = s.begin();
		size_t ret = 0;
		while (it != s.end())
		{
			ret += (*it + 131);
		}
		return ret;
	}
};

//定义哈希桶
template<typename K, typename V, typename HashFunC = __HashFunc<K>>
class HashTableBucket
{
	typedef HashTableBucketNode<K, V> Node;
public:
	//构造函数
	HashTableBucket(size_t size)
		:_size(0)
	{
		size_t newsize = GetNextPrime(size);
		_v.resize(newsize);
	}

	//获取哈希值
	size_t HashFunc(const K& key)
	{
		HashFunC hf;
		size_t index = hf(key);
		return index % _v.size();
	}

	//插入函数，返回一个结构体类型
	pair<Node*,bool> Insert(const K& key,const V& value)
	{
		CheckCapacity();
		size_t index = HashFunc(key);

		//如果没有链
		if (!_v[index])
		{
			Node* newNode = new Node(key, value);
			_v[index] = newNode;
			++_size;
			return make_pair(newNode, true);
		}
		
		Node* prev = NULL;
		Node* cur = _v[index];
		while (cur)
		{
			//表示已经存在
			if (cur->_key == key)
				return make_pair(cur, false);

			prev = cur;
			cur = cur->_next;
		}
		Node* newNode = new Node(key, value);
		prev->_next = newNode;
		++_size;
		return make_pair(newNode, true);
	}

	Node* find(const K& key)
	{
		//遍历该桶
		for (size_t index = 0; index < _size; ++index)
		{
			Node* cur = _v[index];

			//遍历每一条以_v[index]为头的链表
			while (cur)
			{
				//找到该节点，返回
				if (cur->_key == key)
					return cur;

				cur = cur->_next;
			}
		}

		//没有查询到
		return NULL;
	}

	bool Erase(const K& key)
	{
		//找出key所在的链表位置
		size_t index = HashFunc(key);

		//遍历以该_v[index]为头的链表
		Node* cur = _v[index];//需要删除的节点
		Node* prev = NULL;//删除节点的前一个节点

		while (cur)
		{
			if (cur->_key == key)
			{
				if (prev == NULL)
					_v[index] = NULL;
				else
					prev->_next = cur->_next;

				delete cur;
				return true;
			}
			prev = cur;
			cur = cur->_next;
		}

		return false;
	}

protected:
	vector<Node*> _v;
	size_t _size;
protected:
	//交换两个哈希桶的内容
	void Swap(HashTableBucket<K,V> & tmp)
	{
		std::swap(_v, tmp._v);
		std::swap(_size, tmp._size);
	}

	size_t GetNextPrime(size_t size)
	{
		for (size_t i = 0; i < _PrimeSize; ++i)
		{
			//找到下一个素数
			if (_PrimeList[i] > size)
			{
				return _PrimeList[i];
			}
		}

		return _PrimeList[_PrimeSize-1];
	}


	//检查是否需要扩容
	void CheckCapacity()
	{
		//此时系数为1，达到1的时候扩容
		if (_size == _v.size())
		{
			HashTableBucket<K, V> tmp(_v.size());

			for (size_t index = 0; index < _size; ++index)
			{
				Node* cur = _v[index];
				while (cur)
				{
					tmp.Insert(cur->_key, cur->_value);
					cur = cur->_next;
				}
			}

			Swap(tmp);
		}
	}
};

void TestHashTableBucket()
{
	HashTableBucket<int, int> hb(4);
	hb.Insert(10, 0);
	hb.Insert(20, 0);
	hb.Insert(30, 0);
	hb.Insert(40, 0);
	hb.Insert(50, 0);
	hb.Insert(60, 0);
	hb.Insert(70, 0);

	hb.Erase(10);
}
