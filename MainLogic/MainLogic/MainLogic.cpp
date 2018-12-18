#include<iostream>
#include<fstream>
#include<windows.h>
#include<tchar.h>
#include"definition.h"
#include"Map.h"

Info info;
vector<TSoldier>soldiers;//所有的士兵
//vector<vector<TPoint>>points;    //所有的地图
vector<TowerInfo>towers; //所有的食堂


vector<TSoldier>soldiers0;//玩家0视野内的士兵信息
vector<TSoldier>soldiers1;//玩家1视野内的士兵信息
vector<TSoldier>soldiers2;//玩家2视野内的士兵信息
vector<TSoldier>soldiers3;//玩家3视野内的士兵信息

vector<vector<TPoint>>points0;//玩家0视野内的地图信息
vector<vector<TPoint>>points1;//玩家1视野内的地图信息
vector<vector<TPoint>>points2;//玩家2视野内的地图信息
vector<vector<TPoint>>points3;//玩家3视野内的地图信息

vector<TowerInfo>towers0;//玩家0视野内的食堂信息
vector<TowerInfo>towers1;//玩家1视野内的食堂信息
vector<TowerInfo>towers2;//玩家2视野内的食堂信息
vector<TowerInfo>towers3;//玩家3视野内的食堂信息

int ID_timer = 1;//士兵ID的编号
//调用选手dll准备
typedef void(*Func)(Info& info);
HINSTANCE hDLL;
Func Player_ai;


