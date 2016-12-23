/*
* author:haohaosong
* date:2016/12/7
* note:Vector是用顺序表实现的，所以在实现迭代器的时候，我们可以采用原生指针
*     但是反向迭代器扔需要重新定义，在定义反向迭代器的模板里，我们需要重载++，--
*	  以及解引用等操作符
*/
#pragma once 

#include<assert.h>
#include<iostream>
using namespace std;

//定义反向迭代器模板
template<typename T,typename Ref,typename  Ptr>
struct __VectorReverseIterator
{
	typedef __VectorReverseIterator<T, T&, T*> self;//简化名称为self

	T* _node;//反向迭代器也只有一个原生指针，只是++，--的操作不同而已

	__VectorReverseIterator(T* node)//构造函数，传入一个原生指针，把它定义成反向迭代器
		:_node(node)
	{}

	//反向迭代器最重要的部分，重载++，--
	self& operator++()//前置++
	{
		return _node--;
	}
	self operator++(int)//后置++
	{
		self tmp(_node);
		_node--;
		return tmp;
	}
	self& operator--()//前置++
	{
		return _node++;
	}
	self operator--(int)//后置++
	{
		self tmp(_node);
		_node++;
		return tmp;
	}

	//重载==和!=，这也是非常重要的，不然在使用 it != v.rEnd();会提示 != 没有定义
	bool operator==(const self & s)
	{
		return _node == s._node;
	}
	bool operator!=(const self & s)
	{
		return _node != s._node;
	}

	//解引用，访问原生指针的元素
	Ref operator*()
	{
		return (*_node);
	}
	Ref operator->()
	{
		return (*_node);
	}
};

//定义Vector
template<typename T>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
	typedef __VectorReverseIterator<T, T&, T*> ReverseIterator;//重新命名为ReverseIterator
