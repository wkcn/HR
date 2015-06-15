#ifndef MANAGER_H
#define MANAGER_H

#include "Staff.h"

class Manager : public Staff{
	protected:
		int events;	//这里的翻译不太好
	public:
		Manager(int id,const string &name,int age,STAFF_STATE state);

		Achievement GetAchievement();
		STAFF_KIND GetKind();
		
		void SetEvents(int events);
		int GetEvents();
}；

#endif