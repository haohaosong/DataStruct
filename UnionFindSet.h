#pragma once

/*
* author:haohaosong
* date:2017/3/5
* note:并查集的实现，用于解决朋友圈问题
*/

#include<iostream>
using namespace std;

#include<vector>

//定义并查集
class UnionFindSet
{
public:
	//构造函数，将初始值置为-1
	//并进行扩容
	UnionFindSet(size_t n)
	{
		v.resize(n+1, -1);
	}

	//找到根节点
	size_t FindRoot(size_t x)
	{
		size_t ret = x;

		while (v[ret] >= 0)
			ret = v[ret];

		return ret;
	}

	//将两个人的朋友圈进行合并
	void Union(size_t x1,size_t x2)
	{
		size_t root1 = FindRoot(x1);
		size_t root2 = FindRoot(x2);

		//同根，已经在一个集合
		if (root1 == root2)
			return;

		v[root1] += v[root2];
		v[root2] = root1;
	}

	//判断是否在一个集合
	bool IsInSet(int x1, int x2)
	{
		return FindRoot(x1) == FindRoot(x2);
	}

	//求集合(朋友圈)的个数
	size_t SetSize()
	{
		size_t count = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i] < 0)
				count++;
		}

		return count-1;
	}
protected:
	vector<int> v;
};

void TestUnionFindSet()
{
	//五个人，有四个朋友圈关系
	const int n = 5;
	const int m = 4;

	UnionFindSet ufs(n);
	int r[m][2] = { { 1, 2 }, { 2, 3 }, { 4, 5 }, { 2, 4 } };
	for (size_t i = 0; i < m; ++i)
	{
		ufs.Union(r[i][0], r[i][1]);
	}

	cout << "朋友圈的个数为:" << ufs.SetSize() << endl;
}