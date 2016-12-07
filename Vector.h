/*
* author:haohaosong 
* date:2016/12/7 
*/ 

#pragma once 

#include<assert.h>
#include<iostream>
using namespace std;

template<typename T>
class Vector
{
public:
	typedef T* Iterator;
public:
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
		if (TypeTraits<T>::TypeTraits().Get())
		{
			memcpy(_start, v._start, sizeof(T)*size);
		}
		else
		{
			for (size_t i = 0; i < v.Size(); ++i)
			{
				_start[i] = v._start[i];
			}
		}
	}
	Vector<T>& operator=(Vector<T> v)
	{
		std::swap(*this, v);
		return *this;
	}
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
	void Insert(Iterator pos,const T& x)
	{
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
				*(cur+1) = *cur;
				cur--;
			}
			*(_start + tmp) = x;
		}
		++_finish;
	}
	void Resize(size_t sz,T c = T())
	{
		if (sz <= Size())
		{
			_finish -= (Size() - sz);
		}
		size_t size = Size();
		size_t capacity = Capacity();
		if (sz > capacity)//需要重新开辟空间
		{
			size_t newcapacity = sz;
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
			delete _start;
			_start = tmp;
			_finish = _start + sz;
			_endOfStorage = _finish;
			Iterator cur = _finish;
			while (cur != _finish)
			{
				*cur = c;
				cur++;
			}
		}
		else//不需要开辟空间
		{
			Iterator cur = _finish;
			_finish = _start + sz;
			while (cur != _finish)
			{
				*cur = c;
				cur++;
			}
		}
	}
	void Reserve(size_t newcapacity)
	{
		size_t size = Size();
		if (newcapacity <= Capacity())
			return;
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
	Iterator Front()
	{
		return _start;
	}
	Iterator Back()
	{
		return _finish;
	}
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
			size_t newcapacity = Capacity()*2 + 3;
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
