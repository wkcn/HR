#ifndef MANAGER_H
#define MANAGER_H

#include "Staff.h"

class Manager : public Staff{
	protected:
		int events;	//这里的翻译不太好
	public:
		void getAchievement();
}

#endif