/*
* author:haohaosong
* date:2016/11/30
* note:迷宫的实现，需要文件 maze.txt
* 可以自己实现 一个 maze.txt 也可以直接用本库提供的maze.txt
*/

#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
using namespace std;
#include<stack>
const int N = 10;

struct Pos
{
	size_t _row;//行
	size_t _col;//列
};

//打印当前的地图
void PrintMaze(int *maze,size_t N)
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j)
		{
			//通过偏移来求当前的元素
			cout << maze[i*N + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//判断一个点是否可以走
bool CheckIsAccess(int *maze, size_t N, Pos next)
{
	if (maze[next._row*N + next._col] == 0)
	{
		return true;
	}
	return false;
}

bool CheckIsAccess(int *maze, size_t N, Pos next, Pos cur)
{
	//越界，无法走
	if (next._row < 0 || next._row >= N || next._col < 0 || next._col >= N)
	{
		return false;
	}

	if (maze[next._row*N + next._col] == 0 )
	{
		return true;
	}
	return (maze[next._row*N + next._col] > (maze[cur._row*N + cur._col] + 1));
}

//打开maze.txt从而得到该地图
void GetMaze(int *maze, size_t N)
{
	FILE *fp = fopen("1.txt","r");
	if (fp == NULL)
	{
		//如果打开失败，抛出异常
		throw invalid_argument("Read maze filed");
	}
	
	//进行数组的初始化
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N;)
		{	
			//如果是0或1，初始化
			//如果出现其他字符，说明地图有误抛出异常
			char ch = fgetc(fp);
			if (ch == '0' || ch == '1')
			{	
				maze[i*N + j] = ch - '0';
				++j;
			}
			else if (ch == EOF)
			{
				throw invalid_argument("Maze Error");
			}
		}
	}
}

//得到迷宫的路径
bool GetMazePath(int *maze, size_t n, Pos entry, stack<Pos>&path)
{
	maze[entry._row*N + entry._col] = 2;
	Pos cur, next;
	cur = entry;
	path.push(entry);
	while (!path.empty())
	{
		cur = path.top();

		//如果到达边界，则找到了出口
		if (cur._row == N - 1)
		{
			return true;
		}

		next = cur;
		
		//向下走
		next._row += 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;
	
		//向上走
		next._row -= 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		//向右走
		next._col += 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		//向左走
		next._col -= 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;
	
		//如果上述都无法走，则返回上一个点
		//方法是弹出栈顶元素
		path.pop();
	}
	return false;
}

//递归得到迷宫的路径
void GetMazePath_R(int *maze, size_t N, Pos entry, stack<Pos>&path)
{
	if (entry._row == N - 1)
	{
		return;
	}
	Pos cur, next;
	cur = entry;

	next = cur;

	next._row += 1;
	if (CheckIsAccess((int*)maze, N, next) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = 2;
		GetMazePath_R(maze, N, next, path);
	}
	next = cur;

	next._row -= 1;
	if (CheckIsAccess((int*)maze, N, next) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = 2;
		GetMazePath_R(maze, N, next, path);
	}
	next = cur;

	next._col += 1;
	if (CheckIsAccess((int*)maze, N, next) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = 2;
		GetMazePath_R(maze, N, next, path);
		next = cur;

		next._col -= 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			GetMazePath_R(maze, N, next, path);
		}
		next = cur;
	}
}

//递归去找最佳的路径
void GetMazePath_R(int *maze, size_t N, Pos entry, stack<Pos>&path, stack<Pos>&shortpath)
{
	if (entry._row == N - 1)
	{
		if (path.size() < shortpath.size() || shortpath.size() == 0)
		{
			shortpath = path;
		}
		path.pop();
		return;
	}
	Pos cur, next;
	cur = entry;
	next = cur;

	next._row += 1;
	if (CheckIsAccess((int*)maze, N, next, cur) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = maze[cur._row*N + cur._col] + 1;
		GetMazePath_R(maze, N, next, path, shortpath);
	}
	next = cur;

	next._row -= 1;
	if (CheckIsAccess((int*)maze, N, next, cur) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = maze[cur._row*N + cur._col] + 1;
		GetMazePath_R(maze, N, next, path, shortpath);
	}
	next = cur;

	next._col += 1;
	if (CheckIsAccess((int*)maze, N, next, cur) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = maze[cur._row*N + cur._col] + 1;
		GetMazePath_R(maze, N, next, path, shortpath);
	}
	
	next = cur;
	next._col -= 1;
	if (CheckIsAccess((int*)maze, N, next, cur) == true)
	{
		path.push(next);
		maze[next._row*N + next._col] = maze[cur._row*N + cur._col] + 1;
		GetMazePath_R(maze, N, next, path, shortpath);
	}
	path.pop();
}

//进行地图的重置
void ReMaze(int* maze)
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j)
		{
			if (maze[i*N + j] != 1)
			{
				maze[i*N + j] = 0;
			}
		}
	}
}

/*
* note:这个方法有个小问题，挡出现环的时候无法处理
*   但是 void GetMazePath_R(int *maze, size_t N, Pos entry, stack<Pos>&path, stack<Pos>&shortpath) 可以处理这个问题
*/

//求最小路径
stack<Pos> MinPath()
{
	int maze[N][N] = { 0 };
	int curmaze[N][N] = { 0 }; 
	GetMaze((int*)maze, N);
	stack<Pos> path,MinPath,empty;
	
	while (GetMazePath((int*)maze, N, { 2, 0 }, path))
	{
		if (path.size() < MinPath.size()||MinPath.size() == 0)
		{
			MinPath = path;
		}
		
		//重置地图，求下一个解
		ReMaze((int*)maze);
		
		//将上次的出口改为1
		maze[path.top()._row][path.top()._col] = 1;
		
		path = empty;
	}
	return MinPath;
}

void TestMaze()
{
	//捕获异常，进行异常的处理
	try
	{
		int maze[N][N] = { 0 };
		int curmaze[N][N] = { 0 };
		GetMaze((int*)maze, N);
		stack<Pos> path, shortpath;
		path.push({2,0});
		GetMazePath_R((int*)maze, N, { 2, 0 },path, shortpath);
		cout << shortpath.size() << endl;
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
}
