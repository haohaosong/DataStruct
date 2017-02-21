#pragma once

#include<iostream>
using namespace std;

//包我们自己定义的位图头文件
#include"BitMap.h"

//定义五个字符串哈希算法
template<class T>
struct BKDRHash
{
	size_t operator()(const T str)
	{
		register size_t hash = 0;
		for (size_t i = 0; i < str.size(); ++i)
		{
			hash = hash * 131 + str[i];
		}
		return hash;
	}
};

template<class T>
struct SDBMHash
{
	size_t operator()(const T str)
	{
		register size_t hash = 0;
		for (size_t i = 0; i < str.size(); ++i)
		{
			hash = 65599 * hash + str[i];
		}
		return hash;
	}
};

template<class T>
struct RSHash
{
	size_t operator()(const T str)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		for (size_t i = 0; i < str.size(); ++i)
		{
			hash = hash * magic + str[i];
			magic *= 378551;
		}
		return hash;
	}
};

template<class T>
struct APHash
{
	size_t operator()(const T str)
	{
		register size_t hash = 0;
		for (long i = 0; i < str.size(); ++i)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ str[i] ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ str[i] ^ (hash >> 5)));
			}
		}
		return hash;
	}
};

template<class T>
struct JSHash
{
	size_t operator()(const T str)
	{
		if (str.empty())
			return 0;
		register size_t hash = 1315423911;
		for (size_t i = 0; i < str.size(); ++i)
		{
			hash ^= ((hash << 5) + str[i] + (hash >> 2));
		}
		return hash;
	}
};

//定义布隆过滤器，5个HashFunc是五种不同的字符串哈希算法，用来辅助实现布隆过滤器
template<typename K = string,
typename HashFunc1 = BKDRHash<K>,
typename HashFunc2 = SDBMHash<K>,
typename HashFunc3 = RSHash<K>,
typename HashFunc4 = APHash<K>,
typename HashFunc5 = JSHash<K>>
class BoolmFilter
{
public:
	BoolmFilter(size_t num)
		:_bp(num*2*5)
	{}

	size_t HashFunC1(const K& num)
	{
		HashFunc1 hf;
		size_t index = hf(num);
		return index;
	}
	size_t HashFunC2(const K& num)
	{
		HashFunc2 hf;
		return hf(num);
	}
	size_t HashFunC3(const K& num)
	{
		HashFunc3 hf;
		return hf(num);
	}
	size_t HashFunC4(const K& num)
	{
		HashFunc4 hf;
		return hf(num);
	}
	size_t HashFunC5(const K& num)
	{
		HashFunc5 hf;
		return hf(num);
	}
	void Set(const K &num)
	{
		//用五中不同的字符串哈希算法求出五个不同的值
		size_t hf1 = HashFunC1(num);
		size_t hf2 = HashFunC2(num);
		size_t hf3 = HashFunC3(num);
		size_t hf4 = HashFunC4(num);
		size_t hf5 = HashFunC5(num);

		//测试五个哈希值
		//cout << hf1 << " " << hf2 << " " << hf3 << " " << hf4 << " " << hf5 << endl;

		//将五个值都放入位图中
		_bp.Set(hf1);
		_bp.Set(hf2);
		_bp.Set(hf3);
		_bp.Set(hf4);
		_bp.Set(hf5);
	}

	//Reset可以用引用计数来实现
	//void Reset();

	bool Find(K &num)
	{
		//分别判断num对应的五个值是否存在于位图中
		//若有一个不存在，则返回FALSE，num必定不存在
		//若都存在，则返回TRUE，但是不能确定一定存在
		int hf1 = HashFunC1(num);
		if(_bp.Find(hf1) == false)
			return false;

		int hf2 = HashFunC2(num);
		if (_bp.Find(hf2) == false)
			return false;

		int hf3 = HashFunC3(num);
		if (_bp.Find(hf3) == false)
			return false;

		int hf4 = HashFunC4(num);
		if (_bp.Find(hf4) == false)
			return false;

		int hf5 = HashFunC5(num);
		if (_bp.Find(hf5) == false)
			return false;

		return true;
	}
protected:
	//定义位图 _bp;
	BitMap _bp;
};
