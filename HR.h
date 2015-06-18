#ifndef HR_H
#define HR_H

#include "Database.h"

class HR{
	public:
		HR();
		void Run();
		void ShowHelp();
	private:
		vector<Staff*> viewList;
		stringstream helpText;
};

#endif
