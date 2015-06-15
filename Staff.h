#ifndef STAFF_H
#define STAFF_H

#include "Defines.h"

//员工类(虚基类)
class Staff{
	private:
		static enum STAFF_STATE{
			ACTIVE,RESIGN,LEAVE
		};// 员工状态（在职，离职，请假）
		static enum STAFF_KIND{
			STAFF,SALESMAN,MANAGER,SALESMANAGER
		};//员工类型
	protected:
		int id;
		string name;
		int age;
		STAFF_STATE state;
	public:
		Staff(int id,const string &name,int age,STAFF_STATE state);

		virtual Achievement GetAchievement() = 0;	//返回员工业绩
		virtual GetKind();
		
		void ChangeID(int id);
		void ChangeName(const string &name);
		void ChangeAge(int age);
		void ChangeState(STAFF_STATE state);\
		
		int GetID();
		string GetName();
		int GetAge();
		STAFF_STATE GetState();
};

#endif