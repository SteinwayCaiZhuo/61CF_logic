#include"Map.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
vector<vector<TPoint>>points;
void Map_Init()
{
	ifstream inFile("NewMap.csv",ios::in);//��ȡ��ͼ�ļ������ļ����ö��ŷָ��ɵı�
	string lineStr;	
	int x = 0, y = 99;//�ӵ�ͼ���Ͻǿ�ʼ���б���
	for (int i = 0; i < 100; i++)//�Ƚ���һ��100��100�Ķ�ά����
	{
		TPoint None;
		vector<TPoint> Line;
		for (int j = 0; j < 100; j++)
		{
			Line.push_back(None);
		}
		points.push_back(Line);
	}
	while (getline(inFile, lineStr)) {//���ж�ȡ	
		// ��ɶ�ά��ṹ		
		stringstream ss(lineStr);		
		string str;	
		// ���ն��ŷָ�		
		while (getline(ss, str, ','))//���ж�ȡ
		{
			TPoint* Place;
			Place = &points[x][y];
			Place->x = x;
			Place->y = y;
			if (str=="90")//ʳ��0
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 0;
				Place->soldier = -1;
			}
			else if (str == "48")//ʳ��1
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 1;
				Place->soldier = -1;
			}
			else if (str == "49")//ʳ��2
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 2;
				Place->soldier = -1;
			}
			else if (str == "107")//ʳ��3
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 3;
				Place->soldier = -1;
			}
			else if (str == "92")//ʳ��4
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 4;
				Place->soldier = -1;
			}
			else if (str == "95")//ʳ��5
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 5;
				Place->soldier = -1;
			}
			else if (str == "96")//ʳ��6
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 6;
				Place->soldier = -1;
			}
			else if (str == "190")//ʳ��7
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 7;
				Place->soldier = -1;
			}
			else if (str == "177")//ʳ��8
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 8;
				Place->soldier = -1;
			}
			else if (str == "155")//ʳ��9
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 9;
				Place->soldier = -1;
			}
			else if (str == "186")//ʳ��10
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 10;
				Place->soldier = -1;
			}
			else if (str == "198")//ʳ��11
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 11;
				Place->soldier = -1;
			}
			else if (str == "167")//ʳ��12
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 12;
				Place->soldier = -1;
			}
			else if (str == "150")//ʳ��13
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 13;
				Place->soldier = -1;
			}
			else if (str == "132")//ʳ��14
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 14;
				Place->soldier = -1;
			}
			else if (str == "174")//ʳ��15
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 15;
				Place->soldier = -1;
			}
			else if (str == "119")//ʳ��16
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 16;
				Place->soldier = -1;
			}
			else if (str == "209")//ʳ��17
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 17;
				Place->soldier = -1;
			}
			else if (str == "162")//ʳ��18
			{
				Place->land = Road;
				Place->occupied = 1;
				Place->occupied_type = Tower;
				Place->visible = 1;
				Place->tower = 18;
				Place->soldier = -1;
			}
			else if (str=="1")//ɭ��
			{
				Place->land = Forest;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="46")//ɽ��
			{
				Place->land = Mountain;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="2")//����
			{
				Place->land = River;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="18")//������
			{
				Place->land = Dorm;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="62")//��·
			{
				Place->land = Road;
				Place->occupied = 0;
				Place->occupied_type = UnKnown;
				Place->visible = 1;
				Place->soldier = -1;
				Place->tower = -1;
			}
			else if (str=="15")//��ѧ��
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

