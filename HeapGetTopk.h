/*
* author:haohaosong
* date:2016/12/31
* note:求一堆数据前K个最大数，利用堆实现 
*/

#pragma once 
 
void TopK(int* a, size_t k, size_t n)
{
	//初始化一个数组
	int* heap = new int[k];

	//将前k个元素放入数组
	for (size_t i = 0; i < k; i++)
	{
		heap[i] = a[i];
	}

	//向下调整，使之成为堆
	for (int i = (k - 2) / 2; i >= 0; --i)
	{
		AdjustDown(heap,k,i);
	}

	//遍历，放入堆的顶部
	for (size_t i = k; i < n; ++i)
	{
		//如果比堆顶小，放入，进行向下调整
		if (a[i]>heap[0])
		{
			heap[0] = a[i];
			AdjustDown(heap, k, 0);
		}
	}

	for (size_t i = 0; i < k; i++)
	{
		cout << heap[i] << " ";
	}
	cout << endl;
}

void TestTopK()
{
	const int N = 10000;
	const int K = 5;
	int a[N];

	for (size_t i = 0; i < N; ++i)
	{
		a[i] = rand() % N;
	}

	a[1] = N + 1;
	a[100] = N + 2;
	a[200] = N + 3;
	a[666] = N + 4;
	a[111] = N + 19;

	TopK(a, K, N);
}
