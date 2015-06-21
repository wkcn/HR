#include "SalesMan.h"

SalesMan::SalesMan(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state){
    _manager_id = -1;
}
SalesMan::~SalesMan(){}

Achievement SalesMan::GetAchievement(){
	return Achievement(_sales,0);
}

STAFF_KIND SalesMan::GetKind(){
	return SALESMAN;
}

void SalesMan::SetManagerID(int id){
	_manager_id = id;
}
void SalesMan::SetSales(int sales){
	this -> _sales = sales;
} 

int SalesMan::GetManagerID(){
	return _manager_id;
}
int SalesMan::GetSales(){
	return _sales;
}
