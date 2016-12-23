#pragma once 

/*
* author:haohaosong
* date:2016/12/23
* note:在实现二叉树时，用到栈和队列两种数据结构，并且，可以采用递归和非递归的两种形式来实现
*/
#include<stack>
#include<assert.h>
#include<queue>

//定义一个二叉树节点
template<typename T>
struct BinaryTreeNode
{
	T _data;//数据
	BinaryTreeNode<T> *_left;//左孩子
	BinaryTreeNode<T> *_right;//右孩子

	BinaryTreeNode(const T& x)//节点的构造函数
		:_data(x)
		, _left(NULL)
		, _right(NULL)
	{}
};

//定义二叉树
template<typename T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;//重命名为Node
public:
	BinaryTree()
		:_root(NULL)
	{}
	BinaryTree(T* arr, const size_t size,const T& invalid = T())
	{
		assert(arr);
		size_t index = 0;
		_root = CreateTree(arr, size, index, invalid);
	}

	//拷贝构造函数
	BinaryTree(const BinaryTree& b)
	{
		_root = Copy(b._root);
	}

	//赋值运算符重载
	BinaryTree&operator=(BinaryTree t)
	{
		if (this != &t)
		{
			std::swap(t._root, _root);
		}
		return *this;
	}

	//析构函数
	~BinaryTree()
	{
		if (_root != NULL)
		{
			Destory(_root);
			_root = NULL;
		}
	}

	//先序遍历，中序遍历，后序遍历
	void PrevOrder()
	{
		//采用递归方法，调用protected内部的_PrevOrder函数，中序和后序也是一样
		_PrevOrder(_root);
		cout << endl;
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
	void PostOrder()
	{
		_PostOrder(_root);
		cout << endl;
	}

	//三种遍历方式的非递归形式
	void PrevOrderNonR()
	{
		//定义一个栈和一个指针变量
		stack<Node*> s;
		Node* cur = _root;

		//在cur,或者栈为空时
		while (cur || !s.empty())
		{
			//递归遍历左字数
			while (cur)
			{
				//访问元素
				cout << cur->_data << " ";
				
				//进行压栈
				s.push(cur);
				
				//指向左孩子
				cur = cur->_left;
			}
			//一路向左，此时cur为空

			//取栈顶元素
			Node* top = s.top();
			//出栈
			s.pop();
			//访问右孩子
			cur = top->_right;
		}
		cout << endl;
	}

	//中序遍历的非递归，只用把压栈访问元素的位置改到出栈的时候访问即可
	void InOrderNonR()
	{
		//定义一个栈和指针变量cur
		Node* cur = _root;
		stack<Node*> s;

		//判断是否结束
		while (cur || !s.empty())
		{
			//循环压入左字数
			while (cur)
			{
				s.push(cur);

				//中序，先不要访问元素
				cur = cur->_left;
			}
			Node* top = s.top();
			s.pop();

			//此时再访问元素
			cout << top->_data << " ";
			cur = top->_right;
		}
	}

	//后序非递归遍历
	void PostOrderNonR()
	{
		//与中序，先序相比，多定义了一个prev指针，保存上一个访问的元素
		Node* prev = NULL;

		//定义一个栈s和指向节点的临时变量cur
		Node* cur = _root;
		stack<Node*> s;

		//判断是否结束
		while (cur || !s.empty())
		{
			//递归压入左子树
			while (cur)
			{
				//依旧不访问元素
				s.push(cur);
				cur = cur->_left;
			}

			//取栈顶元素进行判断
			Node* top = s.top();

			//如果站定元素的右子树为空  或者  右子树已经被访问
			if (top->_right == NULL || top->_right == prev)
			{
				//打印根
				cout << top->_data << " ";
				
				//将刚刚访问过的元素让prev保存起来
				prev = top;

				//出栈
				s.pop();
			}
			//右子树不为空 并且 还没有被访问
			else
			{
				//访问右字数
				cur = top->_right;
			}
		}
	}

	//层序遍历
	void LevelOrder()
	{
		if (_root == NULL)
			return;

		//利用队列来存储每一层的节点
		queue<Node*> q;
		//压入根节点
		q.push(_root);

		//队列为空，访问结束
		while (q.empty() == false)
		{
			//取队头元素，进行访问
			Node* tmp = q.front();
			cout << tmp->_data << " ";

			//弹出队列的首元素
			q.pop();

			//哪个孩子不为空，就压入该孩子
			if (tmp->_left != NULL)
				q.push(tmp->_left);
			if (tmp->_right != NULL)
				q.push(tmp->_right);
		}
		cout << endl;
	}

	//求二叉树的节点个数
	size_t Size()
	{
		return _Size(_root);
	}

	//求二叉树的深度
	size_t Depth()
	{
		return _Depth(_root);
	}

	//求二叉树的叶子节点
	size_t GetLeafSize()
	{
		size_t count = 0;
		_GetLeafSize(_root,count);
		return count;
	}

	//求第K层节点的个数
	size_t GetKLevelSize(size_t k)
	{
		assert(k > 0);
		return _GetKLevelSize(_root,k);
	}
protected:
	Node* CreateTree(T* arr,const size_t size, size_t& index,const T& invalid = T())
	{
		if (index < size && arr[index]!=invalid)
		{
			Node* root = new Node(arr[index]);
			root->_left = CreateTree(arr,size,++index,invalid);
			root->_right = CreateTree(arr, size, ++index, invalid);
			return root;
		}
		return NULL;
	}
	void Destory(Node* root)
	{
		assert(root);
		if (root->_left != NULL)
			Destory(root->_left);
		root->_left = NULL;
		if (root->_right != NULL)
			Destory(root->_right);
		root->_right = NULL;

		delete[] root;
		root = NULL;
		return;
	}
	Node* Copy(Node* root)
	{
		if (root == NULL)
			return NULL;
		Node* newnode = new Node(root->_data);
		newnode->_left = Copy(root->_left);
		newnode->_right = Copy(root->_right);
		return newnode;
	}
	void _PrevOrder(Node* root)
	{
		if (root == NULL)
			return;
		cout << root->_data << " ";
		_PrevOrder(root->_left);
		_PrevOrder(root->_right);
	}
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;
		_InOrder(root->_left);
		cout << root->_data << " ";
		_InOrder(root->_right);
	}
	void _PostOrder(Node* root)
	{
		if (root == NULL)
			return;
		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_data << " ";
	}
	size_t _Size(Node* root)
	{
		if (root == NULL)
			return 0;
		return _Size(root->_left) + _Size(root->_right) + 1;
	}
	size_t _Depth(Node* root)
	{
		if (root == NULL)
			return 0;
		size_t leftDepth = _Depth(root->_left);
		size_t rightDepth = _Depth(root->_right);

		return leftDepth > rightDepth ? leftDepth + 1: rightDepth + 1;
	}
	void _GetLeafSize(Node* root,size_t &count)
	{
		if (root->_left == NULL && root->_right == NULL)
		{
			count++;
			return;
		}
		if (root->_left != NULL)
			_GetLeafSize(root->_left,count);
		if (root->_right!=NULL)
			_GetLeafSize(root->_right,count);
	}
	size_t _GetKLevelSize(Node* root ,size_t k)
	{
		if (root == NULL)
			return 0;
		if (k == 1)
			return 1;

		return _GetKLevelSize(root->_left,k-1) + _GetKLevelSize(root->_right,k-1);
	}
protected:
	Node* _root;
};

void TestBinaryTree()
{
	int arr[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	//int arr[] = { 1, 2, '#', 3, '#','#', 4,5, '#', 6 ,'#', 7,'#','#',8};
	BinaryTree<int> t(arr,sizeof(arr)/sizeof(arr[0]),'#');
	t.PostOrder();
	t.PostOrderNonR();
}
