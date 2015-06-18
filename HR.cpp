#include "HR.h"

HR::HR(){
	ifstream fin("help.txt");
	string str;
	while(!fin.eof()){
		getline(fin,str);
		helpText << str << endl;
	}
	fin.close();
}

void HR::Run(){
	cout << "欢迎使用人力资源管理系统" << endl;
	cout << "若需帮助，请输入help" << endl;
	string com;
	vector<string> sp;//分割指令用
	//简单起见，这里不用封装了
	while (true){
		getline(cin,com);//获取输入命令
		StrSplit(com,sp,' ');
		if (IgnoreLU(sp[0],"help")){
			ShowHelp();
		}
	}
}

void HR::ShowHelp(){
	cout << helpText.str();
}
