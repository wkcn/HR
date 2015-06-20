#include "HR.h"

HR::HR(){
}

void HR::Run(){
	cout << "欢迎使用人力资源管理系统" << endl;
	cout << "若需帮助，请输入help" << endl;
	string com;
	Database DB;
	//简单起见，这里不用封装了
	while (true){
		getline(cin,com);//获取输入命令
		if (IgnoreLU(com,"exit") || IgnoreLU(com,"quit")){
			DB.Save();
			cout << "Bye" << endl;
			break;
		}
		//StrSplit(com,sp,' ');
		DB.Execute(com);
	}
}
