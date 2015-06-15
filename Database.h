#ifndef DATABASE_H
#define DATABASE_H

#include "Staff.h"
#include "SalesMan.h"
#include "SalesManager.h"

//这次让我们模拟数据库！
/*
	支持指令：
	select sales where id>5
	select (sales,events)
	update ()
	以上均返回二维数组
*/
class Database{
private:
	//map<int,Staff *> staffs;	//为了使用多态，这里采用存储指针的方式。id(int)为主键
public:
	void Load();
	void Save();
	~Database();
};

#endif