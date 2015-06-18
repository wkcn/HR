#include "Database.h"

Database::~Database(){
	for(auto &mp : staffs){
		Staff *p = mp.second;
		SalesManager *psalesManager = dynamic_cast<SalesManager*>(p);
		if(psalesManager){
			delete psalesManager;
			continue;
		}
		SalesMan *psalesMan = dynamic_cast<SalesMan*>(p);
		if(psalesMan){
			delete psalesMan;
			continue;
		}
		Manager *pmanager = dynamic_cast<Manager*>(p);
		if(pmanager){
			delete pmanager;
		}
	}
}

void Database::Save(){
	ofstream fout("staff.dat");
	//Form Header
	fout<<"id#name#kind#age#state#manager_id#sales#events\n";	//注意，比如名字输入时要检查，不能存在#
	for(auto &mp : staffs){
		Staff *p = mp.second;

		fout << p->GetID() << "#";
		fout << p->GetName() << "#";
		fout << p->GetKind() << "#";
		fout << p->GetAge() << "#";
		fout << p->GetState() << "#";

		SalesMan *sa = dynamic_cast<SalesMan*>(p);
		if(!sa){
			fout << sa->GetManagerID() << "#";
		}else{
			fout << "-1#";
		}

		Achievement ach = p->GetAchievement();
		fout << ach.sales << "#" << ach.events << endl;
	}
}

void Database::Load(){ 
	fstream fin("staff.dat");
	if(fin.fail())return;
	string temp;
	getline(fin,temp);	//Read Form Header
	while(!fin.eof()){
		getline(fin,temp);
		vector<string> data;
		StrSplit(temp,data,'#');

		int id = STOI(data[0]);
		string name = data[1];
		STAFF_KIND kind = STAFF_KIND(STOI(data[2]));
		int age = STOI(data[3]);
		STAFF_STATE state = STAFF_STATE(STOI(data[4]));
		int manager_id = STOI(data[5]);
		int sales = STOI(data[6]);
		int events = STOI(data[7]);

		Staff *p;
		SalesMan *psalesman;
		Manager *pmanager;
		SalesManager *psalesManager;
		switch (kind){
			case SALESMAN:
				psalesman = new SalesMan(id,name,age,state);
				psalesman -> SetManagerID(manager_id);
				psalesman -> SetSales(sales);
				p = dynamic_cast<Staff*>(psalesman);
				break;
			case MANAGER:
				pmanager = new Manager(id,name,age,state);
				pmanager -> SetEvents(events);
				p = dynamic_cast<Staff*>(pmanager);
				break;
			case SALESMANAGER:
				psalesManager = new SalesManager(id,name,age,state);
				p = dynamic_cast<Staff*>(dynamic_cast<Manager*>(psalesManager));
				break;
			default:
				throw "读取数据出错！";
	 	}
		
		staffs[id] = p;
	} 
}
