/*
ģ�飺������

���ܣ�
	��������һ����Ϣ����뼼������Ҫ������ѧ�Ĳ�ͬ����ת�����γɣ�����������ѧ�Ľ���ת���������λ����ı���뷽����
��Ȩ��
   �Ϻ�����ͨ�ſƼ����޹�˾
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
//����ת���ṹ������jz��ʾ���ٽ��ƣ�letters��ʾ���Ƶ��ַ���	��iCodeTable��ʾ�ַ���������
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
//��ʼ��
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
//��ȡ�ַ�����ʾ������
unsigned int GetJzInt(JzStatus & st,const char * cValue, size_t iL)
{
	unsigned int i = 0;
	for (size_t k =0; k<iL; k++)
	{
		i = i + st.iCodeTable[cValue[k]] *(unsigned int) pow((double)st.jz, (int)k);
	}
	return i;
}
//��ȡ������ʾ���ַ������������ַ����ĳ���
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
���룺
*/
string EncodeJz(JzStatus & st,const char* msg,size_t len,std::vector<unsigned char>&iSer)
{
	int i = len/sizeof(unsigned int);
	int j = len % sizeof(unsigned int);
	string outstr;
	for (int k = 0; k < i; k++)
	{
		int p;
		memcpy(&p, (void*)&msg[k*sizeof(int)], sizeof(int));
		int m = 0;
		char * tmp = GetJzChar(st, p, m);
		outstr.append(tmp);
		delete[] tmp;
		iSer.emplace_back(m);
	}
	if (j>0)
	{
		int p;
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
����
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