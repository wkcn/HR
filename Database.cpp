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

int Database::ISP(const string &op){
	if (op == "and" || op == "or"){
		return 10;
	}
	if (op[0] == '(')return 0;
	return 20;
}

int Database::ICP(const string &op){
	if (op == "and" || op == "or"){
		return 10;
	}
	if (op[0] == '(')return 999;
	return 20;
}

Exp* Database::Build(const string filter){
	
	vector<string> vs;
	string buf;
	//分词
	
	static const int symsSize = 18;
	static string syms[symsSize] = {"(",")",">","<",">=","<=","==","=","!=","<>","+","-","*","/","+=","-=","*=","/="};
	static string checkstr = "()><=!+-*/";

	for(size_t i = 0;i < filter.size(); ++i){
		if (filter[i] == ' '){
			if(!(buf.empty()))vs.push_back(buf);
			buf.clear();
		}else if (filter[i] == '\'' || filter[i] == '\"'){
			char q = filter[i];
			if(!(buf.empty()))vs.push_back(buf);
			buf.clear();
			buf += q;
			++i;
			for(;i < filter.size(); ++i){
				if (filter[i] == q){
					buf += q;
					break;
				}else{
					buf += filter[i];
				}
			}
			vs.push_back(buf);
			buf.clear();
		}else if (checkstr.find(filter[i]) != string::npos){
			if(!(buf.empty()))vs.push_back(buf);
			buf.clear();
			buf = filter[i];
			if (i+1 < filter.size()){
				string temp(buf);
				temp += filter[i + 1]; 
				bool ex = false;
				for (auto &s : syms){
					if (s == temp){
						ex = true;
						++i;
						break;
					}
				}
				if (ex)buf = temp;
			}
			vs.push_back(buf);
			buf.clear();	
		}else{
			buf += filter[i];
		}
	}
	if(!buf.empty())vs.push_back(buf);

	stack<string> op;
	vector<string> bac;
	vector<bool> kinds;//0为一般表达式，1为逻辑符号
	for(size_t i = 0;i < vs.size(); ++i){
		//cout << vs[i] << endl;
		if (vs[i] == ")"){
			bool can = false;
			while (!op.empty()){
				string q = op.top();
				op.pop();
				if (q == "("){
					can = true;
					break;
				}else{
					bac.push_back(q);
					kinds.push_back(1);
				}
			}
			if (!can){
				throw "括号不匹配";
			}
		}else{
			if (checkstr.find(vs[i][0]) != string::npos || vs[i][0] == '(' || vs[i] == "and" || vs[i] == "or"){
				while (!op.empty() && ISP(op.top()) >= ICP(vs[i])){
					string q = op.top();
					op.pop();
					bac.push_back(q);
					//cout <<q<<endl;
					kinds.push_back(1);
				}
				op.push(vs[i]);
			}else{
				bac.push_back(vs[i]);
				kinds.push_back(0);
			}
		}
	}
	while (!op.empty()){
		string q = op.top();
		op.pop();
		bac.push_back(q);
		kinds.push_back(1);
	}
/*
	for(auto v:bac){
		cout << v <<endl;
	}
*/
	stack<Exp*> es;
	try{
		for (size_t i = 0; i < bac.size();++i){
			if (kinds[i]){
				//符号(这里都是二元操作符)
				Exp *t = new Exp();
				t -> rv = es.top();es.pop();
				t -> lv = es.top();es.pop();
				t -> name = bac[i];
				t -> kind = EXP_OP;
				es.push(t);
			}else{
				Exp *t = new Exp();
				if (bac[i][0] == '-' || (bac[i][0] >= '0' && bac[i][0] <= '9')){
					t -> kind = EXP_NUM;
					t -> name = bac[i];
				}else{
					if (bac[i][0] == '\"' || bac[i][0] == '\"'){
						t -> kind = EXP_STR;
						t -> name = bac[i].substr(1,bac[i].size()-2);
					}else{
						t -> kind = EXP_VAR;
						t -> name = bac[i];
					}
				}
				es.push(t);
			}
		}
		Exp *root = es.top();es.pop();
		if (es.empty())return root;
		throw "您输入的表达式有误，不能完全解析";
	}catch(...){
		throw "您输入的表达式有误，缺少了参数";
	}	
	return 0;
}
