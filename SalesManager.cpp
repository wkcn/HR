#include "SalesManager.h"

SalesManager::SalesManager(int id,const string &name,int age,STAFF_STATE state):Manager(id,name,age,state){
	
}

Achievement SalesManager::GetAchievement(){
	//完成的管理事项个数及所负责的销售员的业绩之和
	//这个统一由数据库处理吧
	char sql[128];
	sprintf(sql,"select sales where manager_id=%d",id)；
	int salesSum = GetSum(DB.execute(sql));
	return Achievement(salesSum,events);
}

STAFF_KIND SalesManager::GetKind(){
	return SALESMANAGER;
}