void main()
{
	int i,j,k;
	//文件准备
	ofstream fout("result.txt");
	fout<<"FC16"<< endl;
	fout<<"==================================================="<<endl;
	//地图准备
    Map_Init();
	//塔准备
	for(i=0;i<19;i++)
	{
	    TowerInfo new_tower;
	    new_tower.id = i;
		new_tower.level = 1;
		new_tower.owner = -1;
		new_tower.boost = 0;
		new_tower.population = 10;
		new_tower.recruiting = false;
		new_tower.recruiting_round = -1;
		new_tower.resource_income = 4;
		new_tower.blood = 200;
		new_tower.max_blood = 200;
		towers.push_back(new_tower);
	}
	towers[0].position = points[6][86];
	towers[1].position = points[20][83];
	towers[2].position = points[33][89];
	towers[3].position = points[44][74];
	towers[4].position = points[61][71];
	towers[5].position = points[71][85];
	towers[6].position = points[83][74];
	towers[7].position = points[15][56];
	towers[8].position = points[15][40];
	towers[9].position = points[28][48];
	towers[10].position = points[51][51];
	towers[11].position = points[63][46];
	towers[12].position = points[73][43];
	towers[13].position = points[85][49];
	towers[14].position = points[8][13];
	towers[15].position = points[22][18];
	towers[16].position = points[39][7];
	towers[17].position = points[61][22];
	towers[18].position = points[77][12];
	for(i=0;i<19;i++)
	{
		towers[i].x_position = towers[i].position.x;
		towers[i].y_position = towers[i].position.y;
	}
	//Info基本信息初始化
	info.playerSize = 4;
	info.round = 0;
	info.towerNum = 19;
	info.playerAlive = 0;
	for(i=0;i<info.playerSize;i++)
	{
		PlayerInfo new_player;
		new_player.id = i;
		new_player.kill_num = 0;
		new_player.live = true;
		new_player.population = 0;
		new_player.max_population = 20+10;
		new_player.tower_num = 1;
		new_player.score = 0;
		new_player.soldier_num = 0;
		new_player.survival_round = 0;
		new_player.rank = -1;
		new_player.resource = 50;
		info.playerInfo.push_back(new_player);
	}
	points0 = points;
	points1 = points;
	points2 = points;
	points3 = points;
	towers0 = towers;
	towers1 = towers;
	towers2 = towers;
	towers3 = towers;
	//出生塔分配(暂时)
	towers[1].owner = 0;
	towers[5].owner = 1;
	towers[14].owner = 2;
	towers[18].owner = 3;

	while(info.round < 300)
	{
		//更新info的普遍信息(包括移动力削弱)
		info.playerAlive = 0;
		for(i = 0;i<info.playerSize;i++)
		{
			if(info.playerInfo[i].live)
				info.playerAlive++;
		}
		for(i = 0;i<soldiers.size();i++)
		{
			if(soldiers[i].position.land == Mountain)
			{
				if(soldiers[i].type == LightKnight||soldiers[i].type == HeavyKnight)
					soldiers[i].move_left -= 2;
				else if(soldiers[i].type == Mangonel)
					soldiers[i].move_left -= 1;
			}
		}
		//文件输出
		fout<<"Round"<<" "<<info.round+1<<endl;
		fout<<"PlayerAlive: "<<info.playerAlive<<endl;
		fout<<"Player0Info"<<endl;
		fout<<"Rank "<<info.playerInfo[0].rank<<" Score "<<(int)info.playerInfo[0].score<<" KillNum "<<info.playerInfo[0].kill_num
			<<" TowerNum "<<info.playerInfo[0].tower_num<<" SurvivalRound "<<info.playerInfo[0].survival_round
			<<" SoldierNum "<<info.playerInfo[0].soldier_num<<" Resource "<<(int)info.playerInfo[0].resource
			<<" MaxPopulation "<<info.playerInfo[0].max_population<<" Population "<<info.playerInfo[0].population<<endl;
		fout<<"Player1Info"<<endl;
		fout<<"Rank "<<info.playerInfo[1].rank<<" Score "<<(int)info.playerInfo[1].score<<" KillNum "<<info.playerInfo[1].kill_num
			<<" TowerNum "<<info.playerInfo[1].tower_num<<" SurvivalRound "<<info.playerInfo[1].survival_round
			<<" SoldierNum "<<info.playerInfo[1].soldier_num<<" Resource "<<(int)info.playerInfo[1].resource
			<<" MaxPopulation "<<info.playerInfo[1].max_population<<" Population "<<info.playerInfo[1].population<<endl;
		fout<<"Player2Info"<<endl;
		fout<<"Rank "<<info.playerInfo[2].rank<<" Score "<<(int)info.playerInfo[2].score<<" KillNum "<<info.playerInfo[2].kill_num
			<<" TowerNum "<<info.playerInfo[2].tower_num<<" SurvivalRound "<<info.playerInfo[2].survival_round
			<<" SoldierNum "<<info.playerInfo[2].soldier_num<<" Resource "<<(int)info.playerInfo[2].resource
			<<" MaxPopulation "<<info.playerInfo[2].max_population<<" Population "<<info.playerInfo[2].population<<endl;
		fout<<"Player3Info"<<endl;
		fout<<"Rank "<<info.playerInfo[3].rank<<" Score "<<(int)info.playerInfo[3].score<<" KillNum "<<info.playerInfo[3].kill_num
			<<" TowerNum "<<info.playerInfo[3].tower_num<<" SurvivalRound "<<info.playerInfo[3].survival_round
			<<" SoldierNum "<<info.playerInfo[3].soldier_num<<" Resource "<<(int)info.playerInfo[3].resource
			<<" MaxPopulation "<<info.playerInfo[3].max_population<<" Population "<<info.playerInfo[3].population<<endl;
		fout<<"TowerInfo"<<endl;
		
		for(i=0;i<info.towerNum;i++)
		{
			fout<<"TowerID "<<i<<" "<<"Owner "<<towers[i].owner<<" Level "<<towers[i].level
				<<" Blood "<<towers[i].blood<<" Recruiting "<<towers[i].recruiting
				<<" RecruitingRound "<<towers[i].recruiting_round<<" RecruitingType ";
				if(towers[i].recruiting)
				{
					switch (towers[i].soldier)
					{
					case LightInfantry:fout<<"LightInfantry";break;
					case LightArcher:fout<<"LightArcher";break;
					case LightKnight:fout<<"LightKnight";break;
					case Mangonel:fout<<"Mangonel";break;
					case HeavyInfantry:fout<<"HeavyInfantry";break;
					case HeavyArcher:fout<<"HeavyArcher";break;
					case HeavyKnight:fout<<"HeavyKnight";break;
					default:
						break;
					}
				}
				else
					fout<<"None";

					fout<<endl;
		}
		
		fout<<"SoldierInfo"<<endl;
		for(i=0;i<soldiers.size();i++)
		{
			fout<<"SoldierID "<<soldiers[i].id<<" Owner "<<soldiers[i].owner<<" Type ";
			switch (soldiers[i].type)
			{
			case LightInfantry:fout<<"LightInfantry";break;
			case LightArcher:fout<<"LightArcher";break;
			case LightKnight:fout<<"LightKnight";break;
			case Mangonel:fout<<"Mangonel";break;
			case HeavyInfantry:fout<<"HeavyInfantry";break;
			case HeavyArcher:fout<<"HeavyArcher";break;
			case HeavyKnight:fout<<"HeavyKnight";break;
			default:
					break;
			}
			fout<<" Level "<<soldiers[i].level<<" Blood "<<soldiers[i].blood<<" X_Position "<<soldiers[i].x_position
				<<" Y_Position "<<soldiers[i].y_position<<endl;
		}
		
		fout<<"CommandsInfo"<<endl;
		//玩家0视野准备
		for(i=0;i<soldiers.size()&&soldiers[i].owner==0;i++)
		{
			int x = soldiers[i].x_position;
			int y = soldiers[i].y_position;
			switch (soldiers[i].type)
			{			
			case LightInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
			case LightArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
			case LightKnight:
				{
					for( j=x-5;j<x+6;j++)
					{
						for(k=y-5;k<y+6;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
			case HeavyInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;			
							}
						}
					}
					break;
				}
			case HeavyArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
			case HeavyKnight:
				{
					for( j=x-4;j<x+5;j++)
					{
						for(k=y-4;k<y+5;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
			default:
				{
					for( j=x-2;j<x+3;j++)
					{
						for(k=y-2;k<y+3;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player0_visible = true;
							}
						}
					}
					break;
				}
				
			}
		}
		//塔的视野
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].owner != -1)
			{
				int x = towers[i].x_position;
				int y = towers[i].y_position;
				for( j=x-2-towers[i].level;j<x+3+towers[i].level;j++)
				{
					for(k=y-2-towers[i].level;k<y+3+towers[i].level;k++)
					{
						if(j>=0&&j<100&&k>=0&&k<100)
						{
							switch (towers[i].owner)
							{
							case 0:
								points[j][k].player0_visible = true;
								break;
							case 1:
								points[j][k].player1_visible = true;
								break;
							case 2:
								points[j][k].player2_visible = true;
								break;
							case 3:
								points[j][k].player3_visible = true;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
		//玩家0的代码前准备（地图、士兵、食堂信息更新）		
		for(j=0;j<100;j++)//获得地图
		{
			for(k=0;k<100;k++)
			{
				if(points[j][k].player0_visible)
				{
					points0[j][k] = points[j][k];
				}
				else
				{
					points0[j][k].occupied = false;
					points0[j][k].occupied_type = UnKnown;
					points0[j][k].soldier = -1;
					points0[j][k].tower = -1;
					points0[j][k].visible = false;
				}
			}
		}
		for(i=0;i<towers.size();i++)//获得塔信息
		{
			if(towers[i].position.player0_visible)
			{
				towers0[i] = towers[i];
			}
			else
			{
				towers0[i].blood = -1;
				towers0[i].owner = -1;
				towers0[i].level = -1;
				towers0[i].boost = -1;
				towers0[i].max_blood = -1;
				towers0[i].population = -1;
				towers0[i].position = points0[towers0[i].x_position][towers0[i].y_position];
				towers0[i].recruiting = false;
				towers0[i].recruiting_round = -1;
				towers0[i].resource_income = -1;
			}
		}
		//获得士兵信息
		soldiers0.clear();
		for(i=0;i<soldiers.size();i++)
		{
			if(points[soldiers[i].x_position][soldiers[i].y_position].player0_visible)
			{
				soldiers0.push_back(soldiers[i]);
			}
		}
		//info信息改变
		info.pointInfo = points0;
		info.towerInfo = towers0;
		info.soldierInfo = soldiers0;
		info.myID = 0;//info信息改变
		//玩家0代码
		hDLL = LoadLibrary(_TEXT("..//Debug//mydll.dll" ));
		if(hDLL!=NULL)
	    {
			
			Player_ai = (Func)GetProcAddress(hDLL,"player_ai");
			if(Player_ai != NULL)
			{
				Player_ai(info);
			}
	    }
	    FreeLibrary(hDLL);
		//玩家0的命令处理，更新全局信息，玩家0的信息清空，包括命令清空
		fout<<"Player0Commands"<<endl;
		for(i=0;i<info.myCommandList.m_commands.size();i++)
		{
			switch (info.myCommandList.m_commands[i].type)
			{
			case Attack:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int temp_x,temp_y;
						temp_x = info.myCommandList.m_commands[i].parameters[1];
						temp_y = info.myCommandList.m_commands[i].parameters[2];
						float distance2 = (soldiers[j].x_position-temp_x)*(soldiers[j].x_position-temp_x)+(soldiers[j].y_position-temp_y)*(soldiers[j].y_position-temp_y);
						float distance = sqrt(distance2);
						if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Soldier)
						//如果没打过且其占据目标位置类型为士兵
						{
							
							for(k=0;k<soldiers.size();k++)
							{
								if(soldiers[k].id==points[temp_x][temp_y].soldier)
								  break;
							}
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Soldier"<<" VictimID "<<soldiers[k].id<<endl;
							int real_attack = soldiers[i].attack;
							int real_armor = soldiers[k].armor;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_attack += 5;
							if(soldiers[j].type == Mangonel&&soldiers[j].position.land == Classroom)
								real_attack += 5;
							if((soldiers[k].type == LightArcher||soldiers[k].type == HeavyArcher)&&soldiers[k].position.land == Forest)
								real_armor += 5;
							if((soldiers[k].type == HeavyInfantry||soldiers[k].type == HeavyArcher||soldiers[k].type == HeavyKnight)&&soldiers[k].position.land == Dorm)
								real_armor += 5;
							soldiers[k].blood -= real_attack-real_armor;
							soldiers[j].attackable=false;
							if(soldiers[k].blood<=0)//目标士兵死亡
							{
								//获得经验(处理升级信息)
								int available_soldier_num=0;
										
								for(int t1=0;t1<soldiers.size();t1++)
								{
									if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
									{
										available_soldier_num++;
									}
								}
								if((125-50*available_soldier_num)>50)
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += (125-50*available_soldier_num);
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
										}
									}
								}
								else
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += 50;
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
									    }
									}
								}
										
								//更新地面信息，势力击杀数，人口数变动
								points[soldiers[k].x_position][soldiers[k].y_position].occupied = false;
								points[soldiers[k].x_position][soldiers[k].y_position].occupied_type = None;
								points[soldiers[k].x_position][soldiers[k].y_position].soldier = -1;
								info.playerInfo[soldiers[k].owner].soldier_num--;
								switch (soldiers[k].type)
								{
								    case LightInfantry:info.playerInfo[soldiers[k].owner].population -= 2;break;
									case LightArcher:info.playerInfo[soldiers[k].owner].population -= 3;break;
									case LightKnight:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case Mangonel:info.playerInfo[soldiers[k].owner].population -= 10;break;
									case HeavyInfantry:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case HeavyArcher:info.playerInfo[soldiers[k].owner].population -= 8;break;
									case HeavyKnight:info.playerInfo[soldiers[k].owner].population -= 10;break;
									default:
										break;
								}
										
								info.playerInfo[info.myID].kill_num ++;
								//删除死亡士兵
								soldiers.erase(soldiers.begin()+k);
							}
						}
						else if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Tower)
						//如果能打且目标位置为塔
						{
							int t_tower_id = points[temp_x][temp_y].tower;
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Tower"<<" VictimID "<<t_tower_id<<endl;
							int real_damage = soldiers[j].attack;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_damage += 5;
							else if(soldiers[j].type == Mangonel)
							{
								if(soldiers[j].position.land == Classroom)
									real_damage = 5*(real_damage+5);
								else
									real_damage = 5*real_damage;
							}

							towers[t_tower_id].blood -= real_damage;
								//判断塔的易主问题
							if(towers[t_tower_id].blood<=0)
							{
								if(towers[t_tower_id].owner==-1)
								{
									towers[t_tower_id].blood = towers[t_tower_id].max_blood;
									towers[t_tower_id].owner = info.myID;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
								else
								{
										
									towers[t_tower_id].level = 1;
									towers[t_tower_id].boost = 0;
									towers[t_tower_id].max_blood = 200;
									towers[t_tower_id].blood = 100;
									towers[t_tower_id].resource_income = 4;	
									info.playerInfo[towers[t_tower_id].owner].max_population -= towers[t_tower_id].population;
									info.playerInfo[towers[t_tower_id].owner].tower_num --;
									towers[t_tower_id].population = 10;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									towers[t_tower_id].owner = info.myID;	
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
							}
							
                        }
					}
					break;
				}
			case Move:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int tx,ty;
						int water_judge = 0;//0表示路径上无水，1表示有水
						tx = soldiers[j].x_position;
						ty = soldiers[j].y_position;
						MoveDirection direction = (MoveDirection)info.myCommandList.m_commands[i].parameters[1];
						TMove  distance = info.myCommandList.m_commands[i].parameters[2];
						if(distance<0)
							distance = 0;
						if(distance<soldiers[j].move_left&&distance>0)
							//能够移动
						{
							switch (direction)
							{
							case UP:
								if((ty+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty+k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty+distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty+distance].occupied = true;
									    points[tx][ty+distance].occupied_type = Soldier;
									    points[tx][ty+distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty+distance;
									    soldiers[j].position = points[tx][ty+distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"UP"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case DOWN:
								if((ty-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty-k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty-distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty-distance].occupied = true;
									    points[tx][ty-distance].occupied_type = Soldier;
									    points[tx][ty-distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty-distance;
									    soldiers[j].position = points[tx][ty-distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"DOWN"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case LEFT:
								if((tx-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx-k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx-distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx-distance][ty].occupied = true;
									    points[tx-distance][ty].occupied_type = Soldier;
									    points[tx-distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx-distance;
									    soldiers[j].position = points[tx-distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"LEFT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case RIGHT:
								if((tx+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx+k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx+distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx+distance][ty].occupied = true;
									    points[tx+distance][ty].occupied_type = Soldier;
									    points[tx+distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx+distance;
									    soldiers[j].position = points[tx+distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"RIGHT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							default:
								break;
							}
						}
					    
					}
					break;
				}
			case Upgrade:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						switch (towers[j].level)
						{
						case 1:
							if(info.playerInfo[info.myID].resource>=80)
							{
								towers[j].max_blood = 250;
								towers[j].blood += 50;
								towers[j].boost = 1;
								towers[j].resource_income = 6;
								towers[j].population = 15;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 80;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						case 2:
							if(info.playerInfo[info.myID].resource>=150)
							{
								towers[j].max_blood = 300;
								towers[j].blood += 50;
								towers[j].boost = 2;
								towers[j].resource_income = 8;
								towers[j].population = 20;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 150;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						default:
							break;
						}
					}
					break;
				}
			case Produce:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						TSoldierType type;
						type = (TSoldierType)info.myCommandList.m_commands[i].parameters[1];
						if(towers[j].recruiting == false)
						{
							switch (type)
							{
							case LightInfantry:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=2)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 4-towers[j].boost;
									towers[j].soldier = LightInfantry;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightArcher:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=3)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 5-towers[j].boost;
									towers[j].soldier = LightArcher;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightKnight:
								if(info.playerInfo[info.myID].resource >= 50&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 6-towers[j].boost;
									towers[j].soldier = LightKnight;
									info.playerInfo[info.myID].resource -= 50;
								}
								break;
							case Mangonel:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 8-towers[j].boost;
									towers[j].soldier = Mangonel;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							case HeavyInfantry:
								if(info.playerInfo[info.myID].resource >= 55&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 7-towers[j].boost;
									towers[j].soldier = HeavyInfantry;
									info.playerInfo[info.myID].resource -= 55;
								}
								break;
							case HeavyArcher:
								if(info.playerInfo[info.myID].resource >= 60&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=8)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyArcher;
									info.playerInfo[info.myID].resource -= 60;
								}
								break;
							case HeavyKnight:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyKnight;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							default:
								break;
							}
						}
					}
					break;
				}
			default:
				break;
			}
		}
		info.myCommandList.m_commands.clear();
		//玩家1视野准备
		for(i=0;i<soldiers.size()&&soldiers[i].owner==1;i++)
		{
			int x = soldiers[i].x_position;
			int y = soldiers[i].y_position;
			switch (soldiers[i].type)
			{			
			case LightInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
			case LightArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
			case LightKnight:
				{
					for( j=x-5;j<x+6;j++)
					{
						for(k=y-5;k<y+6;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
			case HeavyInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;			
							}
						}
					}
					break;
				}
			case HeavyArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
			case HeavyKnight:
				{
					for( j=x-4;j<x+5;j++)
					{
						for(k=y-4;k<y+5;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
			default:
				{
					for( j=x-2;j<x+3;j++)
					{
						for(k=y-2;k<y+3;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player1_visible = true;
							}
						}
					}
					break;
				}
				
			}
		}
		//塔的视野
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].owner != -1)
			{
				int x = towers[i].x_position;
				int y = towers[i].y_position;
				for( j=x-2-towers[i].level;j<x+3+towers[i].level;j++)
				{
					for(k=y-2-towers[i].level;k<y+3+towers[i].level;k++)
					{
						if(j>=0&&j<100&&k>=0&&k<100)
						{
							switch (towers[i].owner)
							{
							case 0:
								points[j][k].player0_visible = true;
								break;
							case 1:
								points[j][k].player1_visible = true;
								break;
							case 2:
								points[j][k].player2_visible = true;
								break;
							case 3:
								points[j][k].player3_visible = true;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
		//玩家1的代码前准备（地图、士兵、食堂获取）
		for(j=0;j<100;j++)//获得地图
		{
			for(k=0;k<100;k++)
			{
				if(points[j][k].player1_visible)
				{
					points1[j][k] = points[j][k];
				}
				else
				{
					points1[j][k].occupied = false;
					points1[j][k].occupied_type = UnKnown;
					points1[j][k].soldier = -1;
					points1[j][k].tower = -1;
					points1[j][k].visible = false;
				}
			}
		}
		for(i=0;i<towers.size();i++)//获得塔信息
		{
			if(towers[i].position.player1_visible)
			{
				towers1[i] = towers[i];
			}
			else
			{
				towers1[i].blood = -1;
				towers1[i].owner = -1;
				towers1[i].level = -1;
				towers1[i].boost = -1;
				towers1[i].max_blood = -1;
				towers1[i].population = -1;
				towers1[i].position = points1[towers1[i].x_position][towers1[i].y_position];
				towers1[i].recruiting = false;
				towers1[i].recruiting_round = -1;
				towers1[i].resource_income = -1;
			}
		}
		//获得士兵信息
		soldiers1.clear();
		for(i=0;i<soldiers.size();i++)
		{
			if(points[soldiers[i].x_position][soldiers[i].y_position].player1_visible)
			{
				soldiers1.push_back(soldiers[i]);
			}
		}
		//info信息更新
		info.pointInfo = points1;
		info.towerInfo = towers1;
		info.soldierInfo = soldiers1;
		info.myID = 1;
		//玩家1代码
		hDLL = LoadLibrary(_TEXT("..//Debug//mydll.dll" ));
		if(hDLL!=NULL)
	    {
			
			Player_ai = (Func)GetProcAddress(hDLL,"player_ai");
			if(Player_ai != NULL)
			{
				Player_ai(info);
			}
	    }
	    FreeLibrary(hDLL);
		//玩家1命令处理
		fout<<"Player1Commands"<<endl;
		for(i=0;i<info.myCommandList.m_commands.size();i++)
		{
			switch (info.myCommandList.m_commands[i].type)
			{
			case Attack:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int temp_x,temp_y;
						temp_x = info.myCommandList.m_commands[i].parameters[1];
						temp_y = info.myCommandList.m_commands[i].parameters[2];
						float distance2 = (soldiers[j].x_position-temp_x)*(soldiers[j].x_position-temp_x)+(soldiers[j].y_position-temp_y)*(soldiers[j].y_position-temp_y);
						float distance = sqrt(distance2);
						if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Soldier)
						//如果没打过且其占据目标位置类型为士兵
						{
							
							for(k=0;k<soldiers.size();k++)
							{
								if(soldiers[k].id==points[temp_x][temp_y].soldier)
								  break;
							}
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Soldier"<<" VictimID "<<soldiers[k].id<<endl;
							int real_attack = soldiers[i].attack;
							int real_armor = soldiers[k].armor;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_attack += 5;
							if(soldiers[j].type == Mangonel&&soldiers[j].position.land == Classroom)
								real_attack += 5;
							if((soldiers[k].type == LightArcher||soldiers[k].type == HeavyArcher)&&soldiers[k].position.land == Forest)
								real_armor += 5;
							if((soldiers[k].type == HeavyInfantry||soldiers[k].type == HeavyArcher||soldiers[k].type == HeavyKnight)&&soldiers[k].position.land == Dorm)
								real_armor += 5;
							soldiers[k].blood -= real_attack-real_armor;
							soldiers[j].attackable=false;
							if(soldiers[k].blood<=0)//目标士兵死亡
							{
								//获得经验(处理升级信息)
								int available_soldier_num=0;
										
								for(int t1=0;t1<soldiers.size();t1++)
								{
									if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
									{
										available_soldier_num++;
									}
								}
								if((125-50*available_soldier_num)>50)
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += (125-50*available_soldier_num);
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
										}
									}
								}
								else
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += 50;
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
									    }
									}
								}
										
								//更新地面信息，势力击杀数，人口数变动
								points[soldiers[k].x_position][soldiers[k].y_position].occupied = false;
								points[soldiers[k].x_position][soldiers[k].y_position].occupied_type = None;
								points[soldiers[k].x_position][soldiers[k].y_position].soldier = -1;
								info.playerInfo[soldiers[k].owner].soldier_num--;
								switch (soldiers[k].type)
								{
								    case LightInfantry:info.playerInfo[soldiers[k].owner].population -= 2;break;
									case LightArcher:info.playerInfo[soldiers[k].owner].population -= 3;break;
									case LightKnight:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case Mangonel:info.playerInfo[soldiers[k].owner].population -= 10;break;
									case HeavyInfantry:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case HeavyArcher:info.playerInfo[soldiers[k].owner].population -= 8;break;
									case HeavyKnight:info.playerInfo[soldiers[k].owner].population -= 10;break;
									default:
										break;
								}
										
								info.playerInfo[info.myID].kill_num ++;
								//删除死亡士兵
								soldiers.erase(soldiers.begin()+k);
							}
						}
						else if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Tower)
						//如果能打且目标位置为塔
						{
							int t_tower_id = points[temp_x][temp_y].tower;
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Tower"<<" VictimID "<<t_tower_id<<endl;
							int real_damage = soldiers[j].attack;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_damage += 5;
							else if(soldiers[j].type == Mangonel)
							{
								if(soldiers[j].position.land == Classroom)
									real_damage = 5*(real_damage+5);
								else
									real_damage = 5*real_damage;
							}

							towers[t_tower_id].blood -= real_damage;
								//判断塔的易主问题
							if(towers[t_tower_id].blood<=0)
							{
								if(towers[t_tower_id].owner==-1)
								{
									towers[t_tower_id].blood = towers[t_tower_id].max_blood;
									towers[t_tower_id].owner = info.myID;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
								else
								{
										
									towers[t_tower_id].level = 1;
									towers[t_tower_id].boost = 0;
									towers[t_tower_id].max_blood = 200;
									towers[t_tower_id].blood = 100;
									towers[t_tower_id].resource_income = 4;	
									info.playerInfo[towers[t_tower_id].owner].max_population -= towers[t_tower_id].population;
									info.playerInfo[towers[t_tower_id].owner].tower_num --;
									towers[t_tower_id].population = 10;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									towers[t_tower_id].owner = info.myID;	
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
							}
							
                        }
					}
					break;
				}
			case Move:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int tx,ty;
						int water_judge = 0;//0表示路径上无水，1表示有水
						tx = soldiers[j].x_position;
						ty = soldiers[j].y_position;
						MoveDirection direction = (MoveDirection)info.myCommandList.m_commands[i].parameters[1];
						TMove  distance = info.myCommandList.m_commands[i].parameters[2];
						if(distance<0)
							distance = 0;
						if(distance<soldiers[j].move_left&&distance>0)
							//能够移动
						{
							switch (direction)
							{
							case UP:
								if((ty+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty+k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty+distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty+distance].occupied = true;
									    points[tx][ty+distance].occupied_type = Soldier;
									    points[tx][ty+distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty+distance;
									    soldiers[j].position = points[tx][ty+distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"UP"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case DOWN:
								if((ty-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty-k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty-distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty-distance].occupied = true;
									    points[tx][ty-distance].occupied_type = Soldier;
									    points[tx][ty-distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty-distance;
									    soldiers[j].position = points[tx][ty-distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"DOWN"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case LEFT:
								if((tx-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx-k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx-distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx-distance][ty].occupied = true;
									    points[tx-distance][ty].occupied_type = Soldier;
									    points[tx-distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx-distance;
									    soldiers[j].position = points[tx-distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"LEFT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case RIGHT:
								if((tx+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx+k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx+distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx+distance][ty].occupied = true;
									    points[tx+distance][ty].occupied_type = Soldier;
									    points[tx+distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx+distance;
									    soldiers[j].position = points[tx+distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"RIGHT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							default:
								break;
							}
						}
					    
					}
					break;
				}
			case Upgrade:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						switch (towers[j].level)
						{
						case 1:
							if(info.playerInfo[info.myID].resource>=80)
							{
								towers[j].max_blood = 250;
								towers[j].blood += 50;
								towers[j].boost = 1;
								towers[j].resource_income = 6;
								towers[j].population = 15;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 80;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						case 2:
							if(info.playerInfo[info.myID].resource>=150)
							{
								towers[j].max_blood = 300;
								towers[j].blood += 50;
								towers[j].boost = 2;
								towers[j].resource_income = 8;
								towers[j].population = 20;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 150;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						default:
							break;
						}
					}
					break;
				}
			case Produce:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						TSoldierType type;
						type = (TSoldierType)info.myCommandList.m_commands[i].parameters[1];
						if(towers[j].recruiting == false)
						{
							switch (type)
							{
							case LightInfantry:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=2)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 4-towers[j].boost;
									towers[j].soldier = LightInfantry;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightArcher:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=3)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 5-towers[j].boost;
									towers[j].soldier = LightArcher;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightKnight:
								if(info.playerInfo[info.myID].resource >= 50&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 6-towers[j].boost;
									towers[j].soldier = LightKnight;
									info.playerInfo[info.myID].resource -= 50;
								}
								break;
							case Mangonel:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 8-towers[j].boost;
									towers[j].soldier = Mangonel;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							case HeavyInfantry:
								if(info.playerInfo[info.myID].resource >= 55&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 7-towers[j].boost;
									towers[j].soldier = HeavyInfantry;
									info.playerInfo[info.myID].resource -= 55;
								}
								break;
							case HeavyArcher:
								if(info.playerInfo[info.myID].resource >= 60&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=8)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyArcher;
									info.playerInfo[info.myID].resource -= 60;
								}
								break;
							case HeavyKnight:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyKnight;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							default:
								break;
							}
						}
					}
					break;
				}
			default:
				break;
			}
		}
		info.myCommandList.m_commands.clear();
		//玩家2视野准备
		for(i=0;i<soldiers.size()&&soldiers[i].owner==2;i++)
		{
			int x = soldiers[i].x_position;
			int y = soldiers[i].y_position;
			switch (soldiers[i].type)
			{			
			case LightInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
			case LightArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
			case LightKnight:
				{
					for( j=x-5;j<x+6;j++)
					{
						for(k=y-5;k<y+6;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
			case HeavyInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;			
							}
						}
					}
					break;
				}
			case HeavyArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
			case HeavyKnight:
				{
					for( j=x-4;j<x+5;j++)
					{
						for(k=y-4;k<y+5;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
			default:
				{
					for( j=x-2;j<x+3;j++)
					{
						for(k=y-2;k<y+3;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player2_visible = true;
							}
						}
					}
					break;
				}
				
			}
		}
		//塔的视野
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].owner != -1)
			{
				int x = towers[i].x_position;
				int y = towers[i].y_position;
				for( j=x-2-towers[i].level;j<x+3+towers[i].level;j++)
				{
					for(k=y-2-towers[i].level;k<y+3+towers[i].level;k++)
					{
						if(j>=0&&j<100&&k>=0&&k<100)
						{
							switch (towers[i].owner)
							{
							case 0:
								points[j][k].player0_visible = true;
								break;
							case 1:
								points[j][k].player1_visible = true;
								break;
							case 2:
								points[j][k].player2_visible = true;
								break;
							case 3:
								points[j][k].player3_visible = true;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
		//玩家2的代码前准备（地图、士兵、食堂获取）
		for(j=0;j<100;j++)//获得地图
		{
			for(k=0;k<100;k++)
			{
				if(points[j][k].player2_visible)
				{
					points2[j][k] = points[j][k];
				}
				else
				{
					points2[j][k].occupied = false;
					points2[j][k].occupied_type = UnKnown;
					points2[j][k].soldier = -1;
					points2[j][k].tower = -1;
					points2[j][k].visible = false;
				}
			}
		}
		for(i=0;i<towers.size();i++)//获得塔信息
		{
			if(towers[i].position.player2_visible)
			{
				towers2[i] = towers[i];
			}
			else
			{
				towers2[i].blood = -1;
				towers2[i].owner = -1;
				towers2[i].level = -1;
				towers2[i].boost = -1;
				towers2[i].max_blood = -1;
				towers2[i].population = -1;
				towers2[i].position = points2[towers2[i].x_position][towers2[i].y_position];
				towers2[i].recruiting = false;
				towers2[i].recruiting_round = -1;
				towers2[i].resource_income = -1;
			}
		}
		//获得士兵信息
		soldiers2.clear();
		for(i=0;i<soldiers.size();i++)
		{
			if(points[soldiers[i].x_position][soldiers[i].y_position].player2_visible)
			{
				soldiers2.push_back(soldiers[i]);
			}
		}
		//info信息更新
		info.pointInfo = points2;
		info.towerInfo = towers2;
		info.soldierInfo = soldiers2;
		info.myID = 2;
		//玩家2代码
		hDLL = LoadLibrary(_TEXT("..//Debug//mydll.dll" ));
		if(hDLL!=NULL)
	    {
			
			Player_ai = (Func)GetProcAddress(hDLL,"player_ai");
			if(Player_ai != NULL)
			{
				Player_ai(info);
			}
	    }
	    FreeLibrary(hDLL);
		//玩家2命令处理
		fout<<"Player2Commands"<<endl;
		for(i=0;i<info.myCommandList.m_commands.size();i++)
		{
			switch (info.myCommandList.m_commands[i].type)
			{
			case Attack:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int temp_x,temp_y;
						temp_x = info.myCommandList.m_commands[i].parameters[1];
						temp_y = info.myCommandList.m_commands[i].parameters[2];
						float distance2 = (soldiers[j].x_position-temp_x)*(soldiers[j].x_position-temp_x)+(soldiers[j].y_position-temp_y)*(soldiers[j].y_position-temp_y);
						float distance = sqrt(distance2);
						if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Soldier)
						//如果没打过且其占据目标位置类型为士兵
						{
							
							for(k=0;k<soldiers.size();k++)
							{
								if(soldiers[k].id==points[temp_x][temp_y].soldier)
								  break;
							}
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Soldier"<<" VictimID "<<soldiers[k].id<<endl;
							int real_attack = soldiers[i].attack;
							int real_armor = soldiers[k].armor;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_attack += 5;
							if(soldiers[j].type == Mangonel&&soldiers[j].position.land == Classroom)
								real_attack += 5;
							if((soldiers[k].type == LightArcher||soldiers[k].type == HeavyArcher)&&soldiers[k].position.land == Forest)
								real_armor += 5;
							if((soldiers[k].type == HeavyInfantry||soldiers[k].type == HeavyArcher||soldiers[k].type == HeavyKnight)&&soldiers[k].position.land == Dorm)
								real_armor += 5;
							soldiers[k].blood -= real_attack-real_armor;
							soldiers[j].attackable=false;
							if(soldiers[k].blood<=0)//目标士兵死亡
							{
								//获得经验(处理升级信息)
								int available_soldier_num=0;
										
								for(int t1=0;t1<soldiers.size();t1++)
								{
									if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
									{
										available_soldier_num++;
									}
								}
								if((125-50*available_soldier_num)>50)
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += (125-50*available_soldier_num);
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
										}
									}
								}
								else
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += 50;
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
									    }
									}
								}
										
								//更新地面信息，势力击杀数，人口数变动
								points[soldiers[k].x_position][soldiers[k].y_position].occupied = false;
								points[soldiers[k].x_position][soldiers[k].y_position].occupied_type = None;
								points[soldiers[k].x_position][soldiers[k].y_position].soldier = -1;
								info.playerInfo[soldiers[k].owner].soldier_num--;
								switch (soldiers[k].type)
								{
								    case LightInfantry:info.playerInfo[soldiers[k].owner].population -= 2;break;
									case LightArcher:info.playerInfo[soldiers[k].owner].population -= 3;break;
									case LightKnight:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case Mangonel:info.playerInfo[soldiers[k].owner].population -= 10;break;
									case HeavyInfantry:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case HeavyArcher:info.playerInfo[soldiers[k].owner].population -= 8;break;
									case HeavyKnight:info.playerInfo[soldiers[k].owner].population -= 10;break;
									default:
										break;
								}
										
								info.playerInfo[info.myID].kill_num ++;
								//删除死亡士兵
								soldiers.erase(soldiers.begin()+k);
							}
						}
						else if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Tower)
						//如果能打且目标位置为塔
						{
							int t_tower_id = points[temp_x][temp_y].tower;
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Tower"<<" VictimID "<<t_tower_id<<endl;
							int real_damage = soldiers[j].attack;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_damage += 5;
							else if(soldiers[j].type == Mangonel)
							{
								if(soldiers[j].position.land == Classroom)
									real_damage = 5*(real_damage+5);
								else
									real_damage = 5*real_damage;
							}

							towers[t_tower_id].blood -= real_damage;
								//判断塔的易主问题
							if(towers[t_tower_id].blood<=0)
							{
								if(towers[t_tower_id].owner==-1)
								{
									towers[t_tower_id].blood = towers[t_tower_id].max_blood;
									towers[t_tower_id].owner = info.myID;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
								else
								{
										
									towers[t_tower_id].level = 1;
									towers[t_tower_id].boost = 0;
									towers[t_tower_id].max_blood = 200;
									towers[t_tower_id].blood = 100;
									towers[t_tower_id].resource_income = 4;	
									info.playerInfo[towers[t_tower_id].owner].max_population -= towers[t_tower_id].population;
									info.playerInfo[towers[t_tower_id].owner].tower_num --;
									towers[t_tower_id].population = 10;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									towers[t_tower_id].owner = info.myID;	
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
							}
							
                        }
					}
					break;
				}
			case Move:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int tx,ty;
						int water_judge = 0;//0表示路径上无水，1表示有水
						tx = soldiers[j].x_position;
						ty = soldiers[j].y_position;
						MoveDirection direction = (MoveDirection)info.myCommandList.m_commands[i].parameters[1];
						TMove  distance = info.myCommandList.m_commands[i].parameters[2];
						if(distance<0)
							distance = 0;
						if(distance<soldiers[j].move_left&&distance>0)
							//能够移动
						{
							switch (direction)
							{
							case UP:
								if((ty+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty+k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty+distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty+distance].occupied = true;
									    points[tx][ty+distance].occupied_type = Soldier;
									    points[tx][ty+distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty+distance;
									    soldiers[j].position = points[tx][ty+distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"UP"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case DOWN:
								if((ty-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty-k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty-distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty-distance].occupied = true;
									    points[tx][ty-distance].occupied_type = Soldier;
									    points[tx][ty-distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty-distance;
									    soldiers[j].position = points[tx][ty-distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"DOWN"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case LEFT:
								if((tx-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx-k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx-distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx-distance][ty].occupied = true;
									    points[tx-distance][ty].occupied_type = Soldier;
									    points[tx-distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx-distance;
									    soldiers[j].position = points[tx-distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"LEFT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case RIGHT:
								if((tx+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx+k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx+distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx+distance][ty].occupied = true;
									    points[tx+distance][ty].occupied_type = Soldier;
									    points[tx+distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx+distance;
									    soldiers[j].position = points[tx+distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"RIGHT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							default:
								break;
							}
						}
					    
					}
					break;
				}
			case Upgrade:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						switch (towers[j].level)
						{
						case 1:
							if(info.playerInfo[info.myID].resource>=80)
							{
								towers[j].max_blood = 250;
								towers[j].blood += 50;
								towers[j].boost = 1;
								towers[j].resource_income = 6;
								towers[j].population = 15;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 80;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						case 2:
							if(info.playerInfo[info.myID].resource>=150)
							{
								towers[j].max_blood = 300;
								towers[j].blood += 50;
								towers[j].boost = 2;
								towers[j].resource_income = 8;
								towers[j].population = 20;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 150;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						default:
							break;
						}
					}
					break;
				}
			case Produce:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						TSoldierType type;
						type = (TSoldierType)info.myCommandList.m_commands[i].parameters[1];
						if(towers[j].recruiting == false)
						{
							switch (type)
							{
							case LightInfantry:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=2)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 4-towers[j].boost;
									towers[j].soldier = LightInfantry;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightArcher:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=3)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 5-towers[j].boost;
									towers[j].soldier = LightArcher;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightKnight:
								if(info.playerInfo[info.myID].resource >= 50&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 6-towers[j].boost;
									towers[j].soldier = LightKnight;
									info.playerInfo[info.myID].resource -= 50;
								}
								break;
							case Mangonel:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 8-towers[j].boost;
									towers[j].soldier = Mangonel;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							case HeavyInfantry:
								if(info.playerInfo[info.myID].resource >= 55&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 7-towers[j].boost;
									towers[j].soldier = HeavyInfantry;
									info.playerInfo[info.myID].resource -= 55;
								}
								break;
							case HeavyArcher:
								if(info.playerInfo[info.myID].resource >= 60&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=8)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyArcher;
									info.playerInfo[info.myID].resource -= 60;
								}
								break;
							case HeavyKnight:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyKnight;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							default:
								break;
							}
						}
					}
					break;
				}
			default:
				break;
			}
		}
		info.myCommandList.m_commands.clear();
		//玩家3视野准备
		for(i=0;i<soldiers.size()&&soldiers[i].owner==3;i++)
		{
			int x = soldiers[i].x_position;
			int y = soldiers[i].y_position;
			switch (soldiers[i].type)
			{			
			case LightInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
			case LightArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
			case LightKnight:
				{
					for( j=x-5;j<x+6;j++)
					{
						for(k=y-5;k<y+6;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
			case HeavyInfantry:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;			
							}
						}
					}
					break;
				}
			case HeavyArcher:
				{
					for( j=x-3;j<x+4;j++)
					{
						for(k=y-3;k<y+4;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
			case HeavyKnight:
				{
					for( j=x-4;j<x+5;j++)
					{
						for(k=y-4;k<y+5;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
			default:
				{
					for( j=x-2;j<x+3;j++)
					{
						for(k=y-2;k<y+3;k++)
						{
							if(j>=0&&j<100&&k>=0&&k<100)
							{
									points[j][k].player3_visible = true;
							}
						}
					}
					break;
				}
				
			}
		}
		//塔的视野
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].owner != -1)
			{
				int x = towers[i].x_position;
				int y = towers[i].y_position;
				for( j=x-2-towers[i].level;j<x+3+towers[i].level;j++)
				{
					for(k=y-2-towers[i].level;k<y+3+towers[i].level;k++)
					{
						if(j>=0&&j<100&&k>=0&&k<100)
						{
							switch (towers[i].owner)
							{
							case 0:
								points[j][k].player0_visible = true;
								break;
							case 1:
								points[j][k].player1_visible = true;
								break;
							case 2:
								points[j][k].player2_visible = true;
								break;
							case 3:
								points[j][k].player3_visible = true;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
		//玩家3的代码前准备（地图、士兵、食堂获取）
		for(j=0;j<100;j++)//获得地图
		{
			for(k=0;k<100;k++)
			{
				if(points[j][k].player3_visible)
				{
					points3[j][k] = points[j][k];
				}
				else
				{
					points3[j][k].occupied = false;
					points3[j][k].occupied_type = UnKnown;
					points3[j][k].soldier = -1;
					points3[j][k].tower = -1;
					points3[j][k].visible = false;
				}
			}
		}
		for(i=0;i<towers.size();i++)//获得塔信息
		{
			if(towers[i].position.player3_visible)
			{
				towers3[i] = towers[i];
			}
			else
			{
				towers3[i].blood = -1;
				towers3[i].owner = -1;
				towers3[i].level = -1;
				towers3[i].boost = -1;
				towers3[i].max_blood = -1;
				towers3[i].population = -1;
				towers3[i].position = points3[towers3[i].x_position][towers3[i].y_position];
				towers3[i].recruiting = false;
				towers3[i].recruiting_round = -1;
				towers3[i].resource_income = -1;
			}
		}
		//获得士兵信息
		soldiers3.clear();
		for(i=0;i<soldiers.size();i++)
		{
			if(points[soldiers[i].x_position][soldiers[i].y_position].player3_visible)
			{
				soldiers3.push_back(soldiers[i]);
			}
		}
		//info信息更新
		info.pointInfo = points3;
		info.towerInfo = towers3;
		info.soldierInfo = soldiers3;
		info.myID = 3;
		//玩家3代码
		hDLL = LoadLibrary(_TEXT("..//Debug//mydll.dll" ));
		if(hDLL!=NULL)
	    {
			
			Player_ai = (Func)GetProcAddress(hDLL,"player_ai");
			if(Player_ai != NULL)
			{
				Player_ai(info);
			}
	    }
	    FreeLibrary(hDLL);
		//玩家3命令处理
		fout<<"Player3CommandsInfo"<<endl;
		for(i=0;i<info.myCommandList.m_commands.size();i++)
		{
			switch (info.myCommandList.m_commands[i].type)
			{
			case Attack:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int temp_x,temp_y;
						temp_x = info.myCommandList.m_commands[i].parameters[1];
						temp_y = info.myCommandList.m_commands[i].parameters[2];
						float distance2 = (soldiers[j].x_position-temp_x)*(soldiers[j].x_position-temp_x)+(soldiers[j].y_position-temp_y)*(soldiers[j].y_position-temp_y);
						float distance = sqrt(distance2);
						if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Soldier)
						//如果没打过且其占据目标位置类型为士兵
						{
							
							for(k=0;k<soldiers.size();k++)
							{
								if(soldiers[k].id==points[temp_x][temp_y].soldier)
								  break;
							}
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Soldier"<<" VictimID "<<soldiers[k].id<<endl;
							int real_attack = soldiers[i].attack;
							int real_armor = soldiers[k].armor;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_attack += 5;
							if(soldiers[j].type == Mangonel&&soldiers[j].position.land == Classroom)
								real_attack += 5;
							if((soldiers[k].type == LightArcher||soldiers[k].type == HeavyArcher)&&soldiers[k].position.land == Forest)
								real_armor += 5;
							if((soldiers[k].type == HeavyInfantry||soldiers[k].type == HeavyArcher||soldiers[k].type == HeavyKnight)&&soldiers[k].position.land == Dorm)
								real_armor += 5;
							soldiers[k].blood -= real_attack-real_armor;
							soldiers[j].attackable=false;
							if(soldiers[k].blood<=0)//目标士兵死亡
							{
								//获得经验(处理升级信息)
								int available_soldier_num=0;
										
								for(int t1=0;t1<soldiers.size();t1++)
								{
									if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
									{
										available_soldier_num++;
									}
								}
								if((125-50*available_soldier_num)>50)
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += (125-50*available_soldier_num);
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
										}
									}
								}
								else
								{
									for(int t1=0;t1<soldiers.size();t1++)
									{
										if(soldiers[t1].owner==soldiers[j].owner&&(soldiers[k].x_position-soldiers[t1].x_position)*(soldiers[k].x_position-soldiers[t1].x_position)+(soldiers[k].y_position-soldiers[t1].y_position)*(soldiers[k].y_position-soldiers[t1].y_position)<=25)
										{
											soldiers[t1].experience += 50;
											if(soldiers[t1].experience>=150&&soldiers[t1].level<3)//升级处理
											{
												soldiers[t1].level++;
												soldiers[t1].experience -= 150;
												soldiers[t1].armor += 5;
												soldiers[t1].attack += 5;
											}
									    }
									}
								}
										
								//更新地面信息，势力击杀数，人口数变动
								points[soldiers[k].x_position][soldiers[k].y_position].occupied = false;
								points[soldiers[k].x_position][soldiers[k].y_position].occupied_type = None;
								points[soldiers[k].x_position][soldiers[k].y_position].soldier = -1;
								info.playerInfo[soldiers[k].owner].soldier_num--;
								switch (soldiers[k].type)
								{
								    case LightInfantry:info.playerInfo[soldiers[k].owner].population -= 2;break;
									case LightArcher:info.playerInfo[soldiers[k].owner].population -= 3;break;
									case LightKnight:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case Mangonel:info.playerInfo[soldiers[k].owner].population -= 10;break;
									case HeavyInfantry:info.playerInfo[soldiers[k].owner].population -= 5;break;
									case HeavyArcher:info.playerInfo[soldiers[k].owner].population -= 8;break;
									case HeavyKnight:info.playerInfo[soldiers[k].owner].population -= 10;break;
									default:
										break;
								}
										
								info.playerInfo[info.myID].kill_num ++;
								//删除死亡士兵
								soldiers.erase(soldiers.begin()+k);
							}
						}
						else if(distance <= soldiers[j].range&&soldiers[j].attackable&&points[temp_x][temp_y].occupied_type == Tower)
						//如果能打且目标位置为塔
						{
							int t_tower_id = points[temp_x][temp_y].tower;
							fout<<"Attack "<<"AttackerID "<<soldiers[j].id<<" VicType "<<"Tower"<<" VictimID "<<t_tower_id<<endl;
							int real_damage = soldiers[j].attack;
							if((soldiers[j].type == LightKnight||soldiers[j].type == HeavyKnight)&&soldiers[j].position.land == Forest)
								real_damage += 5;
							else if(soldiers[j].type == Mangonel)
							{
								if(soldiers[j].position.land == Classroom)
									real_damage = 5*(real_damage+5);
								else
									real_damage = 5*real_damage;
							}

							towers[t_tower_id].blood -= real_damage;
								//判断塔的易主问题
							if(towers[t_tower_id].blood<=0)
							{
								if(towers[t_tower_id].owner==-1)
								{
									towers[t_tower_id].blood = towers[t_tower_id].max_blood;
									towers[t_tower_id].owner = info.myID;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
								else
								{
										
									towers[t_tower_id].level = 1;
									towers[t_tower_id].boost = 0;
									towers[t_tower_id].max_blood = 200;
									towers[t_tower_id].blood = 100;
									towers[t_tower_id].resource_income = 4;	
									info.playerInfo[towers[t_tower_id].owner].max_population -= towers[t_tower_id].population;
									info.playerInfo[towers[t_tower_id].owner].tower_num --;
									towers[t_tower_id].population = 10;
									towers[t_tower_id].recruiting = false;
									towers[t_tower_id].recruiting_round = -1;
									towers[t_tower_id].owner = info.myID;	
									info.playerInfo[info.myID].tower_num++;
									info.playerInfo[info.myID].max_population += 5*(towers[t_tower_id].level+1);
								}
							}
							
                        }
					}
					break;
				}
			case Move:
				{
					for(j=0;j<soldiers.size();j++)
					{
						if(soldiers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(soldiers[j].owner == info.myID)
					{
						int tx,ty;
						int water_judge = 0;//0表示路径上无水，1表示有水
						tx = soldiers[j].x_position;
						ty = soldiers[j].y_position;
						MoveDirection direction = (MoveDirection)info.myCommandList.m_commands[i].parameters[1];
						TMove  distance = info.myCommandList.m_commands[i].parameters[2];
						if(distance<0)
							distance = 0;
						if(distance<soldiers[j].move_left&&distance>0)
							//能够移动
						{
							switch (direction)
							{
							case UP:
								if((ty+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty+k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty+distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty+distance].occupied = true;
									    points[tx][ty+distance].occupied_type = Soldier;
									    points[tx][ty+distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty+distance;
									    soldiers[j].position = points[tx][ty+distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"UP"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case DOWN:
								if((ty-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx][ty-k].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx][ty-distance].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx][ty-distance].occupied = true;
									    points[tx][ty-distance].occupied_type = Soldier;
									    points[tx][ty-distance].soldier = soldiers[j].id;
									    soldiers[j].y_position = ty-distance;
									    soldiers[j].position = points[tx][ty-distance];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"DOWN"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case LEFT:
								if((tx-distance)>=0)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx-k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx-distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx-distance][ty].occupied = true;
									    points[tx-distance][ty].occupied_type = Soldier;
									    points[tx-distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx-distance;
									    soldiers[j].position = points[tx-distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"LEFT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							case RIGHT:
								if((tx+distance)<100)
								{
									for(k=1;k<=distance;k++)
									{
										if(points[tx+k][ty].land==River)
										{
											water_judge = 1;
											break;
										}
									}
									if(points[tx+distance][ty].occupied == false&&water_judge == 0)
								    {
									    points[tx][ty].occupied = false;
									    points[tx][ty].occupied_type = None;
									    points[tx][ty].soldier = -1;
									    points[tx+distance][ty].occupied = true;
									    points[tx+distance][ty].occupied_type = Soldier;
									    points[tx+distance][ty].soldier = soldiers[j].id;
									    soldiers[j].x_position = tx+distance;
									    soldiers[j].position = points[tx+distance][ty];
									    soldiers[j].move_left -= distance;
										fout<<"Move "<<"SoldierID "<<soldiers[j].id<<" Direction "<<"RIGHT"<<" Distance "<<distance<<endl;
								    }
								}
								break;
							default:
								break;
							}
						}
					    
					}
					break;
				}
			case Upgrade:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						switch (towers[j].level)
						{
						case 1:
							if(info.playerInfo[info.myID].resource>=80)
							{
								towers[j].max_blood = 250;
								towers[j].blood += 50;
								towers[j].boost = 1;
								towers[j].resource_income = 6;
								towers[j].population = 15;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 80;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						case 2:
							if(info.playerInfo[info.myID].resource>=150)
							{
								towers[j].max_blood = 300;
								towers[j].blood += 50;
								towers[j].boost = 2;
								towers[j].resource_income = 8;
								towers[j].population = 20;
								towers[j].level ++;
								info.playerInfo[info.myID].resource -= 150;
								info.playerInfo[info.myID].max_population += 5;
							}
							break;
						default:
							break;
						}
					}
					break;
				}
			case Produce:
				{
					for(j=0;j<towers.size();j++)
					{
						if(towers[j].id == info.myCommandList.m_commands[i].parameters[0])	
						{
							break;
						}
					}
					if(towers[j].owner==info.myID)
					{
						TSoldierType type;
						type = (TSoldierType)info.myCommandList.m_commands[i].parameters[1];
						if(towers[j].recruiting == false)
						{
							switch (type)
							{
							case LightInfantry:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=2)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 4-towers[j].boost;
									towers[j].soldier = LightInfantry;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightArcher:
								if(info.playerInfo[info.myID].resource >= 40&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=3)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 5-towers[j].boost;
									towers[j].soldier = LightArcher;
									info.playerInfo[info.myID].resource -= 40;
								}
								break;
							case LightKnight:
								if(info.playerInfo[info.myID].resource >= 50&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 6-towers[j].boost;
									towers[j].soldier = LightKnight;
									info.playerInfo[info.myID].resource -= 50;
								}
								break;
							case Mangonel:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 8-towers[j].boost;
									towers[j].soldier = Mangonel;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							case HeavyInfantry:
								if(info.playerInfo[info.myID].resource >= 55&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=5)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 7-towers[j].boost;
									towers[j].soldier = HeavyInfantry;
									info.playerInfo[info.myID].resource -= 55;
								}
								break;
							case HeavyArcher:
								if(info.playerInfo[info.myID].resource >= 60&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=8)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyArcher;
									info.playerInfo[info.myID].resource -= 60;
								}
								break;
							case HeavyKnight:
								if(info.playerInfo[info.myID].resource >= 70&&(info.playerInfo[info.myID].max_population-info.playerInfo[info.myID].population)>=10)
								{
									towers[j].recruiting = true;
									towers[j].recruiting_round = 9-towers[j].boost;
									towers[j].soldier = HeavyKnight;
									info.playerInfo[info.myID].resource -= 70;
								}
								break;
							default:
								break;
							}
						}
					}
					break;
				}
			default:
				break;
			}
		}
		info.myCommandList.m_commands.clear();
		//生产士兵
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].recruiting)
			{
				if(towers[i].recruiting_round == 0)
				{
					TSoldier new_soldier;
					int pop_take = 0;
					switch (towers[i].soldier)
					{
					case LightInfantry:
						{
							pop_take = 2;
							new_soldier.attack = 25;
							new_soldier.armor = 15;
							new_soldier.blood = 100;
							new_soldier.move_ability = 3;
							new_soldier.move_left = 3;
							new_soldier.range = 1;
							new_soldier.type = LightInfantry;							
						    break;
						}
					case LightArcher:
						{
							pop_take = 3;
							new_soldier.attack = 35;
							new_soldier.armor = 5;
							new_soldier.blood = 80;
							new_soldier.move_ability = 3;
							new_soldier.move_left = 3;
							new_soldier.range = 2;
							new_soldier.type = LightArcher;							
						    break;
						}
					case LightKnight:
						{
							pop_take = 5;
							new_soldier.attack = 40;
							new_soldier.armor = 10;
							new_soldier.blood = 100;
							new_soldier.move_ability = 5;
							new_soldier.move_left = 5;
							new_soldier.range = 1;
							new_soldier.type = LightKnight;							
						    break;
						}
					case Mangonel:
						{
							pop_take = 10;
							new_soldier.attack = 10;
							new_soldier.armor = 20;
							new_soldier.blood = 200;
							new_soldier.move_ability = 2;
							new_soldier.move_left = 2;
							new_soldier.range = 3;
							new_soldier.type = Mangonel;							
						    break;
						}
					case HeavyInfantry:
						{
							pop_take = 5;
							new_soldier.attack = 30;
							new_soldier.armor = 25;
							new_soldier.blood = 150;
							new_soldier.move_ability = 3;
							new_soldier.move_left = 3;
							new_soldier.range = 1;
							new_soldier.type = HeavyInfantry;							
						    break;
						}
					case HeavyArcher:
						{
							pop_take = 8;
							new_soldier.attack = 45;
							new_soldier.armor = 15;
							new_soldier.blood = 100;
							new_soldier.move_ability = 3;
							new_soldier.move_left = 3;
							new_soldier.range = 3;
							new_soldier.type = HeavyArcher;							
						    break;
						}
					case HeavyKnight:
						{
							pop_take = 10;
							new_soldier.attack = 50;
							new_soldier.armor = 20;
							new_soldier.blood = 120;
							new_soldier.move_ability = 4;
							new_soldier.move_left = 4;
							new_soldier.range = 1;
							new_soldier.type = HeavyKnight;							
						    break;
						}
					default:
						break;
					}
					new_soldier.owner = towers[i].owner;
					new_soldier.experience = 0;
					new_soldier.level = 1;			
					new_soldier.attackable = false;
					int position_temp = 2;
					while(position_temp<4)
					{
						if(points[towers[i].x_position+position_temp][towers[i].y_position].occupied == false)
						{
							new_soldier.position = points[towers[i].x_position+position_temp][towers[i].y_position];
							new_soldier.x_position = towers[i].x_position+position_temp;
							new_soldier.y_position = towers[i].y_position;
							new_soldier.id = ID_timer++;
							soldiers.push_back(new_soldier);
							info.playerInfo[towers[i].owner].soldier_num++;
							info.playerInfo[towers[i].owner].population += pop_take;
							points[towers[i].x_position+position_temp][towers[i].y_position].occupied = true;
							points[towers[i].x_position+position_temp][towers[i].y_position].occupied_type = Soldier;
							points[towers[i].x_position+position_temp][towers[i].y_position].soldier = new_soldier.id;
							break;
						}
						else if(points[towers[i].x_position-position_temp][towers[i].y_position].occupied == false)
						{
							new_soldier.position = points[towers[i].x_position-position_temp][towers[i].y_position];
							new_soldier.x_position = towers[i].x_position-position_temp;
							new_soldier.y_position = towers[i].y_position;
							new_soldier.id = ID_timer++;
							soldiers.push_back(new_soldier);
							info.playerInfo[towers[i].owner].soldier_num++;
							info.playerInfo[towers[i].owner].population += pop_take;
							points[towers[i].x_position-position_temp][towers[i].y_position].occupied = true;
							points[towers[i].x_position-position_temp][towers[i].y_position].occupied_type = Soldier;
							points[towers[i].x_position-position_temp][towers[i].y_position].soldier = new_soldier.id;
							break;
						}
						else if(points[towers[i].x_position][towers[i].y_position+position_temp].occupied == false)
						{
							new_soldier.position = points[towers[i].x_position][towers[i].y_position+position_temp];
							new_soldier.x_position = towers[i].x_position;
							new_soldier.y_position = towers[i].y_position+position_temp;
							new_soldier.id = ID_timer++;
							soldiers.push_back(new_soldier);
							info.playerInfo[towers[i].owner].soldier_num++;
							info.playerInfo[towers[i].owner].population += pop_take;
							points[towers[i].x_position][towers[i].y_position+position_temp].occupied = true;
							points[towers[i].x_position][towers[i].y_position+position_temp].occupied_type = Soldier;
							points[towers[i].x_position][towers[i].y_position+position_temp].soldier = new_soldier.id;
							break;
						}
						else if(points[towers[i].x_position][towers[i].y_position-position_temp].occupied == false)
						{
							new_soldier.position = points[towers[i].x_position][towers[i].y_position-position_temp];
							new_soldier.x_position = towers[i].x_position;
							new_soldier.y_position = towers[i].y_position-position_temp;
							new_soldier.id = ID_timer++;
							soldiers.push_back(new_soldier);
							info.playerInfo[towers[i].owner].soldier_num++;
							info.playerInfo[towers[i].owner].population += pop_take;
							points[towers[i].x_position][towers[i].y_position-position_temp].occupied = true;
							points[towers[i].x_position][towers[i].y_position-position_temp].occupied_type = Soldier;
							points[towers[i].x_position][towers[i].y_position-position_temp].soldier = new_soldier.id;
							break;
						}
						else
						{
							info.playerInfo[towers[i].owner].resource += 40;
						}
						position_temp++;
					}
					towers[i].recruiting = false;
					towers[i].recruiting_round = -1;
				}
				else
				towers[i].recruiting_round--;
			}
		}
		//资源回复、塔的血量回复、更新rank,移动力，攻击回复
		for(i=0;i<towers.size();i++)
		{
			if(towers[i].blood<towers[i].max_blood)
			{
				if(towers[i].blood+5>=towers[i].max_blood)
					towers[i].blood = towers[i].max_blood;
				else
					towers[i].blood += 5;
			}
			if(towers[i].owner!=-1)
			{
				info.playerInfo[towers[i].owner].resource += towers[i].resource_income;
			}
		}
		for(i=0;i<soldiers.size();i++)
		{
			soldiers[i].move_left = soldiers[i].move_ability;
			soldiers[i].attackable = true;
		}
		info.round++;//回合数增加
		for(i=0;i<4;i++)
		{
			if(info.playerInfo[i].live)
				info.playerInfo[i].survival_round++;
			info.playerInfo[i].rank = -1;
			info.playerInfo[i].score = info.playerInfo[i].kill_num*25+info.playerInfo[i].survival_round+50*info.playerInfo[i].tower_num;
		}
		double min;
		j=4;
		while(j>0)
		{
			min = 10000;
		    for(i=0;i<4;i++)
		    {
				if(info.playerInfo[i].score<=min&&info.playerInfo[i].rank==-1)
					min = info.playerInfo[i].score;
		    }
			for(i=0;i<4;i++)
			{
				if(info.playerInfo[i].score == min&&info.playerInfo[i].rank == -1)
				{
					info.playerInfo[i].rank = j;
					break;
				}
			}
			j--;
		}
		//检测输出
		
	}
	fout.close();
}