#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#include <cstdio>

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

char WordLowwer(char c);

bool IgnoreLU(const string a,const string b);

string StrLowwer(const string &s);

#endif