public:
	//构造函数
	Vector()
		:_start(0)
		, _finish(0)
		, _endOfStorage(0)
	{}
	Vector(const Vector<T>& v)
		:_start(new T[v.Size()])
	{
		_finish = _start + v.Size();
		_endOfStorage = _start + v.Size();
		if (TypeTraits<T>::IsPODType().Get())
		{
			memcpy(_start, v._start, sizeof(T)*v.Size());
		}
		else
		{
			for (size_t i = 0; i < v.Size(); ++i)
			{
				_start[i] = v._start[i];
			}
		}
	}

	//自定义的swap函数，库内部的swap只能用于交换内置类型
	void swap(Vector<T> v)
	{
		std::swap(v._endOfStorage, _endOfStorage);
		std::swap(v._finish, _finish);
		std::swap(v._start, _start);
	}
	Vector<T>& operator=(Vector<T> v)
	{
		//这里不能直接调用库里面swap进行对象的交换，会出现循环递归调用导致栈溢出的后果
		swap(v);
		return *this;
	}

	//添加/删除一个元素
	void PushBack(const T& x)
	{
		CheckCapacity();
		*_finish = x;
		++_finish;
	}
	void PopBack()
	{
		assert(Size());
		--_finish;
	}

	//求第一个元素的位置
	Iterator Begin()
	{
		return _start;
	}
	const Iterator Begin() const
	{
		return _start;
	}
	Iterator End()
	{
		return _finish;
	}
	const Iterator End() const
	{
		return _finish;
	}

	//仅仅只用新添加 rBegin接口和 rEnd()接口
	ReverseIterator rBegin()
	{
		return _finish - 1;
	}
	ReverseIterator rEnd()
	{
		return _start - 1;
	}

	//删除固定位置的一个元素
	void Erase(Iterator pos)
	{
		Iterator cur = pos;
		while (cur != _finish)
		{
			*cur = *(cur + 1);
			++cur;
		}
		--_finish;
	}

	//在指定位置插入一个元素，内容为x
	void Insert(Iterator pos, const T& x)
	{
		//进行容量判断
		if (Capacity() == 0)
		{
			CheckCapacity();
			*_start = x;
		}
		else
		{
			size_t tmp = pos - _start;
			CheckCapacity();
			Iterator cur = _finish - 1;
			while (cur >= (_start + tmp))
			{
				*(cur + 1) = *cur;
				cur--;
			}
			*(_start + tmp) = x;
		}
		++_finish;
	}

	//改变Vecctor的大小，缺省参数是给定的默认值
	void Resize(size_t sz, T c = T())
	{
		if (sz <= Size())
		{
			_finish -= (Size() - sz);
		}
		size_t size = Size();
		size_t capacity = Capacity();
		if (sz > capacity)
		{
			size_t newcapacity = sz;
			Iterator tmp = new T[newcapacity];

			//这里进行类型的判断，利用类型萃取
			if (TypeTraits<T>::IsPODType().Get())
			{
				//进行浅拷贝
				memcpy(tmp, _start, sizeof(T)*size);
			}
			else
			{
				//进行深拷贝
				for (size_t i = 0; i < size; ++i)
				{
					tmp[i] = _start[i];
				}
			}

			//释放原来的空间，指向新开辟的这段空间
			delete _start;
			_start = tmp;

			//_finish和_endOfStorage 需要用start进行计算，因为内存位置已经改变
			_finish = _start + sz;
			_endOfStorage = _finish;

			//若给定的大小比之前大，则将新的空间赋值为默认值c
			Iterator cur = _finish;
			while (cur != _finish)
			{
				*cur = c;
				cur++;
			}
		}
		else
		{
			//直接赋值为默认值c
			Iterator cur = _finish;
			_finish = _start + sz;
			while (cur != _finish)
			{
				*cur = c;
				cur++;
			}
		}
	}

	//扩容
	void Reserve(size_t newcapacity)
	{
		size_t size = Size();
		if (newcapacity <= Capacity())
			return;
		Iterator tmp = new T[newcapacity];

		//依旧进行类型萃取
		if (TypeTraits<T>::IsPODType().Get())
		{
			memcpy(tmp, _start, sizeof(T)*size);
		}
		else
		{
			for (size_t i = 0; i < size; ++i)
			{
				tmp[i] = _start[i];
			}
		}
		delete[] _start;
		_start = tmp;
		_finish = _start + size;
		_endOfStorage = _start + newcapacity;
	}

	//重载[],方便像数组一样使用
	T& operator[](const size_t pos)
	{
		assert(pos< Size());
		return _start[pos];
	}
	const T& operator[](const size_t pos) const
	{
		assert(pos < Size());
		return _start[pos];
	}

	//Front，Back接口
	Iterator Front()
	{
		return _start;
	}
	ConstIterator Front()const
	{
		return _start;
	}
	Iterator Back()
	{
		return _finish;
	}
	ConstIterator Back()const
	{
		return _finish;
	}

	//获得Vector的Size和Capacity
	size_t Size()
	{
		return _finish - _start;
	}
	size_t Size() const
	{
		return _finish - _start;
	}
	size_t Capacity()
	{
		return _endOfStorage - _start;
	}
	size_t Capacity() const
	{
		return _endOfStorage - _start;
	}
protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
	void CheckCapacity()
	{
		if (_finish == _endOfStorage)
		{
			size_t size = Size();
			size_t newcapacity = Capacity() * 2 + 3;
			Iterator tmp = new T[newcapacity];
			if (TypeTraits<T>::IsPODType().Get())
			{
				memcpy(tmp, _start, sizeof(T)*size);
			}
			else
			{
				for (size_t i = 0; i < size; ++i)
				{
					tmp[i] = _start[i];
				}
			}
			delete[] _start;
			_start = tmp;
			_finish = _start + size;
			_endOfStorage = _start + newcapacity;
		}
	}
};

//类型萃取
struct __TrueType
{
	bool Get()
	{
		return true;
	}
};
struct __FalseType
{
	bool Get()
	{
		return false;
	}
};
//类型萃取 
template<typename T>
struct TypeTraits
{
	typedef __FalseType IsPODType;
};
template<>
struct TypeTraits<int>
{
	typedef __TrueType IsPODType;
};
template<>
struct TypeTraits<char>
{
	typedef __TrueType IsPODType;
};

//测试反向迭代器
void FunTest()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);

	Vector<int>::ReverseIterator it = v.rBegin();//定义反向迭代器 it ,指向最后一个元素
	while (it != v.rEnd())//判断it 是不是指向第一个元素的前一个
	{
		cout << *it << " ";//进行打印
		it++;//元素向前挪
	}
	cout << endl;
}
