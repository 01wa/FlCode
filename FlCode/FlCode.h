/*
模块：分离码

功能：
	分离码是一种信息编解码技术，主要利用数学的不同进制转换来形成，结合码表和数学的进制转换，提出码位分离的编解码方法。
版权：
   上海泥娃通信科技有限公司
   email: zhangliuxue@126.com
*/
#pragma once
#include "math.h"
#include "malloc.h"
#include "string.h"
#include "iostream"
#include "string"
#include "map"
#include "vector"
using namespace std;

//注意：当字符串为空时，也会返回一个空字符串
void split(const std::string& s, std::string& delim, std::vector< std::string >* ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last>0)
	{
		ret->push_back(s.substr(last, index - last));
	}
}
//进制转换结构描述：jz表示多少进制，letters表示进制的字符串	，iCodeTable表示字符代表的数
typedef struct JzStatus
{
	int jz;
	string letters;
	std::map<char, int> iCodeTable;
}JzStatus;
union i_c
{
	unsigned int iVal;
	unsigned char cVal[sizeof(unsigned int)];
};
#define uint32	 unsigned int
#define BigtoLittle32(A) ((( (uint32)(A) & 0xff000000) >> 24) | \
	(((uint32)(A)& 0x00ff0000) >> 8) | \
	(((uint32)(A)& 0x0000ff00) << 8) | \
	(((uint32)(A)& 0x000000ff) << 24))
bool IsBigEndian()
{
	union NUM
	{
		int a;
		char b;
	}num;
	num.a = 0x1234;
	if (num.b == 0x12)
	{
		return true;
	}
	return false;
}
//初始化
void InitJz(JzStatus & st)
{
	if (st.letters.empty())
		st.letters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-";
		
	st.jz = st.letters.size();
	for (int i = 0; i < st.jz; i++)
	{
		st.iCodeTable[st.letters[i]] = i;
	}
}
//获取字符串表示的整数
unsigned int GetJzInt(JzStatus & st,const char * cValue, size_t iL)
{
	unsigned int i = 0;
	for (size_t k =0; k<iL; k++)
	{
		i = i + st.iCodeTable[cValue[k]] *(unsigned int) pow((double)st.jz, (int)k);
	}
	return i;
}
//获取整数表示的字符串，并给出字符串的长度
char * GetJzChar(JzStatus & st,unsigned int iValue, int & k)
{
	char * p = (char*)malloc(2);
	unsigned int i = iValue;
	memset(p, 0, 2);
	while (i != 0)
	{
		i = iValue / st.jz;
		p[k] = st.letters[iValue % st.jz];
		if (i > 0)
		{
			k++;
			p = (char*)realloc(p, k + 2);
		}
		iValue = i;
	}
	p[k + 1] = '\0';
	k ++;
	return p;
}
/*
编码：	输入消息，转换为字符串序列和位数序列
*/
string EncodeJz(JzStatus & st,const char* msg,size_t len,std::vector<unsigned char>&iSer)
{
	int i = len/sizeof(unsigned int);
	int j = len % sizeof(unsigned int);
	string outstr;
	for (int k = 0; k < i; k++)
	{
		int p=0;
		memcpy(&p, (void*)&msg[k*sizeof(int)], sizeof(int));
		int m = 0;
		char * tmp = GetJzChar(st, p, m);
		outstr.append(tmp);
		delete[] tmp;
		iSer.emplace_back(m);
	}
	if (j>0)
	{
		int p=0;
		memcpy(&p, (void*)&msg[i*sizeof(int)],j);
		int m = 0;
		char * tmp = GetJzChar(st, p, m);
		outstr.append(tmp);
		iSer.emplace_back(m);
		delete[] tmp;
	}
	return std::move(outstr);
}
/*
解码
*/
vector<unsigned char> DecodeJz(JzStatus & st, const string &msg, const size_t& len, const std::vector<unsigned char>&iSer)
{
	vector<unsigned char> p1;
	size_t offset = 0;
	const char * tp = msg.c_str();
	int po = 0;
	i_c ic;
	for (auto &x : iSer)
	{
		unsigned int m = GetJzInt(st, &tp[offset], x);
		ic.iVal = m;
		for (int h = 0; h < sizeof(int);h++)
			p1.emplace_back(ic.cVal[h]);
		offset += x;
	}
	return std::move( p1);
}
/*
编码：空格分割
*/
string EncodeJzByspace(JzStatus & st, const char* msg, size_t len)
{
	int i = len / sizeof(unsigned int);
	int j = len % sizeof(unsigned int);
	string outstr;
	for (int k = 0; k < i; k++)
	{
		int p = 0;
		memcpy(&p, (void*)&msg[k*sizeof(int)], sizeof(int));
		int m = 0;
		char * tmp = GetJzChar(st, p, m);
		outstr.append(tmp);
		outstr.append(" ");
		delete[] tmp;
	}
	if (j>0)
	{
		int p = 0;
		memcpy(&p, (void*)&msg[i*sizeof(int)], j);
		int m = 0;
		char * tmp = GetJzChar(st, p, m);
		outstr.append(tmp);
		outstr.append(" ");
		delete[] tmp;
	}
	return std::move(outstr);
}
/*
解码：空格分割
*/
vector<unsigned char> DecodeJzByspace(JzStatus & st, const string &msg, const size_t& len)
{
	vector<unsigned char> p1;
	size_t offset = 0;
	const char * tp = msg.c_str();
	int po = 0;
	i_c ic;
	std::vector< std::string > strvec;
	string 	delim = " ";
	split(msg, delim, &strvec);
	for (auto &x : strvec)
	{
		unsigned int m = GetJzInt(st, x.c_str(), x.length());
		ic.iVal = m;
		for (int h = 0; h < sizeof(int); h++)
			p1.emplace_back(ic.cVal[h]);
	}
	return std::move(p1);
}