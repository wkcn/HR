#include "Staff.h"

Staff::Staff(int id,const string &name,int age,STAFF_STATE state){
	this -> _id = id;
	this -> _name = name;
	this -> _age = age;
	this -> _state = state;
}
Staff::~Staff(){}

void Staff::ChangeID(int id){
	this -> _id = id;
}
void Staff::ChangeName(const string &name){
	this -> _name = name;
} 
void Staff::ChangeAge(int age){
	this -> _age = age;
}
void Staff::ChangeState(STAFF_STATE state){
	this -> _state = state;
} 

STAFF_KIND Staff::GetKind(){
	return STAFF;
} 

int Staff::GetID(){
	return _id;
}
string Staff ::GetName(){
	return _name;
}
int Staff::GetAge(){
	return _age;
} 
STAFF_STATE  Staff::GetState(){
	return _state;
}


