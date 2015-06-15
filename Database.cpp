#include "Database.h"

Database::~Database(){
	for(auto &p : staffs)delete p;
}

void Database::Save(){
	ofstream fout("staff.dat");
	//Form Header
	fout<<"id#name#kind#age#state#manager_id#sales#events\n";	//注意，比如名字输入时要检查，不能存在#
	for(auto &p : staffs){
		fout << p->GetID() << "#";

		fout << p->GetName() << "#";

		fout << p->GetKind() << "#";

		fout << p->GetAge() << "#";

		fout << p->GetState() << "#";

		Salesman *sa = dynamic_cast<Salesman*>(p);
		if(!sa){
			fout << sa->GetManagerID() << "#";
		}else{
			fout << "-1#";
		}

		Achivement ach = p->GetAchivement();
		fout << ach->sales << "#" << ach->events << endl;
	}
}

void Database::Load(){

}