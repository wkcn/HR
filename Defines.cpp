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

bool isBlank(char c){
	return (c == ' ' || c == '\t' || c == '\n' || c == '\b' || c == '\r' || c == -1);
}

//以下两个字符串替换函数摘录自 http://blog.csdn.net/cll131421/article/details/7959909
string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)   
{   
    while(true)   {   
        string::size_type   pos(0);   
        if(   (pos=str.find(old_value))!=string::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}   
string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value)   
{   
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {   
        if(   (pos=str.find(old_value,pos))!=string::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}   
