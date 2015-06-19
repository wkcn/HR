#include "SalesManager.h"

SalesManager::SalesManager(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state),SalesMan(id,name,age,state),Manager(id,name,age,state){
	
}

SalesManager::~SalesManager(){}

Achievement SalesManager::GetAchievement(){
	//完成的管理事项个数及所负责的销售员的业绩之和
	//这个统一由数据库处理吧
	//char sql[128];
	//sprintf(sql,"select sales where manager_id=%d",id)；
	//int salesSum = GetSum(DB.execute(sql));
	int salesSum = 0;
	return Achievement(salesSum,_events);
}

STAFF_KIND SalesManager::GetKind( ){
	return SALESMANAGER;
}
