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

class Database{
private:
	map<int,Staff *> staffs;	//为了使用多态，这里采用存储指针的方式。id(int)为主键
public:
	void Load();
	void Save();
	void GetData(vector<Staff*> v,string filter = "");//获取数据，DB类可以解析过滤器语言
	~Database();
}; 

#endif
