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
//数据类型
enum VALUE_KIND{
	ID,NAME,KIND,AGE,STATE,MANAGER_ID,SALES,EVENTS
};
struct Exp{
	//为了方便设计，这里暂时不考虑效率问题
	string name;
	EXP_KIND kind;
	//VALUE_KIND vk;用枚举可提高效率
	Exp *lv;
	Exp *rv;
	Exp(){
		lv = rv = 0;
	}
	~Exp(){
		if (lv)delete lv;
		if (rv)delete rv;
	}
};

class Comparer{
	public:
		bool operator()(Staff *a,Staff *b);
		void clear();
		void push(VALUE_KIND vk,bool z);
		void show();
	private:
		int GetValue(Staff *s,VALUE_KIND vk);
		vector<VALUE_KIND> vks;//自定义的比较参数
		vector<bool> zs;//0是正序，1是逆序
};

class Database{
private:
	map<int,Staff *> staffs;	//为了使用多态，这里采用存储指针的方式。id(int)为主键
	map<int,set<int> > slaves;   //从属关系,(管理者id,被管理者集合)
	map<string,string> alias;   //员工职称的别名(别名，原名)
	stringstream helpText;
	vector<string> priority; //优先级
	vector<bool> prirev; //逆序
	size_t cur_page;//当前页数 从1开始
	size_t max_page;//最大页数
	size_t page_items;//每页项目数
	bool autosave;
	bool changed;
	Exp* viewFilter;//显示项目的过滤器
	string filterName;
	Comparer comparer;//排序器
private:
	int ISP(const string &op);
	int ICP(const string &op);
	Exp *Build(const string filter);
	string NextStr(const string &s,size_t &poi,char c = ' ');
	void JumpSpace(const string &s,size_t &poi);
	void ShowPat();//显示输入格式
	int GetInt(string s);
	void ReadInt(int &i);
	string GetStr(string s);
	int GetStaffValue(Staff *st,string varname);
	string GetSourceName(string s); //得到原名并转为小写
	void ChangeManagerID(int id,int new_manager_id,int old_manager_id = -1);//更改所属管理者编号,-1代表之前不存在manager_id
	void GetStaffList(vector<Staff*> &vs,Exp *filter);
	void ReadInfo(vector<string> &sp,int &id,string &name,int &age,STAFF_STATE &state);//从用户输入读取员工信息
	void ChangePage(size_t n);
	bool isTruth(Staff *st,Exp *filter);
	void PrintStaffs(set<int> &);
public:
	Database();
	~Database();
	void Load();
	void Save();
	void Show(Exp * filter = 0);//筛选后打印出数据
	void Update();//整体数据刷新
	void Execute(string com);//执行SQL命令
	//void FetchAll(DBData &d);//获取结果
	void GetData(vector<Staff*> v,const string filter = "");//获取数据，DB类可以解析过滤器语言
}; 

#endif
