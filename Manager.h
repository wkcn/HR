#ifndef MANAGER_H
#define MANAGER_H

#include "Staff.h"

//这里采用虚继承
class Manager : virtual public Staff{
	protected:
		int _events;	//这里的翻译不太好
	public:
		Manager(int id,const string &name,int age,STAFF_STATE state);
		~Manager();

		Achievement GetAchievement();
		STAFF_KIND GetKind();
		
		void SetEvents(int events);
		int GetEvents();
};

#endif
