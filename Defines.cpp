#include "Defines.h"

void StrSplit(const string &str,vector<string> &res,char c){
	res.clear();
	string buf;
	unsigned int i;
	for(i = 0;i < str.size();++i){
		if(str[i] == c){
			if(!(buf.empty()))res.push_back(buf);
			buf.clear();
		}else{
			buf += str[i];
		}
	}
	if(!buf.empty())res.push_back(buf);
}

int STOI(const string &str){
	int i;
	sscanf(str.c_str(),"%d",&i);
	return i;
}

char WordLowwer(char c){
	if (c >= 'A' && c <= 'Z')return c - 'A' + 'a';
	return c;
}

bool IgnoreLU(const string a,const string b){
	if(a.size()!=b.size())return false;
	bool can = true;
	for(unsigned int i = 0;i<a.size();++i){
		//simply compare
		char q = WordLowwer(a[i]);
		char w = WordLowwer(b[i]);
		int t = q - w;
		if (t%26 != 0){
			can = false;
			break;
		}
	}
	return can;
}

string StrLowwer(const string &s){
	string res;
	for(const char &c:s){
		if(c >= 'A' && c <= 'Z'){
			res += c - 'A' + 'a';
		}else{
			res += c;
		}
	}
	return res;
}
