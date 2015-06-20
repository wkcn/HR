#ifndef HR_DEFINES_H
#define HR_DEFINES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>

#include <cstdio>
#include <cmath>

using namespace std;

//为了方便，将实验要求稍作修改
//GetAchievement()函数返回的是Achievement结构体，
//记录的是业绩数据
struct Achievement{
	int sales;
	int events;
	Achievement():sales(0),events(0){}
	Achievement(int _sales,int _events):sales(_sales),events(_events){};
};

enum STAFF_STATE{
	ACTIVE,RESIGN,LEAVE
};// 员工状态（在职，离职，请假）

enum STAFF_KIND{
	STAFF,SALESMAN,MANAGER,SALESMANAGER
};//员工类型

void StrSplit(const string &str,vector<string> &res,char c = '\n');	//分割字符串
int STOI(const string &str);	//字符串转数字
string ITOS(int num);

char WordLower(char c);

bool IgnoreLU(const string a,const string b);

string StrLower(const string &s);

bool isBlank(char c);

bool MatchStr(const string &name,const string &filter,size_t i=0,size_t j=0);

//替换字符串函数
//摘录自 http://blog.csdn.net/cll131421/article/details/7959909
string& replace_all(string& str,const string & old_value,const string & new_value);
string& replace_all_distinct(string& str,const string & old_value,const string & new_value);

template <typename T>
void Qsort(T a[],int lo,int hi,bool (*lt)(T *a,T *b)){
	if (lo >= hi)return;
	int first = lo;
	int last = hi;
	T key = a[first];

	while (first < last){
		while (first < last && !lt(a[last],key))--last;
		a[first] = a[last];/*将比第一个小的移到低端*/
		while (first < last && lt(a[last],key))++first;
		a[last] = a[first];/*将比第一个大的移到高端*/
		
		a[first] = key;
		qsort(a,lo,first-1,lt);
		qsort(a,first+1,hi,lt);
	}
}

template <typename T>
void Qsort(T *begin,T *end,bool(*lt)(T *a,T *b)){
	qsort(begin,0,end-begin-1,lt);
}

#endif
