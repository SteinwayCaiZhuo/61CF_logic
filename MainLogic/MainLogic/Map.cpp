#include"Map.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
vector<vector<TPoint>>points;
void Map_Init()
{
	ifstream inFile("NewMap.csv",ios::in);//读取地图文件，该文件是用逗号分隔成的表
	string lineStr;	
	int x = 0, y = 99;//从地图左上角开始逐行遍历
	for (int i = 0; i < 100; i++)//先建立一个100×100的二维数组
	{
		TPoint None;
		vector<TPoint> Line;
		for (int j = 0; j < 100; j++)
		{
			Line.push_back(None);
		}
		points.push_back(Line);
	}
	while (getline(inFile, lineStr)) {//逐行读取	
		// 存成二维表结构		
		stringstream ss(lineStr);		
		string str;	
		// 按照逗号分隔		
		while (getline(ss, str, ','))//逐列读取
		{
			TPoint* Place;
			Place = &points[x][y];
			Place->x = x;
			Place->y = y;
			if (str=="90")//食堂0
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 0;
				Place->soldier = -1;
			}
			else if (str == "48")//食堂1
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 1;
				Place->soldier = -1;
			}
			else if (str == "49")//食堂2
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 2;
				Place->soldier = -1;
			}
			else if (str == "107")//食堂3
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 3;
				Place->soldier = -1;
			}
			else if (str == "92")//食堂4
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 4;
				Place->soldier = -1;
			}
			else if (str == "95")//食堂5
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 5;
				Place->soldier = -1;
			}
			else if (str == "96")//食堂6
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 6;
				Place->soldier = -1;
			}
			else if (str == "190")//食堂7
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 7;
				Place->soldier = -1;
			}
			else if (str == "177")//食堂8
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 8;
				Place->soldier = -1;
			}
			else if (str == "155")//食堂9
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 9;
				Place->soldier = -1;
			}
			else if (str == "186")//食堂10
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 10;
				Place->soldier = -1;
			}
			else if (str == "198")//食堂11
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 11;
				Place->soldier = -1;
			}
			else if (str == "167")//食堂12
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 12;
				Place->soldier = -1;
			}
			else if (str == "150")//食堂13
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 13;
				Place->soldier = -1;
			}
			else if (str == "132")//食堂14
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 14;
				Place->soldier = -1;
			}
			else if (str == "174")//食堂15
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 15;
				Place->soldier = -1;
			}
			else if (str == "119")//食堂16
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 16;
				Place->soldier = -1;
			}
			else if (str == "209")//食堂17
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 17;
				Place->soldier = -1;
			}
			else if (str == "162")//食堂18
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 18;
				Place->soldier = -1;
			}
			else if (str=="1")//森林
			{
				Place->land = Forest;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="46")//山脉
			{
				Place->land = Mountain;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="2")//河流
			{
				Place->land = River;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="18")//宿舍区
			{
				Place->land = Dorm;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="62")//道路
			{
				Place->land = Road;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="15")//教学区
			{
				Place->land = Classroom;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			x++;
		}
		x = 0;
		y--;
	}
	return ;
}

