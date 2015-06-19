#include "Form.h"

void Form::write(int r,int c,const string s){
	if (datas.size() <= size_t(r))datas.resize(r+1);
	if (datas[r].size() <= size_t(c)){
		datas[r].resize(c+1);
	}
	if (widths.size() <= size_t(c)){
		//注意，datas不是完整矩阵
		//datas[r]和widths的长度可能不等
		widths.resize(c+1);
	}
	size_t ow = widths[c];
	datas[r][c] = s;
	if (s.size() > ow){
		widths[c] = s.size();
	}else if (s.size() < ow){
		UpdateWidth(c);
	}
	print();
	cout << widths[c] <<endl;
}

void Form::write(int r,int c,int num){
	char cstr[64];
	sprintf(cstr,"%d",num);
	write(r,c,cstr);
}
void Form::write(int r,int c,double f){
	char cstr[64];
	sprintf(cstr,"%lf",f);
	write(r,c,cstr);
}

string Form::read(int r,int c){
	if (datas.size() >= size_t(r) || datas[r].size() >= size_t(c)) return "";
	return datas[r][c];
}

void Form::print(){
	if (datas.size() == 0){
		cout << "表格为空！"<<endl;
	}
	drawLine();
	//header
	printData(0,0);

	drawLine();

	if (datas.size() == 1)return;
	printData(1,datas.size()-1);
	//final line
	drawLine();
}

void Form::drawLine(){
	cout << '+';
	for(size_t i = 0;i < widths.size();++i){
		printC('-',2+widths[i]);
		cout << '+';
	}
	cout << endl;
}
void Form::printData(int start,int end){
	for (size_t r = size_t(start);r <= size_t(end); ++r){
		cout << '|';
		for (size_t i = 0;i < widths.size();++i){
			string con;
			if (r < datas.size() && i < datas[r].size())con = datas[r][i];
			cout << setw(widths[i]+1) << con << " |";
		}
		cout << endl;
	}
}

void Form::printC(char c,int n){
	for(int i = 0;i<n;++i)cout<<c;
}

void Form::UpdateWidth(int c){
	size_t ma = 0;
	for (size_t r = 0;r < datas.size(); ++r){
		if (size_t(c) < datas[r].size() && datas[r][c].size() > ma){
			ma = datas[r][c].size();
		}
	}
	widths[c] = ma;
}

int main(){
	Form f;
	f.write(0,0,"wwwaa");
	f.write(0,1,"ea");
	f.write(0,2,"ea");
	f.write(0,3,"wa");
	f.write(1,0,"wewaa");
	f.write(1,1,"eg");
	f.write(2,5,"wwwaa");
	f.write(2,4,"ea");
	f.print();
	return 0;
}
