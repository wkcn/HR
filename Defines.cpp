#include "Defines.h"

void StrSplit(const string &str,char c,vector<string> &res){
	res.clear();
	string buf;
	int i;
	for(i = 0;i < str.size();++i){
		if(str[i] == c){
			res.push_back(buf);
			buf.clear();
		}else{
			buf += str[i];
		}
	}
	if(!buf.empty())res.push_back(buf);

	return res;
}

int STOI(const stirng &str){
	int i;
	sscanf(str.c_str(),"%d",&i);
	return i;
}