#pragma once

/*
* author:haohaosong
* date:2017/2/17
* note:pair的模拟实现 
*/

//pair的模拟实现
template<typename T1,typename T2>
struct Pair
{
	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;

	Pair()
		:first_type(T1())
		, second_type(T2())
	{}

	Pair(const T1 &x, const T2 &y)
		:first_type(x)
		, second_type(y)
	{}

	//拷贝构造函数
	template<typename U,typename V>
	Pair(const Pair<U, V> &p)
		: first(p.first)
		, second(p.second)
	{}
}
