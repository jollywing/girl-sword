#ifndef __GIRLH__
#define __GIRLH__

#include <stack>
#include "SdlSys.h"
#include "Role.h"
#include "Map.h"
#include "Record.h"
#include "Dialog.h"
#include "Fighter.h"
#include "Menu.h"

using namespace std;

//游戏运行状态的宏定义
#define GAME_LOAD_	0	//游戏数据装载
#define GAME_TITLE_	1	//游戏开始画面
#define MAIN_MOVE_	2	//由玩家控制主角
#define READ_RECORD_	3	//读取纪录
#define GAME_EXIT_	4	//正在退出游戏
#define SYSTEM_MENU_	5	//游戏过程中调出系统菜单
#define WRITE_RECORD_	6	//存写纪录
#define GAME_MESSAGE_	7	//系统消息
#define FIGHT_START_	8	//战斗开始
#define FIGHTING_	9	//战斗进行
#define FIGHT_END_	10	//战斗结束
#define AUTO_PLAY_	11	//自动剧情
#define TREAT_NPC_	12	//处理NPC事件
#define BEFORE_SELECT_	13	//弹出等待玩家选择答案的页面
#define WAIT_SELECT_	14	//等待玩家做出选择
#define	SELECT_YES_		15	//玩家做出是的选择
#define SELECT_NO_		16	//玩家做出否的选择
#define CHECK_STATE_	17	//查看状态
#define CHECK_ABOUT_	18	//查看作品信息
#define RUN_SCRIPT_     19  //运行脚本
#define NPC_MOVE_		20	//移动NPC

// extern short Flag, oldFlag;
extern stack<int> stateStack;

//**************************函数声明**********************************

//和游戏状态对应的函数
void MainLoop();	//0 游戏主循环
void InitGame();	//1 游戏初始化
void MainMove();	//2 主角在地图走动的函数
void InitFight();	//3 初始化战斗
void Fighting();	//4	战斗中
void fight_win();
void fight_fail();

// void FightEnd();	//5 战斗结束
// void TreatNpc();	//6 处理Npc事件
void GameTitle();	//7
void GameExit();	//8 游戏退出
void System_Menu();	//9 游戏过程中调出系统菜单
void GameMessage();	//10 游戏过程中显示消息
void AutoPlay();	//11 自动剧情
void Load();	//12 装载档案
void Store();	//13 存储档案	
// void BeforeSelect();	//14 弹出玩家选择的菜单
void WaitSelect();	//15 等待玩家选择
// void SelectYes();	//16 玩家做出是的选择
// void SelectNo();	//17 玩家做出否的选择
// void CheckState();	//18 查看玩家状态
// void CheckAbout();	//19 查看作品信息

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
// void FlipPage();		//13 将离屏页面的内容传送到主页面
void DrawTitle();		//14 在离屏页面上画片头
void DrawEnd();			//15 画片尾

//游戏辅助函数
void BorderCheck(int *x,int *y);	//1 边界检测
short CrushCheck();					//2 碰撞检测
void SceneChange();					//3 场景切换函数
void QueryMessage(short);			//4 响应玩家在特定地点按下空格事件
Role * FindNpc();					//5 寻找玩家面对的Npc,返回Npc的ID
void GetMapbyID(int);				//6 通过地图ID，改变current_map
Role * GetNpcbyID(int);				//7 得到npc的指针
void RelayoutNpc();					//8 切换地图时，重新加载NPC
void GotoMap(Map *);				//9 主角走到新的地图
//void PressKey(DWORD key);			//10等待按键松开
void MoveNpc();

void WaitKeyRelease();
void wait_any_key_press();


//操作游戏数据函数
void InitData();					//1 初始化数据
short LoadData(char *);				//2 读取数据
short StoreData(char *);				//3 存储数据



#endif
