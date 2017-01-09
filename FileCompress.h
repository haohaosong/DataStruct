/*
* author:haohaosong
* date:2017/1/1
* note:文件压缩小项目 
*/ 

#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
using namespace std;
#include<assert.h>
#include<string>
#include<algorithm>
#include"HuffmanTree.h"
typedef long long LongType;

struct CharInfo
{
	char _ch;
	LongType _count;
	string _code;

	CharInfo(LongType count = 0)
		:_count(count)
	{}

	bool operator !=(const CharInfo& c)
	{
		return 	_count != c._count;
	}

	bool operator<(const CharInfo& c)
	{
		return _count < c._count;
	}

	CharInfo operator+(const CharInfo&c)
	{
		return CharInfo(_count+c._count);
	}
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
public:
	//给infos初始化，次数归0
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}

	void Compress(const char* filename)
	{
		assert(filename);

		//1.统计字符出现的个数
		FILE* fout = fopen(filename, "r");
		assert(fout);
		char ch = fgetc(fout);
		while (ch != EOF)
		{
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//2.生成哈弗曼树
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> huff(_infos,256,invalid);

		//生成哈弗曼编码
		GetHuffmanCode(huff.GerRoot());

		//进行压缩
		string ComFilename = (string)filename;
		ComFilename += ".huff";
		FILE* fIn = fopen(ComFilename.c_str(), "w");//c_str()返回值为const类型
		
		char value = 0;
		int pos = 0;
		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);
		int i = 0;
		while (ch != EOF)
		{
			string& code = _infos[(unsigned char)ch]._code;
			while (pos<8)
			{
				value <<= 1;

				if (code.at(i++) == '1')
					value |= 1;

				pos++;

				if (i == code.length())
					break;
			}

			if (pos == 8)
			{
				fputc(value, fIn);
				pos = 0;
				value = 0;
			}

			if (i == code.length())
			{
				i = 0;
				ch = fgetc(fout);
			}
		}
		if (pos < 8)
		{
			value <<= (8 - pos);
			fputc(value, fIn);
		}

		fclose(fIn);
		fclose(fout);
		//while (ch != EOF)
		//{
		//	//if (ch == '1')
		//	string& code = _infos[ch]._code;

		//	/*if (code.at(1) == '1')
		//		value |= 1;

		//	value <<= 1;*/

		//	bool flag = true;
		//	for (int i = 0; i < code.length(); ++i)
		//	{
		//		if (code.at(i) == '1')
		//			value |= 1;

		//		value <<= 1;
		//		pos++;

		//		if (pos == 8&& i+1 != code.length())
		//		{
		//			flag = false;
		//			break;
		//		}
		//	}

		//	if (pos == 8)
		//	{
		//		fputc(value,fIn);
		//		value = 0;
		//		pos = 0;
		//	}
		//	if (flag == true)
		//		ch = fgetc(fout);
		//	else
		//}
		////if (pos>0 && pos < 8)
		//{
		//	//value <<= (8 - pos);
		//	fputc(value, fIn);
		//}
		//fclose(fout);
		//fclose(fIn);
	}

	//解压缩
	void Uncompress(const char* filename)
	{
		assert(filename);
		string unCompressFile = filename;
		size_t thePos = unCompressFile.find_last_of('.');
		unCompressFile.resize(thePos);
		unCompressFile += ".unhuff";

		FILE* fout = fopen(filename,"r");
		assert(fout);

		FILE* fin = fopen(unCompressFile.c_str(), "w");
		assert(fin);

		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> huff(_infos, 256, invalid);
		
		//总的字符个数等于根节点的的count
		size_t allCount = huff.GerRoot()->_weight._count;
		
		//获取Huffman树的根节点，以便每次用根节点开始
		const HuffmanTreeNode<CharInfo>* root = huff.GerRoot();
		HuffmanTreeNode<CharInfo>* cur = const_cast<HuffmanTreeNode<CharInfo>*>(root);
		
		int pos = 7;
		char ch = fgetc(fout);
		while (ch != EOF)
		{
			//取出要判别的位，并判断是0还是1
			if ((ch >> pos) & 1)
				cur = cur->_right;
			else
				cur = cur->_left;

			//如果读到根节点，重置cur，并进行写入
			if (cur->_left == NULL && cur->_right == NULL)
			{
				allCount--; 
				fputc(cur->_weight._ch, fin);
				cur = const_cast<HuffmanTreeNode<CharInfo>*>(root);

				//allCount用来判断字符是否写完
				if (allCount == 0)
					break;
			}

			//读到最后一位，写入，并且读下一个字符
			if (pos == 0)
			{
				ch = fgetc(fout);
				pos = 7;
			}
			else
			{
				pos--;
			}
		}
		fclose(fin);
		fclose(fout);
	}

	//得到哈弗曼编码
	void GetHuffmanCode(const Node* root)
	{
		if (root == NULL)
			return;

		//找到叶子节点，则求编码，并返回
		//否则递归遍历寻找
		if (root->_left == NULL && root->_right == NULL)
		{
			//利用引用，减少拷贝的开销
			string& code = _infos[(unsigned char)root->_weight._ch]._code;

			//获取huffman编码，并记录在根节点上
			Node* parent = root->_parent;
			Node* child = (Node*)root;
			while (parent)
			{
				if (parent->_right == child)
					code.push_back('1');
				else
					code.push_back('0');

				child = parent;
				parent = parent->_parent;
			}
			//逆序
			reverse(code.begin(), code.end());

			return;
		}

		//递归遍历
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);
	}

protected:
	CharInfo _infos[256];
};

void TestFileCompress()
{
	FileCompress f;
	f.Compress("input.txt");
	f.Uncompress("input.txt.huff");
}
