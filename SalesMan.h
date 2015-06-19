#ifndef SALESMAN_H
#define SALESMAN_H

#include "Staff.h"

//这里采用虚继承
class SalesMan : virtual public Staff{
	protected:
		int _manager_id;	//该销售员所属经理的编号,数值为-1时说明未分配对应经理
		int _sales;		//销售额
	public:
		SalesMan(int id,const string &name,int age,STAFF_STATE state);
		~SalesMan();

		Achievement GetAchievement();
		STAFF_KIND GetKind();
		
		void SetManagerID(int id);
		void SetSales(int sales);
		
		int GetManagerID();
		int GetSales();
};

#endif
