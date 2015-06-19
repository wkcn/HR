#ifndef HR_H
#define HR_H

#include "Database.h"

class HR{
	public:
		HR();
		void Run();
	private:
		vector<Staff*> viewList;
};

#endif
