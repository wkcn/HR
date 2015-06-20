#ifndef SALES_MANAGER_H
#define SALES_MANAGER_H

#include "SalesMan.h"
#include "Manager.h"

class SalesManager : public SalesMan, public Manager{
	private:
		//static map<int,Salesman*> psalesMen;	//由销售人员编号到指向销售人员的指针的映射
	protected:
		//set<int> salesMen;	//该销售经理管理的销售人员编号
	public:
		SalesManager(int id,const string &name,int age,STAFF_STATE state);
		~SalesManager();

		Achievement GetAchievement();
		STAFF_KIND GetKind();
		
		void AddSalesMan(int id);
		void RemoveSalesMan(int id);
	public:
		vector<SalesMan*> slaves;//所管理的销售人员的指针
};

#endif
