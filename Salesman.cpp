#include "Salesman.h"

Salesman::Salesman(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state){
	
}

Achievement Salesman::GetAchievement(){
	return Achievement(sales,0);
}

void Salesman::SetManagerID(int id){
	manager_id = id;
}
void Salesman::SetSales(int sales){
	this -> sales = sales;
}

int Salesman::GetMangerID(){
	return manager_id;
}
int Salesman::GetSales(){
	return sales;
}