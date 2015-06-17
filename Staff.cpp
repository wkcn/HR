#include "Staff.h"

Staff::Staff(int id,const string &name,int age,STAFF_STATE state){
	this -> id = id;
	this -> name = name;
	this -> age = age;
	this -> state = state;
}

void Staff::ChangeID(int id){
	this -> id = id;
}
void Staff::ChangeName(const string &name){
	this -> name = name;
}
void Staff::ChangeAge(int age){
	this -> age = age;
}
void Staff::ChangeState(STAFF_STATE state){
	this -> state = state;
}

STAFF_KIND Staff::GetKind(){
	return STAFF;
}

int Staff::GetID(){
	return id;
}
string Staff::GetName(){
	return name;
}
int Staff::GetAge(){
	return age;
}
STAFF_STATE Staff::GetState(){
	return state;
}


