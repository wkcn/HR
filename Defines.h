#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//为了方便，将实验要求稍作修改
//GetAchievement()函数返回的是Achievement结构体，
//记录的是业绩数据
struct Achievement{
	int sales;
	int events;
	Achievement{):sales(0),events(0){}
	Achievement(int sales,int events):sales(sales),events(events){};
}

#end