#ifndef SALES_MANAGER_H
#define SALES_MANAGER_H

#include "Salesman.h"
#include "Manager.h"

class SalesManager : public Salesman, public Manager{
	protected:
		set<int> salesMen;	//该销售经理管理的销售人员编号
	public:
		SalesManager(int id,const string &name,int age,STAFF_STATE state);
		Achievement GetAchievement();
		
		void AddSalesMan(int id);
		void RemoveSalesMan(int id);
}

#endif