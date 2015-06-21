#include "Database.h"
#include "Form.h"

//SLang
/*
#include "BigInt.h"
#include "SBuild.h"
#include "SVM.h"
*/
#include "./SLang/SLang.h"

bool Comparer::operator()(Staff *a,Staff *b){
	for (size_t i = 0;i < vks.size();++i){
		VALUE_KIND vk = vks[i];
		bool z = zs[i];
		if (vk == NAME){
			if(a->GetName() != b->GetName())return (a->GetName() < b->GetName()) ^ z;
		}else{
			int na = GetValue(a,vk);
			int nb = GetValue(b,vk);
			if (na != nb)return (na < nb)^z;
		}
	}
	return a->GetID() < b->GetID();
}
void Comparer::clear(){
	vks.clear();
	zs.clear();
}
void Comparer::push(VALUE_KIND vk,bool z){
	vks.push_back(vk);
	zs.push_back(z);
}
void Comparer::show(){
	if (vks.empty()){
		cout << "ID+" << endl;
		return;
	}
	//ID,NAME,KIND,AGE,STATE,MANAGER_ID,SALES,EVENTS
	string ds[] = {"ID","Name","Kind","Age","State","MangagerID","Sales","events"};
	bool first = true;
	for(size_t i = 0;i<vks.size();++i){
		if(!first)cout << " > ";
		first = false;
		int q = int(vks[i]);
		cout << ds[q];
		if (!zs[i])cout<<'+';
		else cout<<'-';
	}
	cout << endl;
}
int Comparer::GetValue(Staff *s,VALUE_KIND vk){
	switch (vk){
		case ID:
			return s->GetID();
		case KIND:
			return s->GetKind();
		case AGE:
			return s->GetAge();
		case STATE:
			return s->GetState();	
		case SALES:
			return s->GetAchievement().sales;
		case EVENTS:
			return s->GetAchievement().events;
		default:
			//不计算manager id
			return -1;
	}
}
Database::Database(){
	//读取帮助文档
	ifstream fin("help.txt");
	string str;
	while(!fin.eof()){
		getline(fin,str);
		helpText << str << endl;
	}
	fin.close();

	//参数
	cur_page = 1;
	max_page = 0;
	page_items = 10;
	autosave = true;
	changed = false;
	viewFilter = 0;
	filterName = "*";

	//读取数据
	try{
		Load();//读取员工数据
	}catch(const char * s){
		cout << s << endl;
	}


	cout << "当前共" << staffs.size() << "条数据" << endl;
	//Show();
}

