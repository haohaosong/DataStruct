#pragma once 

/*
* author:haohaosong
* date:2017/3/14
* note:图的实现以及深度广度优先遍历
*/

#include<iostream>
using namespace std;

#include<vector>
#include<assert.h>
#include<queue>
#include<string>
#include<set>
//图的两种实现方式:邻接表，邻接矩阵
//方法1:邻接矩阵

template<typename V,typename W>
class GraphMatrix
{
public:
	GraphMatrix(V* vertexs, size_t n, const W& invalid = W(), bool IsDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(IsDirected)
	{
		_matrix = new W*[n];
		for (size_t i = 0; i < n; ++i)
		{
			_matrix[i] = new W[n];
			for (size_t j = 0; j < n; ++j)
			{
				_matrix[i][j] = invalid;
			}
		}
	}

	~GraphMatrix()
	{}

	int GetIndex(const V& v)
	{
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			if (_vertexs[i] == v)
				return i;
		}
		assert(false);
		return -1;
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = GetIndex(v1);
		size_t des = GetIndex(v2);
		_matrix[src][des] = w;

		if (_isDirected == false)
			_matrix[des][src] = w;
	}
protected:
	vector<V> _vertexs;//定点的集合
	W** _matrix;//邻接矩阵边的集合
	bool _isDirected;//是否是有向图
};

void TestGraphMatrix()
{
	string city[] = { "北京", "上海", "广州", "杭州", "西安" };
	GraphMatrix<string, double> gpm(city, sizeof(city) / sizeof(city[0]));
	gpm.AddEdge("北京", "上海", 300.3);
	gpm.AddEdge("北京", "广州", 850.5);
	gpm.AddEdge("北京", "杭州", 299);
	gpm.AddEdge("北京", "西安", 475);
}

//方法2:邻接表
template<typename W>
struct LinkNode
{
	W _weight;
	LinkNode<W> *_next;
	size_t _src;
	size_t _des;

	LinkNode(size_t src,size_t des,const W& weight)
		:_weight(weight)
		, _next(NULL)
		, _src(src)
		, _des(des)
	{}
};

template<typename V,typename W>
class GraphLink
{
	typedef LinkNode<W> Node;
public:
	GraphLink(V* vertexs, size_t n, bool IsDirected = false)
	{
		_vertexs.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexs[i] = vertexs[i];
		}
		_linkTables.resize(n, NULL);
	}

	~GraphLink()
	{}

	int GetIndex(const V& v)
	{
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			if (v == _vertexs[i])
				return i;
		}
		assert(false); 
		return -1;
	}

	V& GetV(const size_t index)
	{
		assert(index < _vertexs.size());
		return _vertexs[index];
	}

	void _AddEgde(const size_t& src, const size_t& des, const W&w)
	{
		//进行头插
		Node* newNode = new Node(src,des, w);
		newNode->_next = _linkTables[src];
		_linkTables[src] = newNode;
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = GetIndex(v1);
		size_t des = GetIndex(v2);
		_AddEgde(src, des, w);

		if (_isDirected == false)
			_AddEgde(des, src, w);
	}

	void GFS(const V& src)//广度优先遍历
	{
		//利用队列来进行广度优先遍历
		//利用set来快速判断一个顶点是否被访问过
		queue<size_t> q;
		q.push(GetIndex(src));
		set<size_t> s;
		s.insert(GetIndex(src));
		while (!q.empty())
		{
			size_t tmp = q.front();
			cout << GetV(tmp) << " ";
			q.pop();
			Node* cur = _linkTables[tmp];

			//遍历一个节点的邻接表
			//将没有访问过的节点放入队列和set中
			while (cur)
			{
				//根据insert的返回值来判断是否顶点已经被访问过
				//如果没有被访问过，则插入成功，并放入队列中
				pair<set<size_t>::iterator, bool> ret = s.insert(cur->_des);
				if (ret.second == true)
					q.push(*ret.first);

				cur = cur->_next;
			}
		}
		cout << endl;
	}
	void DFS(const V& src)//深度优先遍历
	{
		size_t isrc = GetIndex(src);
		set<size_t> s;
		s.insert(isrc);
		_DFS(isrc, s);
		cout << endl;
	}
protected:
	void _DFS(const size_t src, set<size_t> &s)
	{
		Node* cur = _linkTables[src];
		cout << GetV(src) << " ";
		while (cur)
		{
			//通过判断pair的返回值判断一个顶点是否访问过
			//若没访问过，则递归访问
			pair<set<size_t>::iterator, bool> ret = s.insert(cur->_des);
			if (ret.second == true)
				_DFS(cur->_des, s);

			cur = cur->_next;
		}
	}
protected:
	vector<V> _vertexs;//顶点的集合，用来存储顶点的值
	vector<Node*> _linkTables;//边的集合
	bool _isDirected;//是否是有向图
};

void TestGraphLink()
{
	string city[] = { "北京", "上海", "广州", "杭州", "西安" };
	GraphLink<string, double> gpm(city, sizeof(city) / sizeof(city[0]));
	gpm.AddEdge("北京", "上海", 300.3);
	gpm.AddEdge("北京", "广州", 850.5);
	gpm.AddEdge("北京", "杭州", 299);
	gpm.AddEdge("北京", "西安", 475);
	gpm.AddEdge("上海", "西安", 475);

	gpm.GFS("西安");
	gpm.DFS("北京");
}