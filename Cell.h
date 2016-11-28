/*
* author:haohaosong
* date:2016/11/27
*/ 

#include<iostream>
using namespace std;

#include"Stack.h"

enum Type
{
	OP_SYMBOL,
	OP_NUM,
	ADD,
	SUB,
	MUL,
	DIV,
};

struct Cell
{
	Type _type;
	int _value;
};

int RPN(Cell* cell, size_t n)
{
	Stack<int> s;
	for (size_t idx = 0; idx < n; ++idx)
	{
		if (cell[idx]._type == OP_NUM)
		{
			s.Push(cell[idx]._value);
		}
		else if (cell[idx]._type == OP_SYMBOL)
		{
			int right = s.Top();
			s.Pop();
			int left = s.Top();
			s.Pop();
			switch (cell[idx]._value)
			{
			case ADD:
				s.Push(left + right);
				break;
			case SUB:
				s.Push(left - right);
				break;
			case MUL:
				s.Push(left * right);
				break;
			case DIV:
				if (right == 0)
					throw invalid_argument("表达式错误");
				s.Push(left / right);
				break;
			default:
				throw invalid_argument("表达式错误");
				break;
			}
		}
		else
		{
			throw invalid_argument("表达式错误");
		}
	}
	return s.Top();
}

void FunTest()
{
	Cell cell[5] =
	{
		{ OP_NUM, 12 },
		{ OP_NUM, 3 },
		{ OP_NUM, 4 },
		{ OP_SYMBOL, ADD},
		{ OP_SYMBOL, MUL}
	};
	cout << "result：" <<RPN(cell, 5) << endl;
}
