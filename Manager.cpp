#include "Manager.h"

Manager::Manager(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state){
	_events = 0;
}
Manager::~Manager(){
}

Achievement Manager::GetAchievement(){
	return Achievement(0,_events);
}

STAFF_KIND Manager::GetKind(){
	return MANAGER;
} 

void Manager::SetEvents(int events){
	this -> _events = events;
} 
int Manager::GetEvents(){
	return _events;
}
