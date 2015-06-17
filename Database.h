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


class Database{
private:
	map<int,Staff *> staffs;	//为了使用多态，这里采用存储指针的方式。id(int)为主键
public:
	void Load();
	void Save();
	~Database();
}; 

#endif
