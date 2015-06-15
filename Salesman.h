#ifndef SALESMAN_H
#define SALESMAN_H

#include "Staff.h"

class Salesman : public Staff{
	protected:
		int manager_id;	//该销售员所属经理的编号
		int sales;		//销售额
	public:
		Salesman(int id,const string &name,int age,STAFF_STATE state);
		void getAchievement();
}

#endif