Database::~Database(){
	//虚函数的delete应该怎样？
	for(auto &mp : staffs){
		Staff *p = mp.second;
		delete p;
		continue;
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
	if (!changed)return;
	changed = false;
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

		//注意，SalesManager由于继承了SalesMan,其指针也可以转为SalesMan*
		if(p->GetKind() == SALESMAN){
			SalesMan *sa = dynamic_cast<SalesMan*>(p);
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

		if (data.size() != 8)break;//数据有误
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
				ChangeManagerID(id,manager_id);
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
	Update();
}

int Database::GetStaffValue(Staff *st,string varname){
	if (varname == "id")return st->GetID();
	if (varname == "age")return st->GetAge();
	if (varname == "kind" || varname == "job")return int(st->GetKind());
	if (varname == "state")return int(st->GetState());
	if (varname == "manager_id"){
		if (st->GetKind() == SALESMAN){
			return dynamic_cast<SalesMan*>(st)->GetManagerID();
		}else{
			return -1;
		}
	}
	Achievement ach = st->GetAchievement();
	if (varname == "sales")return ach.sales;
	if (varname == "events")return ach.events;
	return -1;
}

bool Database::isTruth(Staff *st,Exp *filter){
	try{
		if (filter -> kind != EXP_OP)return true;
		//根据C++特性，能够自动短路的
		if (filter -> name == "and")
			return isTruth(st,filter->lv) && isTruth(st,filter->rv);
		if (filter -> name == "or")
			return isTruth(st,filter->lv) || isTruth(st,filter->rv);
		//剩下的是=,>,<,<>,!=,<=,>=
		Exp *lv = filter -> lv;
		Exp *rv = filter -> rv;	
		if (rv->kind == EXP_STR){
			//名字，字符串比较
			string name = st -> GetName();
			if (filter->name[0] == '='){
				//防止出现==输入
				return MatchStr(name,rv->name);
			}else if (filter->name == "!=" || filter -> name == "<>"){
				return !MatchStr(name,rv->name);
			}
			if (filter->name == "<")return name < rv->name;
			if (filter->name == ">")return name > rv->name;
			if (filter->name == "<=")return name <= rv->name;
			if (filter->name == ">=")return name >= rv->name;
		}else{
			int num = GetStaffValue(st,lv->name);
			int vn = STOI(rv->name);
			if (filter -> name[0] == '=')return num == vn;
			if (filter -> name == "!=" || filter -> name == "<>")return num != vn;
			if (filter -> name == "<")return num < vn;
			if (filter -> name == ">")return num > vn;
			if (filter -> name == "<=")return num <= vn;
			if (filter -> name == ">=")return num >= vn;
		}
	}catch(...){
		return false;
	}
	return true;//出现异常，暂时不处理
}

void Database::GetStaffList(vector<Staff*> &vs,Exp *filter){
	//确保过滤器语法正确
	for(auto &mp : staffs){
		bool can = true;
		if (filter){
			if (!isTruth(mp.second,filter)){
				can = false;
			}
		}
		if(can)vs.push_back(mp.second);
	}	
}

void Database::Report(Exp *repFilter,string repName){
	vector<Staff *> vs;
	GetStaffList(vs,repFilter);

	//显示业绩最高，最低的销售员，销售员们的平均业绩
	set<int> ma_ids,mi_ids;
	int ma_achi,mi_achi;//最高业绩,最低业绩
	BigInt sum = 0;

	bool first = true;
	int num = 0;
	for(auto p:vs){
		if (p->GetKind() != SALESMAN)continue;
		++num;
		Achievement achievement = p -> GetAchievement();
		int achi = achievement.sales + achievement.events;
		if (achi > ma_achi || first){
			ma_achi = achi;
			ma_ids.clear();
		}
		if (achi >= ma_achi)ma_ids.insert(p->GetID());

		if (achi < mi_achi || first){
			mi_achi = achi;
			mi_ids.clear();
		}
		if (achi <= mi_achi)mi_ids.insert(p->GetID());

		sum += achi;
		first = false;
	}
	if(num==0){
		cout << "没有成员满足筛选条件" << endl;
		return;
	}
	BigInt avg = sum / BigInt(num);

	cout << "当前筛选结果("<<repName<<")中("<<num<<"名销售员)：" << endl;
	cout << "业绩最低的销售员(业绩："<<mi_achi<<"):\n";
	PrintStaffs(mi_ids);	
	cout << "业绩最高的销售员(业绩："<<ma_achi<<"):\n";
	PrintStaffs(ma_ids);	
	cout << "平均业绩："<< avg << endl;
}

void Database::Show(Exp * filter){
	vector<Staff *> vs;
	if (!filter)
		filter = viewFilter;
	GetStaffList(vs,filter);
	sort(vs.begin(),vs.end(),comparer);
	max_page = ceil(vs.size() * 1.0 / page_items);
	if (cur_page > max_page)cur_page = max_page;
	if (cur_page < 1)cur_page = 1;

	Form form;
	//Header
	form.write(0,0,"ID");
	form.write(0,1,"Name");
	form.write(0,2,"Age");
	form.write(0,3,"State");
	form.write(0,4,"Kind");
	form.write(0,5,"Manager ID");
	form.write(0,6,"Sales");
	form.write(0,7,"Events");

	size_t start = (cur_page - 1) * page_items;
	size_t end = start + page_items;
	//[start,end)
	if (end > vs.size())end = vs.size();
	size_t items = end - start;
	for (size_t i = 0;i < items;++i){
		//多态
		size_t r = start + i;
		Staff *s = vs[r];
		form.write(i+1,0,ITOS(s->GetID()));
		form.write(i+1,1,s->GetName());
		form.write(i+1,2,ITOS(s->GetAge()));
		switch(s->GetState()){
			case ACTIVE:
				form.write(i+1,3,"在职");break;
			case RESIGN:
				form.write(i+1,3,"离职");break;
			case LEAVE:
				form.write(i+1,3,"请假");break;
		}
		switch(s->GetKind()){
			case SALESMAN:
				form.write(i+1,4,"SalesMan");break;
			case MANAGER:
				form.write(i+1,4,"Manager");break;
			case SALESMANAGER:
				form.write(i+1,4,"SalesManager");break;
			case STAFF:
				break;
		}
		SalesMan *psalesman = dynamic_cast<SalesMan*>(s);
	    Manager *pmanager = dynamic_cast<Manager*>(s);
		SalesManager *psalesmanager = dynamic_cast<SalesManager*>(s);
		//因为必定只有3类
		if (psalesmanager){
			form.write(i+1,5,"--");
			Achievement ac = psalesmanager -> GetAchievement();
			form.write(i+1,6,ITOS(ac.sales));
			form.write(i+1,7,ITOS(ac.events));
		}else{
			if (psalesman){
				int manager_id = psalesman -> GetManagerID();
				if (manager_id == -1){
					form.write(i+1,5,"NULL");
				}else{
					form.write(i+1,5,ITOS(manager_id));
				}
				Achievement ac = psalesman -> GetAchievement();
				form.write(i+1,6,ITOS(ac.sales));
				form.write(i+1,7,"--");
			}else{
				form.write(i+1,5,"--");
				Achievement ac = pmanager -> GetAchievement();
				form.write(i+1,6,"--");
				form.write(i+1,7,ITOS(ac.events));
			}
		}
	}	

	cout << "筛选条件：" << filterName << endl;
	form.print();
	cout << "第" << cur_page << "/" << max_page << "页 (共"<<vs.size()<<"项)" << "\t";
	cout << "( n:下一页 N:上一页 gg 39:跳转到39页 )" << endl;

} 

void Database::PrintStaffs(set<int> &s){
	if (s.size() == 0){
		cout << "无";
	}
	bool first = true;
	for (auto u:s){
		Staff *p = staffs[u];
		if (!first)cout<<",";
		cout << p->GetName()<<"("<<p->GetID()<<")";
		first = false;
 	}
	cout << endl;
} 

void Database::Check(){
	set<int> vs;
	for(auto &mp:staffs){
		Staff *p = mp.second;
		if (p -> GetKind() == SALESMAN){
			SalesMan *sa = dynamic_cast<SalesMan*>(p);
			int manager_id = sa -> GetManagerID();
			if (staffs.count(manager_id) == 0)vs.insert(p->GetID());
		}
	}
	if (vs.empty()){
		cout << "数据库正确" << endl;
	}else{
		cout << "以下销售者不在管理范围内，请更正" << endl;
		PrintStaffs(vs);
	}
}

void Database::Detail(int id){
	if (staffs.count(id) == 0){
		cout << "不存在编号为" << id <<"的成员" << endl;
		return;
	}
	Staff *p = staffs[id];
	cout << "姓名:" << p->GetName() <<"\t编号:"<<id<<"\t年龄:"<< p->GetAge()<<endl;
	static string ssk[] = {"员工","销售员","管理者","销售经理"};
	static string sst[] = {"在职","离职","请假"};
	cout << "职位:" << ssk[int(p->GetKind())];
   	cout << "\t状态:" << sst[int(p -> GetState())];
	cout << endl;
	SalesMan *sa = dynamic_cast<SalesMan*>(p);
	Manager *ma = dynamic_cast<Manager*>(p);
	SalesManager *sam = dynamic_cast<SalesManager*>(p);
	Achievement achi = p -> GetAchievement();
	switch (p->GetKind()){
		case SALESMAN:
			cout << "销售额:" << achi.sales << endl;
			cout << "所归属销售经理编号:" << sa -> GetManagerID();
			cout << endl;break;
		case MANAGER:
			cout << "完成事项数:" << achi.events;
			cout << endl;break;
		case SALESMANAGER:
			cout << "所管理销售人员的总销售额:" << achi.sales;
			cout << "\t完成事项数:" << achi.events << endl;
			cout << "所管理的销售人员: " << endl;
			PrintStaffs(slaves[p->GetID()]);
			break;
		default:
			cout << "资料有误" << endl;
	}
}

void Database::ChangePage(size_t n){
	//这里没必要警告了
	if (n < 1){
		n=1;
	}
	else if (n > max_page)n = max_page;
	cur_page = n;
}

void Database::Update(){ 
	//更新从属关系
	for (auto &s : slaves){
		int id = s.first;
		if (staffs.count(id) == 0)continue;
		Staff *pma = staffs[id];
		SalesManager *ma = dynamic_cast<SalesManager*>(pma);
		if(!pma)continue;
		ma -> slaves.clear(); 
		for (auto &slaveid : s.second){
			if (staffs.count(slaveid) == 0)continue;
			Staff *pps = staffs[slaveid];
			SalesMan *ps = dynamic_cast<SalesMan*>(pps);
			if(!ps)continue;
			ma -> slaves.push_back(ps);
		}
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
				if (bac[i][0] == '\'' || bac[i][0] == '\"'){
					t -> kind = EXP_STR;
					t -> name = bac[i].substr(1,bac[i].size()-2);
				}else{
					t -> kind = EXP_VAR;
					t -> name = StrLower(bac[i]);
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
	if (alias.count(StrLower(s)))return alias[s];
	return StrLower(s);
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
	if(s.size()>9)throw "请输入9位以内的数字";
	for (char c:s){
		if (c < '0' || c > '9')throw "请输入正确的正整数";
	}
	int r;
	sscanf(s.c_str(),"%d",&r);
	return r;
}
string Database::GetStr(string s){
	if (s.size()<=2)throw "请输入非空字符串";
	if (s[0] == '\'' && s[s.size()-1] == '\'')return s.substr(1,s.size()-2);
	if (s[0] == '\"' && s[s.size()-1] == '\"')return s.substr(1,s.size()-2);
	throw "请输入合法字符串";
}

void Database::ChangeManagerID(int id,int new_manager_id,int old_manager_id ){
	if (old_manager_id != -1){
		slaves[old_manager_id].erase(id);
	}
	slaves[new_manager_id].insert(id);
}


void Database::ReadInt(int &i){
	string buf;
	cin >> buf;
	i = GetInt(buf);
}

void Database::ReadInfo(vector<string> &sp,int &id,string &name,int &age,STAFF_STATE &state){

	int st = 0;
	try{
		name = GetStr(sp[1]);
	}catch (...){
		throw;
	}

	id = -1;
	age = -1;
	st = -1;

	try{
		id = GetInt(sp[0]);
		age = GetInt(sp[2]);
		st = GetInt(sp[3]);
	}catch (const char * s){
		cout << s << endl;
	}

	//check
	

	while (staffs.count(id) > 0 || id < 0){
		if (id >= 0)cout << "已存在编号为" << id << "的员工" << endl;
		else cout << "您需要输入非负数编号" << endl;
		cout << "请重新输入编号:" << endl;
		//cin >> id;
		//scanf("%d",&id);//c++的读入好像有点问题，貌似是缓存上的问题
		//读取上出了问题
		try{
			ReadInt(id);
		}catch(const char * s){
			id = -1;
			cout << s << endl;
		}
	}

	while (age > 200 || age < 0){
		cout << "这个年龄真的没问题吗？" << endl;
		cout << "请重新输入:" << endl;
		try{
			ReadInt(age);
		}catch(const char * s){
			cout << s << endl;
		}
	}

	while (st > 2 || st < 0){
		cout << "请输入正确的员工状态(0:在职,1:离职,2:请假)" << endl;
		try{
			ReadInt(st);
		}catch(const char * s){
			cout << s << endl;
		}
	}

	state = STAFF_STATE(st);
}

void Database::Execute(string com){
	size_t tail = com.size()-1;
	if (tail != 0 && isBlank(com[tail]))--tail;
	com = com.substr(0,tail+1);
	size_t poi = 0;
	string ex = NextStr(com,poi,' ');
	if (IgnoreLU(ex,"sort")){
		vector<string> vs;
		StrSplit(com,vs,' ');
		comparer.clear();
		for (size_t i = 1;i<vs.size();++i){
			char c = vs[i][vs[i].size()-1];
			bool has = false;
			bool z = 0;
			if (c == '-'){
				z = 1;
				has = true;
			}else if (c == '+')has=true;
			string s = vs[i].substr(0,vs[i].size()-size_t(has));
			//cout << s<<"--"<<vs[i]<<endl;
			if (IgnoreLU(s,"id"))comparer.push(ID,z);
			else if (IgnoreLU(s,"name"))comparer.push(NAME,z);
			else if (IgnoreLU(s,"kind"))comparer.push(KIND,z);
			else if (IgnoreLU(s,"age"))comparer.push(AGE,z);
			else if (IgnoreLU(s,"state"))comparer.push(STATE,z);
			else if (IgnoreLU(s,"sales"))comparer.push(SALES,z);
			else if (IgnoreLU(s,"events"))comparer.push(EVENTS,z);
		}
		cout << "当前排序优先级别为" << endl;
		comparer.show();
		Show();
	}else if (IgnoreLU(ex,"update") || IgnoreLU(ex,"set")){
		size_t wi = com.rfind("where");
		if (wi == string::npos){
			cout << "请填写过滤条件，过滤器的使用方法在帮助文档里。" << endl;
		}else{
			Exp* oldFilter = viewFilter;
			string oldName = filterName;
			try{
				if (com[poi] == '*'){
					filterName = "*";
					viewFilter = 0;
				}else{
					string condition = com.substr(wi+5);
					//cout << condition <<"!con"<<endl;
					viewFilter = Build(condition);
					filterName = condition; 
					//cout << "buiok" << endl;
				}
			}catch(const char *s){
				cout << s << endl;
			}
			//cout << "show"<<endl;
			Show();
			//cout << "oksh"<<endl;

			
			vector<Staff *> vs;
			GetStaffList(vs,viewFilter);

			string choice;
			while(true){
				cout << "是否要对这些数据进行操作？yes:是，no:否" << endl;
				cin >> choice;
				if (IgnoreLU(choice,"yes") || choice == "是"){
					SVM vm;
					SBuild bu;
					streamx ss;
					//srand(size_t(time(0)));
					bu.SetStream(ss);
					JumpSpace(com,poi);
					string temp = com.substr(poi,wi - poi);//使用SLang的语法糖 #展开
					string sl;
					for(auto &c:temp){
						if (c != ' ')sl+=c;
					}

					//Macro展开
					
					replace_all_distinct(sl,"=active","=0");
					replace_all_distinct(sl,"=resign","=1");
					replace_all_distinct(sl,"=leave","=2");
					replace_all_distinct(sl,"job=","kind=");
					

					replace_all_distinct(sl,"=salesmanager","=3");
					replace_all_distinct(sl,"=manager","=2");
					replace_all_distinct(sl,"=salesman","=1");
					//cout << sl << endl;
					

					ss << "#" << sl;
					//cout << "Slang"<<sl<<endl;
					SExp *e = bu.Build();//操作生成

					for (auto &ps:vs){
						vm.SetVar("id",ps->GetID());
						vm.SetVar("kind",int(ps->GetKind()));
						vm.SetVar("age",ps->GetAge());
						vm.SetVar("state",ps->GetState());
						vm.SetVar("manager_id",-1);
						if (ps->GetKind() == SALESMAN){
							SalesMan *psm = dynamic_cast<SalesMan*>(ps);
							vm.SetVar("manager_id",psm->GetManagerID());
						}
						vm.SetVar("sales",ps->GetAchievement().sales);
						vm.SetVar("events",ps->GetAchievement().events);

						vm.Eval(e);

						int id = vm.GetVar("id");
						int kind = vm.GetVar("kind");
						int age = vm.GetVar("age");
						int state = vm.GetVar("state");
						int manager_id = vm.GetVar("manager_id");
						int sales = vm.GetVar("sales");
						int events = vm.GetVar("events");

						cout << kind <<"k"<<endl;
						if (age < 200 && state <= 2 && kind <= 3 && kind>=1){
							ps -> ChangeAge(age);
							ps -> ChangeState(STAFF_STATE(state));
							if (ps -> GetKind() == SALESMAN){
								SalesMan *p = dynamic_cast<SalesMan*>(ps);
								p -> SetManagerID(manager_id);
								p -> SetSales(sales);
							}else{
								Manager *p = dynamic_cast<Manager*>(ps);
								p -> SetEvents(events);
							}
							//太麻烦，不做批量更改id了
							//牵连太大
							
							if (ps -> GetKind() != kind){
								//更改kind
								if (ps -> GetKind() == SALESMAN){
									SalesMan *p = dynamic_cast<SalesMan*>(ps);
									slaves[p -> GetManagerID()].erase(p -> GetID());
								}
								Staff *o;
								SalesMan *sa;
								Manager *ma;
								SalesManager *sam;
								switch(kind){
									case STAFF://forbid warning
									case SALESMAN:
										sa = new SalesMan(ps->GetID(),ps->GetName(),ps->GetAge(),ps->GetState());
										sa -> SetSales(sales);
										sa -> SetManagerID(manager_id);
										o = dynamic_cast<Staff*>(sa);break;
									case MANAGER:
										ma = new Manager(ps->GetID(),ps->GetName(),ps->GetAge(),ps->GetState());
										ma -> SetEvents(events);
										o = dynamic_cast<Staff*>(ma);break;
									case SALESMANAGER:
										sam = new SalesManager(ps->GetID(),ps->GetName(),ps->GetAge(),ps->GetState());
										sam -> SetEvents(events);
										o = dynamic_cast<Staff*>(sam);break;

								}
								int oid = ps->GetID();
								o -> ChangeID(oid);
								o -> ChangeName(ps->GetName());
								o -> ChangeAge(ps->GetAge());
								o -> ChangeState(ps->GetState());
								delete staffs[oid];
								staffs[oid] = o;
								Update();
							}
							/*
							if (ps -> GetID() != id && staffs.count(id)==0){
								int oid = ps -> GetID();
								//更新存储列表和从属关系
								staffs[id] = staffs[oid];
								staffs.erase(oid);
								slaves[id] = slaves[oid];
								slaves.erase(oid);
								Update();//低效率解决方案
							}*/
						}else{
							cout << "员工:" << ps->GetName() << "(" << ps->GetID() << ")更新数据时出现错误" << endl;
						}

					}

					cout << "更新操作完毕" << endl;
					filterName = oldName;
					viewFilter = oldFilter;
					changed = true;
					Update();
					Show();
					break;

				}else{
					if (IgnoreLU(choice,"no") || choice == "否"){
						cout << "已取消操作" <<endl;
						filterName = oldName;
						viewFilter = oldFilter;
						break;
					}else{
					}
				}
			}

		}
	}else if (IgnoreLU(ex,"filter") || ex == "f"){
		if (poi >= com.size()){
			cout << "请填写过滤条件，过滤器的使用方法在帮助文档里。" << endl;
		}else{
			try{
				if (com[poi] == '*'){
					delete viewFilter;
					filterName = "*";
					cout << "成功取消过滤器" << endl;
				}else{
					viewFilter = Build(com.substr(poi));
					filterName = com.substr(poi);
					cout << "成功设置过滤器："<< filterName << endl;
				}
			}catch(const char *s){
				cout << s << endl;
			}
		}
	}else if(IgnoreLU(ex,"report")){
		Exp* repFilter;
		string repName;
		if (poi >= com.size()){
			repName = "*";
			repFilter = 0;
			Report(repFilter,repName);
		}else{
			try{
				if (com[poi] == '*'){
					repName = "*";
					repFilter = 0;
				}else{
					repFilter = Build(com.substr(poi));
					repName = com.substr(poi);
				}
				Report(repFilter,repName);
			}catch(const char *s){
				cout << "筛选条件错误" << endl;
			}
		}
	}else if(IgnoreLU(ex,"delete") || ex == "d"){
		if (poi >= com.size()){
			cout << "请填写过滤条件，过滤器的使用方法在帮助文档里。" << endl;
		}else{
			Exp* oldFilter = viewFilter;
			string oldName = filterName;
			try{
				if (com[poi] == '*'){
					filterName = "*";
					viewFilter = 0;
				}else{
					viewFilter = Build(com.substr(poi));
					filterName = com.substr(poi);
				}
				
				Show();

			}catch(const char *s){
				cout << s << endl;
			}

			vector<Staff *> vs;
			GetStaffList(vs,viewFilter);

			string choice;
			while(true){
				cout << "是否删除这些数据？yes:是，no:否" << endl;
				cin >> choice;
				if (IgnoreLU(choice,"yes") || choice == "是"){
					for (auto &p : vs){
						staffs.erase(p->GetID());
						delete p;
					}
					Update();
					changed = true;
					cout << "删除成功" << endl;
					break;
				}else if (IgnoreLU(choice,"no") || choice == "否"){
					cout << "已经取消操作" << endl;
					break;
				}
			}

			viewFilter = oldFilter;
			filterName = oldName;

		}
	}else if (IgnoreLU(ex,"select")){
		if (poi >= com.size()){
			cout << "请填写过滤条件，过滤器的使用方法在帮助文档里。" << endl;
		}else{
			Exp* oldFilter = viewFilter;
			string oldName = filterName;
			try{
				if (com[poi] == '*'){
					filterName = "*";
					viewFilter = 0;
				}else{
					viewFilter = Build(com.substr(poi));
					filterName = com.substr(poi);
				}
				Show();
			}catch(const char *s){
				cout << s << endl;
			}
			filterName = oldName;
			viewFilter = oldFilter;
		}
	}else if (IgnoreLU(ex,"show")){
		string u = NextStr(com,poi,' ');
		if (!u.empty()){
			int n = STOI(u);
			if (n < 6){
				cout << "每页显示项目最少为6项" << endl;
			}else{
				//更新每页最大项目数
				if (n > 100)n = 100;//最多100项同一页显示
				page_items = n;
				cur_page = 1;
			}
		}
		Show();
	}else if (IgnoreLU(ex,"rename")){
		string u = NextStr(com,poi,' ');
		if (u.empty()){
			cout << "更改名字用法"<<endl;
			cout << "rename <id> 新名字"<<endl;
			cout << "如:rename 1 \'haha\'"<<endl;
		}else{
			try{
				int id = GetInt(u);
				u = NextStr(com,poi,'\n');
				if (u.empty())throw "请输入新的名字";
				string name = GetStr(u);
				if (staffs.count(id)){
					staffs[id]->ChangeName(name);
					cout << "更改成功！" << endl;
					changed = true;
				}else{
					cout << "不存在编号为" << id << "的员工!" << endl;
				}
			}catch(const char *s){
				cout << s << endl;
			}
		}
	}else if (ex == "n"){
		int n = 1;
		string u = NextStr(com,poi,' ');
		if (!u.empty())n = STOI(u);
		ChangePage(cur_page + n);
		Show();
	}else if (ex == "N"){
		int n = 1;
		string u = NextStr(com,poi,' ');
		if (!u.empty())n = STOI(u);
		ChangePage(cur_page - n);
		Show();
	}else if (ex == "gg"){
		int n = 1;
		string u = NextStr(com,poi,' ');
		if (!u.empty())n = STOI(u);
		ChangePage(n);
		Show();
	}else if (ex == "GG"){
		int n = max_page;
		string u = NextStr(com,poi,' ');
		if (!u.empty())n = STOI(u);
		ChangePage(n);
		Show();
	}else if (IgnoreLU(ex,"detail")){
		//显示细节
		string u = NextStr(com,poi,' ');
		if (!u.empty()){
			int n;
			n = STOI(u);
			Detail(n);
		}else{
			cout << "细节用法：" << endl;
			cout << "detail <id>，显示编号为id的员工的细节\n";
		}
	}else if (IgnoreLU(ex,"log") || IgnoreLU(ex,"check")){
		//进行检查
		Check();
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
			cout << "不需要填写销售经理所管销售者" << endl << endl;

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

						int id,age;
						string name;
						STAFF_STATE state;
						ReadInfo(sp,id,name,age,state);

						int manager_id = GetInt(sp[4]);
						int sales = GetInt(sp[5]);

						SalesMan *p = new SalesMan(id,name,age,state);
						p -> SetManagerID(manager_id);
						p -> SetSales(sales);
						staffs[id] = dynamic_cast<Staff*>(p);
						ChangeManagerID(id,manager_id);
						cout << "添加成功！" << endl;
						changed = true;
						Update();
					}catch(const char *s){
						cout << s <<endl;
					}catch(...){
						cout << "输入错误" << endl;
					}
					}else if (pro == "manager"){
					try{
						if (sp.size() < 5)throw "输入参数过少";
						//insert manager (id,name,age,state,events)
						
						int id,age;
						string name;
						STAFF_STATE state;
						ReadInfo(sp,id,name,age,state);

						int events = GetInt(sp[4]);

						Manager *p = new Manager(id,name,age,state);
						p -> SetEvents(events);
						staffs[id] = dynamic_cast<Staff*>(p);
						cout << "添加成功！" << endl;
						changed = true;
						Update();
					}catch(const char *s){
						cout << s <<endl;
					}catch(...){
						cout << "输入错误" << endl;
					}
				}else if (pro == "salesmanager"){
					try{
						if (sp.size() < 5)throw "输入参数过少";
						//insert salesmanager (id,name,age,state,events)
						
						int id,age;
						string name;
						STAFF_STATE state;
						ReadInfo(sp,id,name,age,state);

						int events = GetInt(sp[4]);

						SalesManager *p = new SalesManager(id,name,age,state);
						p -> SetEvents(events);
						staffs[id] = dynamic_cast<Staff*>(p);
						cout << "添加成功！" << endl;
						changed = true;
						Update();
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
				alias[target] = StrLower(source);
				cout << "成功添加别名" << target << " -> " << source << endl;
			}
		}
	}else{
		if(!ex.empty())cout << "无效指令" << endl;
	}
	if (autosave){
		Save();
	}
}
