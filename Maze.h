/*
* author:haohaosong
* date:2016/11/30
* note:This head file need the file name with maze.txt.
*   You can made a new file or cpoy the file form my repository
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

void PrintMaze(int *maze,size_t N)
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j)
		{
			cout << maze[i*N + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool CheckIsAccess(int *maze, size_t N, Pos next)
{
	if (maze[next._row*N + next._col] == 0)
	{
		return true;
	}
	return false;
}

void GetMaze(int *maze, size_t N)
{
	FILE *fp = fopen("1.txt","r");
	if (fp == NULL)
	{
		throw invalid_argument("Read maze filed");
	}
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N;)
		{	
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

/*
* The method of not recursive
*/
bool GetMazePath(int *maze, size_t n, Pos entry, stack<Pos>&path)
{
	maze[entry._row*N + entry._col] = 2;
	Pos cur,next;
	cur = entry;
	path.push(entry);
	while (!path.empty())
	{
		cur = path.top();

		if (cur._row == N - 1)
		{
			return true;
		}

		next = cur;

		next._row += 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		next._row -= 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		next._col += 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		next._col -= 1;
		if (CheckIsAccess((int*)maze, N, next) == true)
		{
			path.push(next);
			maze[next._row*N + next._col] = 2;
			continue;
		}
		next = cur;

		path.pop();
	}
	return false;
}

/*
* The method of recursive
*/
void GetMazePath_R(int *maze, size_t N, Pos entry, stack<Pos>&path)
{
	if (entry._row == N - 1)
	{
		return;
	}
	Pos cur,next;
	cur = entry;

	next = cur;

	next._row += 1;
	if (CheckIsAccess((int*)maze,N,next) ==  true)
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
	}
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

/*
* Reset the maze 
*/
void ReMaze(int* maze，size_t N)
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

void TestMaze()
{
	try
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
			ReMaze((int*)maze,N);
			maze[path.top()._row][path.top()._col] = 1;//将上次的出口改为1
			path = empty;
		}
		cout << "Min path:" << MinPath.size() << endl;
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
}

