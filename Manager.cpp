#include "Manager.h"

Manager::Manager(int id,const string &name,int age,STAFF_STATE state):Staff(id,name,age,state){
	events = 0;
}

Achievement Manager::GetAchievement(){
	return Achievement(0,events);
}

STAFF_KIND Manager::GetKind(){
	return MANAGER;
}

void Manager::SetEvents(int events){
	this -> events = events;
}
int Manager::GetEvents(){
	return events;
}