/*
* author:haohaosong
* date:2017/2/17
* note:哈希表的实现 
*/

#pragma once 

#include<iostream>
using namespace std;

#include<assert.h>
#include<vector>

//定义节点的三种状态 
enum Status
{
    EXIST,
    DELETE,
    EMPTY,
};

//定义哈希节点 
template<typename K,typename V>
struct HashNode
{
    K _key;
    V _value;
    Status _status;

    HashNode(const K& key = K(), const V& value = V())
        :_key(key)
        , _value(value)
        , _status(EMPTY)
    {}
};

//定义仿函数
template<typename K>
struct __HashFunc
{
    size_t operator()(const K& key)
    {
        return key;
    }
};

//特化string的版本
template<>
struct __HashFunc<string>
{
    size_t operator()(const string& s)
    {
        size_t ret = 0;

        //利用迭代器访问每个字符，然后加上该字符的ASCII码
        string::const_iterator it = s.begin();
        while (it != s.end())
        {
            ret +=(131+*it);
            it++;
        }
        return ret;
    }
};

//使用素数表的对齐作为哈希表的容量，可以降低哈希冲突 
const int _PrimeSize = 28;
static const unsigned long _PrimeList[_PrimeSize] =
{
    53ul, 97ul, 193ul, 389ul, 769ul,
    1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
    49157ul, 98317ul, 196613ul, 393241ul,
    786433ul,
    1572869ul, 3145739ul, 6291469ul, 12582917ul,
    25165843ul,
    50331653ul, 100663319ul, 201326611ul, 402653189ul,
    805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
};

template<typename K, typename V, typename HashFunc = __HashFunc<K>>
class HashTable
{
    typedef HashNode<K, V> Node;
public:
    HashTable()
    {}

    HashTable(size_t size)
        :_size(0)
    {
        assert(size > 0);
        _v.resize(size);
    }

    //将K值转换成哈希值
    size_t __HashFunC(const K& key)
    {
        HashFunc hf;//定义一个HashFunc的变量hf
        size_t hash = hf(key);//用变量hf调用HashFunc的仿函数，返回对应的整型
        return hash % _v.size();//算出哈希值，并返回
    }

    pair<Node*, bool> Insert(const K& key,const V& value)
    {
        //检查是否需要扩容
        CheckCapacity();

        //对K值进行取余，判断插入的位置
        size_t index = HashFunC(key);

        //如果存在，则循环着继续寻找
        while (_v[index]._status == EXIST)
        {
            index++;
            if (index == _v.size())
                index = 0;
        }

        _v[index]._key = key;
        _v[index]._value = value;
        _v[index]._status = EXIST;

        _size++;

        return make_pair<Node*,bool>(&_v[index] ,true);
    }

    Node* find(const K& key)
    {
        //对K值进行取余，判断插入的位置
        size_t index = HashFunC(key);

        //如果存在，则继续寻找
        while (_v[index]._status == EXIST)
        {
            //若相等，判断状态是否是删除
            //若删除，则没找到，返回空
            //若没删除，则返回该位置的地址
            if (_v[index]._key == key)
            {
                if (_v[index]._status == DELETE)
                    return NULL;

                return &_v[index];
            }

            index++;
            if (index == _size)
                index = 0;
        }

        return &_v[index];
    }

    void Remove(const K& key)
    {
        //删除仅需要将状态修改
        Node* delNode = find(key);

        if (delNode)
            delNode->_status = DELETE;
    }
    
protected:
    vector<Node> _v;
    size_t _size;
protected:
    //交换两个哈希表
    void Swap(HashTable<K, V> &h)
    {
        swap(_v, h._v);
        swap(_size, h._size);
        return;
    }

    void CheckCapacity()
    {
        //如果_v为空，则扩容到7
        if (_v.empty())
        {
            _v.resize(_PrimeList[0]);
            return;
        }

        //如果超过比例系数，则需要扩容
        if (_size*10 / _v.size() >= 7)
        {
            /*size_t newSize = 2 * _v.size();*/

            //找到更大一个的素数 
            size_t index = 0;
            while (_PrimeList[index] < _v.size())
            {
                index++;
            }
            size_t newSize = _PrimeList[index];

            //用一个临时变量来存储新生成的哈希表
            //生成完成后，将其和_v交换
            HashTable<K, V> tmp(newSize);
            for (size_t i = 0; i < _v.size(); ++i)
            {
                //如果存在，则将该位置的哈希值插入到临时的哈希表中
                if (_v[i]._status == EXIST)
                    tmp.Insert(_v[i]._key,_v[i]._value);
            }
            //交换两个哈希表
            Swap(tmp);
        }
    }
};

void TestHashTable()
{
    HashTable<int, int> h1(10);
    h1.Insert(89, 0);
    h1.Insert(18, 0);
    h1.Insert(49, 0);
    h1.Insert(58, 0);

    h1.find(9);

    HashTable<string, string> dict;
    dict.Insert("left", "左边");
    dict.Insert("sort", "排序");
    dict.Insert("right", "右边");
    dict.Insert("int", "整形");
    dict.Insert("double", "双精度");
    dict.Insert("hello", "你好");

    dict.Remove("left");
    dict.Remove("left");
    dict.Remove("left");
}
