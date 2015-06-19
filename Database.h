#ifndef DATABASE_H
#define DATABASE_H

#include "Staff.h"
#include "SalesMan.h"
#include "SalesManager.h"

//这次让我们模拟数据库！
//
/*
 我们的表格结构

 crate table STAFFTABLE
 (
	id int unsigned not null auto_increment primary key,
	name text,
	kind int,
	age int,
	state int,
	sales int,
	events int
 );
*/

//类型
enum EXP_KIND{ 
	EXP_OP,
	EXP_VAR,
	EXP_NUM,
	EXP_STR
};
struct Exp{
	//为了方便设计，这里暂时不考虑效率问题
	string name;
	EXP_KIND kind;
	Exp *lv;
	Exp *rv;
};

class Comparer{
	public:
		Comparer(Exp *e):filter(e){};
		bool operator()(Staff *a,Staff *b){
			return a->GetID() < b->GetID();
		}
	private:
		Exp *filter;
};

class Database{
private:
	map<int,Staff *> staffs;	//为了使用多态，这里采用存储指针的方式。id(int)为主键
	map<string,string> alias;   //员工职称的别名(别名，原名)
	stringstream helpText;
	vector<string> priority; //优先级
	vector<bool> prirev; //逆序
	int cur_page;//当前页数 从1开始
	int max_page;//最大页数
	int page_items;//每页项目数
private:
	int ISP(const string &op);
	int ICP(const string &op);
	Exp *Build(const string filter);
	string NextStr(const string &s,size_t &poi,char c = ' ');
	void JumpSpace(const string &s,size_t &poi);
	void ShowPat();//显示输入格式
	int GetInt(string s);
	string GetStr(string s);
	string GetSourceName(string s); //得到原名并转为小写
public:
	Database();
	~Database();
	void Load();
	void Save();
	void Show();
	void Execute(string com);//执行SQL命令
	//void FetchAll(DBData &d);//获取结果
	void GetData(vector<Staff*> v,const string filter = "");//获取数据，DB类可以解析过滤器语言
}; 

#endif
