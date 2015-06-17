#include "SalesMan.h"

SalesMan::SalesMan(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state){
	
}

Achievement SalesMan::GetAchievement(){
	return Achievement(sales,0);
}

STAFF_KIND SalesMan::GetKind(){
	return SALESMAN;
}

void SalesMan::SetManagerID(int id){
	manager_id = id;
}
void SalesMan::SetSales(int sales){
	this -> sales = sales;
}

int SalesMan::GetManagerID(){
	return manager_id;
}
int SalesMan::GetSales(){
	return sales;
}
