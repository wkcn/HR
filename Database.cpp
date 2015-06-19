#include "Database.h"
#include "Form.h"

Database::Database(){
	ifstream fin("help.txt");
	string str;
	while(!fin.eof()){
		getline(fin,str);
		helpText << str << endl;
	}
	fin.close();

	Load();//读取员工数据

	cur_page = 1;
	max_page = 0;
	page_items = 10;
}

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
	fout.close();
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
				//p = dynamic_cast<Staff*>(dynamic_cast<Manager*>(psalesManager));
				//使用虚继承后，不会出现二义性了（有两基类）
				p = dynamic_cast<Staff*>(psalesManager);
				break;
			default:
				throw "读取数据出错！";
	 	}
		
		staffs[id] = p;
	} 
	fin.close();
}

void Database::Show(){
	vector<Staff *> vs;
	for(auto &mp : staffs){
		vs.push_back(mp.second);
	}	
	sort(vs.begin(),vs.end(),Comparer(0));

	Form form;
	//Header
	form.write(0,0,"ID");
	form.write(0,1,"Name");
	form.write(0,2,"Age");
	form.write(0,3,"State");
	form.write(0,4,"Job");
	form.write(0,5,"Manager ID");
	form.write(0,6,"Sales");
	form.write(0,7,"Events");
	form.print();
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
	for (size_t i = 0; i < bac.size();++i){
		if (kinds[i]){
			//符号(这里都是二元操作符)
			Exp *t = new Exp();
			if(es.empty())throw "缺少右操作数";
			t -> rv = es.top();es.pop();
			if(es.empty())throw "缺少左操作数";
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
	return 0;
}

void Database::JumpSpace(const string &s,size_t &poi){
	while (poi < s.size() && isBlank(s[poi]))++poi;
}

string Database::NextStr(const string &s,size_t &poi,char c){
	JumpSpace(s,poi);
	string buf;
	for(;poi < s.size();++poi){
		if (s[poi] == c)break;
		else buf += s[poi];
	}
	++poi;
	return buf;
}

string Database::GetSourceName(string s){
	//得到原名并转为小写
	if (alias.count(StrLowwer(s)))return alias[s];
	return StrLowwer(s);
}

void Database::ShowPat(){
	//显示输入格式
	cout << "个人状态(state)：" << endl;
	cout << "在职：0" << endl;
	cout << "离职：1" << endl;
	cout << "请假：2" << endl;
	cout << "字符串需要用单引号或双引号括起" << endl;
}

int Database::GetInt(string s){
	for (char c:s){
		if (c < '0' || c > '9')throw "请输入正确的正整数";
	}
	int r;
	sscanf(s.c_str(),"%d",&r);
	return r;
}
string Database::GetStr(string s){
	if (s.size()<=2)throw "请输入非空字符串";
	if (s[0] == '\'' && s[s.size()-1] == '\'')return s;
	if (s[0] == '\"' && s[s.size()-1] == '\"')return s;
	throw "请输入合法字符串";
}

void Database::Execute(string com){
	size_t tail = com.size()-1;
	if (tail != 0 && isBlank(com[tail]))--tail;
	com = com.substr(0,tail+1);
	size_t poi = 0;
	string ex = NextStr(com,poi,' ');
	if (IgnoreLU(ex,"show")){
		Show();
	}else if (IgnoreLU(ex,"help")){
		cout << helpText.str()<<endl;//输出帮助文档
	}else if (IgnoreLU(ex,"insert") || ex == "i"){
		string pro = NextStr(com,poi,' ');
		if (pro.empty()){
			cout << "插入使用方法" << endl << endl;
			ShowPat();
			cout << endl;
			cout << "insert salesman (id,name,age,state,manager_id,sales)" << endl;
			cout << "其中manager_id是其所属销售经理编号，sales为销售额" << endl;
			cout << "如: insert salesman (0,'Jack',16,0,5,60)" << endl << endl;
			cout << "insert manager (id,name,age,state,events)" << endl;
			cout << "如: insert manager (1,'Sam',18,0,8)" << endl;
			cout << "其中events是完成的任务数" << endl << endl;

			cout << "insert salesmanager (id,name,age,state,events)" << endl;
			cout << "如: insert salesmanager (39,'SmallBoss',21,2,17)" << endl;
			cout << "不需要填写销售经理所管销售者,但销售经理的输入时间应先于所管销售者" << endl << endl;

		}else{
			JumpSpace(com,poi);
			//cout << poi <<endl;
			//cout << com[poi] << "-" << com[com.size()-1] << endl;
			if (com[poi] == '(' && com[com.size()-1] == ')'){
				pro = GetSourceName(pro);
				vector<string> sp;
				StrSplit(com.substr(poi+1,com.size()-poi-2),sp,',');
				if (pro == "salesman"){
					try{
						if (sp.size() < 6)throw "输入参数过少";
						//insert salesman (id,name,age,state,manager_id,sales)
						int id = GetInt(sp[0]);
						string name = GetStr(sp[1]);

						int age = GetInt(sp[2]);
						if (age > 200){
							throw "这个年龄没问题吗？";
						}
						int st = GetInt(sp[3]);
						if (st < 0 || st > 2)throw "输入员工状态错误！";

						STAFF_STATE state = STAFF_STATE(st);

						int manager_id = GetInt(sp[4]);
						int sales = GetInt(sp[5]);
						while (staffs.count(id) > 0){
							cout << "已存在编号为" << id << "的员工" << endl;
							cout << "请重新输入编号:" << endl;
							cin >> id;
						}
						SalesMan *p = new SalesMan(id,name,age,state);
						p -> SetManagerID(manager_id);
						p -> SetSales(sales);
						staffs[id] = dynamic_cast<Staff*>(p);
						cout << "添加成功！" << endl;
					}catch(const char *s){
						cout << s <<endl;
					}catch(...){
						cout << "输入错误" << endl;
					}
				}else if (pro == "manager"){
					try{
						if (sp.size() < 5)throw "输入参数过少";
						//insert manager (id,name,age,state,events)
						int id = GetInt(sp[0]);
						string name = GetStr(sp[1]);

						int age = GetInt(sp[2]);
						if (age > 200){
							throw "这个年龄没问题吗？";
						}
						int st = GetInt(sp[3]);
						if (st < 0 || st > 2)throw "输入员工状态错误！";

						STAFF_STATE state = STAFF_STATE(st);

						int events = GetInt(sp[4]);
						while (staffs.count(id) > 0){
							cout << "已存在编号为" << id << "的员工" << endl;
							cout << "请重新输入编号:" << endl;
							cin >> id;
						}
						Manager *p = new Manager(id,name,age,state);
						p -> SetEvents(events);
						staffs[id] = dynamic_cast<Staff*>(p);
						cout << "添加成功！" << endl;
					}catch(const char *s){
						cout << s <<endl;
					}catch(...){
						cout << "输入错误" << endl;
					}
				}else if (pro == "salesmanager"){
					try{
						if (sp.size() < 5)throw "输入参数过少";
						//insert salesmanager (id,name,age,state,events)
						int id = GetInt(sp[0]);
						string name = GetStr(sp[1]);

						int age = GetInt(sp[2]);

						if (age > 200){
							throw "这个年龄没问题吗？";
						}
						int st = GetInt(sp[3]);
						if (st < 0 || st > 2)throw "输入员工状态错误！";

						STAFF_STATE state = STAFF_STATE(st);

						int events = GetInt(sp[4]);
						while (staffs.count(id) > 0){
							cout << "已存在编号为" << id << "的员工" << endl;
							cout << "请重新输入编号:" << endl;
							cin >> id;
						}
						SalesManager *p = new SalesManager(id,name,age,state);
						p -> SetEvents(events);
						staffs[id] = dynamic_cast<Staff*>(p);
						cout << "添加成功！" << endl;
					}catch(const char *s){
						cout << s <<endl;
						cout << "请重新输入！" << endl;
					}catch(...){
						cout << "输入错误" << endl;
						cout << "请重新输入！" << endl;
					}
				}else{
					cout << "输入了无效职位名，职位名有salesman,manager,salesmanager 或他们的别名" << endl;
				}
			}else{
				cout << "括号不存在或不匹配,或者没有输入职位名称" << endl;
			}
		}
	}else if (IgnoreLU(ex,"alias")){
		string source = NextStr(com,poi,' ');
		string target = NextStr(com,poi,' ');
		if (target.empty()){
			cout << "别名使用方法:\nalias manager mg" << endl;
			cout << "即可用mg代替manager，删除该别名方法alias clear mg"<<endl;
			cout << "注意：别名不会保存，下次运行本应用时会重置" << endl;
			if (alias.size() > 0)cout << "当前别名：" << endl;
			else cout << "当前不存在别名" << endl;
			for (auto &mp : alias){
				cout << mp.first << " -> " << mp.second << endl;
			}
		}else{
			if (source == "clear"){
				alias.erase(target);
				cout << "成功删除别名" << target << endl;
			}else{
				alias[target] = StrLowwer(source);
				cout << "成功添加别名" << target << " -> " << source << endl;
			}
		}
	}
}
