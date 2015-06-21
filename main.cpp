#include "Database.h"

int main(){
	cout << "-*-欢迎使用人力资源管理系统-*-\n" << endl;
	cout << "输入show打印员工列表。\n更多帮助请输入help\n" << endl;
	string com;//指令
	Database DB;//数据库
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
	return 0;
}
