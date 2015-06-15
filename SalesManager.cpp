#include "SalesManager.h"

SalesManager::SalesManager(int id,const string &name,int age,STAFF_STATE state):Manager(id,name,age,state){
	
}

Achievement SalesManager::GetAchievement(){
	//完成的管理事项个数及所负责的销售员的业绩之和

	//C++ 11
	int salesSum = 0;
	for (int &i : salesMen){
		Salesman *p = psalesMen[i];
		salesSum += p->GetAchievement().sales;
	}
	return Achievement(salesSum,events);
}

STAFF_KIND SalesManager::GetKind(){
	return SALESMANAGER;
}