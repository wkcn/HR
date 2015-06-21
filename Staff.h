#ifndef STAFF_H
#define STAFF_H

#include "Defines.h"

//员工类(虚基类)
class Staff{
	protected:
		int _id;
		string _name;
		int _age;
		STAFF_STATE _state;
	public:
		Staff(int id,const string &name,int age,STAFF_STATE state);
		virtual ~Staff();

		virtual Achievement GetAchievement() = 0;	//返回员工业绩
		virtual STAFF_KIND GetKind();
		
		void ChangeID(int id);
		void ChangeName(const string &name);
		void ChangeAge(int age);
		void ChangeState(STAFF_STATE state);
		
		int GetID();
		string GetName();
		int GetAge();
		STAFF_STATE GetState();
};

#endif
