#ifndef FORM_H
#define FORM_H

//表格类,为了更好在控制台打印结构化数据

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;

class Form{
	public:
		void write(int r,int c,const string s);
		void write(int r,int c,int num);
		void write(int r,int c,double f);
		string read(int r,int c);
		void print();
	private:
		void UpdateWidth(int c);//更新宽度
		void printC(char c,int n);//输出n个字符c
		void printData(int start,int end);//列出表格数据,行[start,end]
		void drawLine();//画边界线
		size_t GetFontSize(const string s);
	private:
		vector<size_t> widths;//列宽
		vector< vector<string> > datas;
};

#endif
