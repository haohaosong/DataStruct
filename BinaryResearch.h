#include<iostream>
using namespace std;

/*递归的时间复杂度：递归总次数*每次递归的时间复杂度*/
/*递归的空间复杂度：递归的深度*/

/*时间复杂度：O(logN)*/
/*空间复杂度：:O(logN)*/
int BinaryResearch(int *arr,int left,int right,int find)
{
	while(left <= right)
	{
		int mid = (left+right)>>1;
		if(arr[mid] < find)
		{
			left = mid + 1;
		}
		else if(arr[mid]>find)
		{
			right = mid - 1;
		}
		else
		{
			return mid;
		i}
	}
	return -1;
}

/*时间复杂度:O(logN)*/
/*空间复杂度:O(N)*/
int BinaryResearch(int *arr,int left,int right,int find)
{
	if(left>right)
		return -1;
	int mid = arr[(left+right)>>1];
	if(arr[mid]==find)
		return mid;
	else if(arr[mid] > find)
		return BinaryResearch(arr,left,mid-1,find);
	else
		return BinaryResearch(arr,mid+1,right,find);
}

void FunTest()
{
	int arr[10] = {0,1,2,3,4,5,6,7,8,9};
	int ret = BinaryResearch(arr,0,9,10);
	cout<<"pos:"<<ret<<endl;
}
