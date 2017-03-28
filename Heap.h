/*
* author:haohaosong
* date:2016/12/25
* note:堆的实现 
*/ 
#include<iostream>
using namespace std;

#include<vector>

template<typename T>
class Heap
{
public:
	Heap()
	{}

	Heap(T arr[], size_t n)
		:_a(arr, arr + n)
	{
		//构建堆，需要先将子树先构建成堆
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		{
			//向下调整使该树成为堆
			_AdjustDown(i);
		}
	}

	void Pop()
	{
		//将根节点的值和最后一个值进行交换，再将最后一个元素不视为堆内的元素，进行向下调整
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}

	void Push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size() - 1);
	}
protected:
	vector<T> _a;
protected:
	//插入一个节点，该节点的加入可能破坏了堆的结构
	//向上调整是使加入节点后的树继续满足堆的条件
	void _AdjustUp(int i)
	{
		int child = i;
		int parent = (i - 1) / 2;

		//当子节点没有达到堆顶
		while (child >= 0)
		{
			//如果子比父亲大，则交换。并且进行下一轮的判断
			if (_a[child]>_a[parent])
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (parent - 1) / 2;
			}
			else//不用交换堆，直接结束
			{
				break;
			}
		}
	}

	//向下调整
	//在构建堆时，或者堆顶元素被pop时，需要向下调整
	//1、构建堆，先将最小的子树构建成堆，慢慢将堆生长
	//2、pop后，堆顶元素不满足堆的情况，但是其他子树是满足堆的，所以只需对堆顶元素向下调整
	void _AdjustDown(int root)
	{
		int parent = root;
		int child = 2 * parent + 1;

		//child不能大于节点总的个数
		while (child<_a.size())
		{
			//如果右节点存在并且比左边节点大
			if (child + 1 < _a.size() && _a[child + 1] > _a[child])
			{
				child++;
			}

			//如果孩子比父亲大,则交换，进行下一次循环
			//否则，跳出循环
			if (_a[child] > _a[parent])
			{
				swap(_a[child], _a[parent]);
				//父亲孩子向下走
				parent = child;
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}

	}
};

void TestHeap()
{
	int a[] = { 10, 16, 18, 12, 11, 13, 15, 17, 14, 19 };
	Heap<int> hp1(a, sizeof(a) / sizeof(a[0]));
	hp1.Push(20);
	hp1.Pop();
}
