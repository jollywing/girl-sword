#ifndef __GIRLH__
#define __GIRLH__

#include "Role.h"
#include "Map.h"
#include "Record.h"
#include "Dialog.h"
#include "Fighter.h"
#include "Menu.h"







//全局游戏变量
int current_npc_id;	//当前的Npc的id
Map * current_map;	//当前的地图指针
Fighter * current_enemy;	//当前的敌人指针
short TrapNum; //玩家踩中的陷阱编号
long old_time, new_time; //用于计算帧时间的变量
short frame_fight;	//战斗时的帧计数
int fight_frame_num;	//战斗的总帧数
short round_num;	//回合数
short Flag;//游戏运行状态标志


//**************************函数声明**********************************

//和游戏状态对应的函数
void MainLoop();	//0 游戏主循环
void InitGame();	//1 游戏初始化
void MainMove();	//2 主角在地图走动的函数
void StartFight();	//3 开始战斗
void Fighting();	//4	战斗中
void FightEnd();	//5 战斗结束
void TreatNpc();	//6 处理Npc事件
void GameTitle();	//7
void GameExit();	//8 游戏退出
void System_Menu();	//9 游戏过程中调出系统菜单
void GameMessage();	//10 游戏过程中显示消息
void AutoPlay();	//11 自动剧情
void Load();	//12 装载档案
void Store();	//13 存储档案	
void BeforeSelect();	//14 弹出玩家选择的菜单
void WaitSelect();	//15 等待玩家选择
void SelectYes();	//16 玩家做出是的选择
void SelectNo();	//17 玩家做出否的选择
void CheckState();	//18 查看玩家状态
void CheckAbout();	//19 查看作品信息

//更新游戏画面函数
void DrawState();		//1 画主角的状态
void DrawStateDetail();	//2 显示详细状态信息
void DrawStartMenu();	//3 画开始菜单
void DrawSystemMenu();	//4 画系统菜单
void DrawSelectMenu();	//5 画选择菜单
void DrawRecord();		//6 画纪录
void RefreshCanvas();	//7 更新非战斗画面
void UpdateFight();		//8 更新战斗画面
void DrawRoundNum();	//9 画战斗回合数
void ClrScr();			//10 清屏
void DrawPic(const char *);	//11 在屏幕上显示图片
void ShowAbout();		//12 显示作品的信息	
void FlipPage();		//13 将离屏页面的内容传送到主页面
void DrawTitle();		//14 在离屏页面上画片头
void DrawEnd();			//15 画片尾

//游戏辅助函数
void BorderCheck(int *x,int *y);	//1 边界检测
short CrushCheck();					//2 碰撞检测
void SceneChange();					//3 场景切换函数
void QueryMessage(short);			//4 响应玩家在特定地点按下空格事件
short FindNpc();					//5 寻找玩家面对的Npc,返回Npc的ID
void GetMapbyID(int);				//6 通过地图ID，改变current_map
Role * GetNpcbyID(int);				//7 得到npc的指针
void RelayoutNpc();					//8 切换地图时，重新加载NPC
void GotoMap(Map *);				//9 主角走到新的地图
//void PressKey(DWORD key);			//10等待按键松开
void WaitKeyRelease();

//操作游戏数据函数
void InitData();					//1 初始化数据
short LoadData(char *);				//2 读取数据
short StoreData(char *);				//3 存储数据



#endif
