#ifndef SVM_H
#define SVM_H

//这个不是支持向量机，而是虚拟机~
//暂时不做函数，因为函数要有多个虚拟层

#include "Defines.h"
#include "SPar.h"

typedef BigInt (*SFunc)(SPar);	//让参数实例为参数，可能会影响效率

class SVM{
private:
	map<string, BigInt> vars;	//红黑树？我觉得内部也是通过new新建元素的，所以就不用指针了
	map<string, Vector<BigInt> > arrays;	//数组们	//将来再设置hash
	map<string, SFunc>funcs;	//C++函数
	map<string, SExp*>sfuncs;	//Slang函数
	Vector<string> path;		//搜索目录
	SVM *parent;
private:
	SExp* SearchFile(const string &filename,SExp *exp = 0,bool *ismacro = 0);
public:
	SVM();
	~SVM();
	void SetParent(SVM *);

	bool debug;
	friend class SPar;
	void AddFunc(const string &name, SFunc f);
	void AddPath(const string &name);

	void SetVar(const string &name,const BigInt &value);
	BigInt& GetVar(const string &);

	void ClearVar(const string &name);	//对某个变量清零
	void SetArray(const string &name, int id, const BigInt &value);
	BigInt GetArray(const string &name,int id);
	int GetArraySize(const string &name);
	void ClearArray(const string &name);
	Vector<BigInt>& FindArray(const string &name);

	void Restart();	//重启虚拟机
	BigInt GetValue(SExp *);	//对S表达式求值,这里使用拷贝，写起来更方便些，当然更好的方式是写虚拟寄存器分配
	BigInt Eval(SExp *);	//GetValue的别名
};

#endif
