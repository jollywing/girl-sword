//girl.cpp

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <stack>

#include "Girl.h"
#include "SdlSys.h"
#include "Script.h"
#include "Variable.h"

using namespace std;

// #define CK RGB(0,0,0)	//定义关键色
#define speed 20	//定义角色移动速度
#define FRAME_TIME 100	//定义一帧的毫秒数

//全局游戏变量
// int current_npc_id;	//当前的Npc的id
Map * current_map;	//当前的地图指针
short TrapNum; //玩家踩中的陷阱编号
short frame_fight;	//战斗时的帧计数
int fight_frame_num;	//战斗的总帧数
short round_num;	//回合数


short game_running;
stack<int> stateStack;

MenuFlag g_menuFlag, g_oldMenuFlag;
RwFlag g_rwFlag;


//****************************应用程序函数******************************* 

//程序入口函数
int main(int argc, char ** argv)
{
    int windowed = 1;
    if(argc >1){
        if( strcmp(argv[1], "-h") == 0){
            printf("This is a chinese RPG for Linux. Version: 1.7\n");
            printf("The program runs in window by default;\n");
            printf("You can run it in full screen by adding an option: -x\n");
            printf("Have Fun!!!\n");
            return 0;
        }
        if( strcmp(argv[1], "-x") == 0)
            windowed = 0;
    }

    SDL_Event event;
	//SCR_X = 100;	//定义绘图窗口在屏幕中位置
	//SCR_Y = 100;

    InitSDL(windowed);
    OpenFonts();
    InitAudio();

	//bActive = TRUE;
	// Flag = GAME_LOAD_;	//设置游戏进度 
	InitGame();	//游戏初始化

    while(game_running){

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                game_running = 0;
        }

		MainLoop();
        SDL_Flip(screen);
        SDL_Delay(FRAME_TIME);
    }


    ShutdownAudio();
    CloseFonts();
    FreeSDL();

}
	
/*
	case WM_MOVE:	//窗口移动时更新SCR_X和SCR_Y的值
		RECT r;
		GetWindowRect(hWnd,&r);
		SCR_X += r.left - OldWindow.left;
		SCR_Y += r.top - OldWindow.top;
		OldWindow = r;
		break;
*/

//***********************和游戏状态绑定的函数******************************

//游戏主循环
void MainLoop()
{
    if (stateStack.empty())
        stateStack.push(MAIN_MOVE_);

	switch(stateStack.top())
	{
	case MAIN_MOVE_:	//2
		MainMove();
		break;
	case RUN_SCRIPT_:
		RunScripts();
		break;
	case GAME_MESSAGE_:	//6
		GameMessage();
		break;
    case GAME_MENU_:
        game_menu();
        break;
	case FIGHTING_:		//8
		Fighting();
		break;
	case AUTO_PLAY_:	//10
		AutoPlay();
		break;
	case NPC_MOVE_:
		MoveNpc();
		break;
	default:
		GameExit();
	}
	
}


// npc移动
void MoveNpc()
{
	if (!current_npc->move_to(npc_dest_x, npc_dest_y))
        stateStack.pop();
    // Flag = oldFlag;
	// else
    RefreshCanvas();
}

//游戏初始化
void InitGame()
{

    game_running = 1;
	InitData();	//初始化游戏数据

    stateStack.push(MAIN_MOVE_);
    // stateStack.push(GAME_TITLE_);
    g_menuFlag = TITLE_MENU;
    stateStack.push(GAME_MENU_);
	// Flag = GAME_TITLE_;		//推进游戏进度
}

void new_game()
{
    stateStack.pop(); // leave state of game title
	//显示初始化界面
    SDL_FillRect(screen, NULL, 0);
    SDL_BlitText("游戏正在初始化。。。", screen, 125, SCR_H/2,
            message_font, message_color);
    SDL_Flip(screen);
    ClearVariable(); // clear all flags of plot
    RunScripts("初始化");
}

void start_load_game()
{
    stateStack.pop();
    g_oldMenuFlag = g_menuFlag;
    g_menuFlag = RECORD_MENU;
    g_rwFlag = READ_RECORD;
    stateStack.push(GAME_MENU_);
}

void start_save_game()
{
    stateStack.pop();
    g_oldMenuFlag = g_menuFlag;
    g_menuFlag = RECORD_MENU;
    g_rwFlag = WRITE_RECORD;
    stateStack.push(GAME_MENU_);
}

void read_or_write_record()
{
    short selected;
    char filePath[256];

    selected = get_selected_menu(g_recordMenus, RECORD_MENU_NUM);
    sprintf(filePath, "save/%d.sav", selected + 1);

    ClrScr();
    // SDL_BlitText("请稍侯。。。", screen, 200, 150, message_font, message_color);
    SDL_Flip(screen);

    stateStack.pop();
    if(g_rwFlag == READ_RECORD){
        if(LoadData(filePath)) { // read failed
            show_dialog("读取进度失败，或是记录不存在，或是旧版本的记录！");
        }
        else {
			GetMapbyID(Aqing.MapID);
			RelayoutNpc();
            show_dialog("读取成功！");
        }
    }
    else { // write record
        if(StoreData(filePath)) { // write failed
            show_dialog("存档失败，检查save目录是否存在！");
        }
        else {
            show_dialog("存档成功！");
        }
    }
}

void check_game_info()
{
    ShowAbout();
    SDL_Flip(screen);
    wait_any_key_press();
}

void check_player_info()
{
    DrawStateDetail();
    SDL_Flip(screen);
    wait_any_key_press();
}

void select_yes()
{
    SetVariableValue("选择", 1);
    RefreshCanvas();
    stateStack.pop();
}

// inline void select_no()
// because this function is to be called through func pointer,
// it can not be defined as inline
void select_no()
{
    SetVariableValue("选择", 0);
    RefreshCanvas();
    stateStack.pop();
}

void exit_game()
{
    stateStack.push(GAME_EXIT_);
}

//主角在地图走动的函数
void MainMove()
{
	RefreshCanvas();//先显示后计算

    Uint8 *keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_ESCAPE]) {
        WaitKeyRelease();
		// DrawSystemMenu();
        // stateStack.push(SYSTEM_MENU_);
        g_menuFlag = SYSTEM_MENU;
        stateStack.push(GAME_MENU_);
		// Flag = SYSTEM_MENU_;
		return;
	}

    if (keys[SDLK_SPACE]) {
        current_npc = FindNpc();
        if (current_npc) {
            current_npc_id = current_npc->ID;
            RunScripts(current_npc->Name);
        }
		// if(current_npc_id) {
        //     stateStack.push(TREAT_NPC_);
		// 	// Flag = TREAT_NPC_;
		else {
			TrapNum = current_map->check_trap(Aqing.X, Aqing.Y);
			if (TrapNum>=100 && TrapNum < 200)
				QueryMessage(TrapNum);
		}
		return;
	}

	//计算玩家所在的格子
	int grid_x, grid_y; 
	grid_x = Aqing.X>>5;
	grid_y = Aqing.Y >>5;

    if (keys[SDLK_DOWN]) {
		Aqing.Dir = 0;
		Aqing.Step = Aqing.Step +1;
		if(Aqing.Step > 2)
		Aqing.Step = 1;
		Aqing.Y += speed;
		if(CrushCheck()) {
			Aqing.Y -= speed;
		}
		if(current_map->if_block(Aqing.X, Aqing.Y))
			Aqing.Y = (grid_y+1)*32-10;
	}
	else if(keys[SDLK_RIGHT])	//按下方向键右
	{
		Aqing.Dir = 2;
		Aqing.Step = Aqing.Step +1;
		if(Aqing.Step > 2)
			Aqing.Step = 1;
		Aqing.X += speed;

		if(CrushCheck())
			Aqing.X -= speed;

		if(current_map->if_block (Aqing.X, Aqing.Y))
			Aqing.X = (grid_x+1)*32-Aqing.Width/2;
	}
	else if(keys[SDLK_LEFT])	//按下方向键左
	{
		Aqing.Dir = 1;
		Aqing.Step = Aqing.Step +1;
		if(Aqing.Step > 2)
			Aqing.Step = 1;
		Aqing.X -= speed;

		if(CrushCheck())
			Aqing.X += speed;
		if(current_map->if_block (Aqing.X, Aqing.Y))
			Aqing.X = grid_x*32+Aqing.Width/2;
	}
	else if(keys[SDLK_UP])	//按下方向键上
	{
		Aqing.Dir = 3;
		Aqing.Step = Aqing.Step +1;
		if(Aqing.Step > 2)
			Aqing.Step = 1;
		Aqing.Y -= speed;

		if(CrushCheck())
			Aqing.Y += speed;
		if(current_map->if_block (Aqing.X, Aqing.Y))
			Aqing.Y = grid_y*32;
	}
	else {
		if(Aqing.Step)
			Aqing.Step = 0;
	}

	BorderCheck(&Aqing.X ,&Aqing.Y );
	TrapNum = current_map->check_trap(Aqing.X, Aqing.Y);
	if (TrapNum>= 10 &&TrapNum < 100)
	{
		SceneChange();
	}
	else if (TrapNum >=200 && TrapNum <400)
	{
        stateStack.push(AUTO_PLAY_);
		// Flag = AUTO_PLAY_;
	}
}

void game_menu()
{
    Menu * menus;
    short menuNum;
    SDL_Event e;

    switch(g_menuFlag){
    case TITLE_MENU:
        DrawTitle();
        menus = g_startMenus;
        menuNum = START_MENU_NUM;
        break;
    case SYSTEM_MENU:
        DrawSystemMenu();
        menus = g_sysMenus;
        menuNum = SYS_MENU_NUM;
        break;
    case SELECT_MENU:
        menus = g_selectMenus;
        menuNum = SELECT_MENU_NUM;
        draw_menu_group(menus, menuNum, screen);
        break;
    case RECORD_MENU:
        menus = g_recordMenus;
        menuNum = RECORD_MENU_NUM;
        draw_menu_group(menus, menuNum, screen);
    }
    SDL_Flip(screen);

    SDL_WaitEvent(&e);
    if(e.type == SDL_KEYDOWN){
        switch(e.key.keysym.sym){
        case SDLK_ESCAPE:
            if(g_menuFlag == TITLE_MENU)
                exit_game();
            else if(g_menuFlag == SYSTEM_MENU)
                stateStack.pop();
            else if(g_menuFlag == RECORD_MENU)
                g_menuFlag = g_oldMenuFlag;
            break;
        case SDLK_SPACE:
        case SDLK_RETURN:
        {
            PlayWavSound(SELECT);
            // 一般 case 分支内不允许定义变量，要定义可以象这样用大括号包围起来
            short selected = get_selected_menu(menus, menuNum);
            if(menus[selected].on_click)
                menus[selected].on_click();
            break;
        }
        case SDLK_UP:
            PlayWavSound(CHANGE_SEL);
            change_selected_menu(menus, menuNum, -1);
            break;
        case SDLK_DOWN:
            PlayWavSound(CHANGE_SEL);
            change_selected_menu(menus, menuNum, 1);
        } // end switch keysym 
    } // end if SDLK_UP
}

//开始战斗函数
void InitFight()
{
    playerFighter.pY = g_currentEnemy->pY;
    if(playerFighter.attacking == g_currentEnemy->attacking )
        playerFighter.attacking = !g_currentEnemy->attacking;

    frame_fight =0;
    fight_frame_num = 0;
    round_num = 0;
}

//战斗中
void Fighting()
{
	UpdateFight();

    // add wizard key: x
    Uint8 *keys = SDL_GetKeyState(NULL);
    if(keys[SDLK_x]){
        fight_win();
        return;
    }

	fight_frame_num++;//总的战斗帧数增加
	frame_fight++;

	//更新回合数
	if(fight_frame_num % 12 == 0)
	{
		round_num ++;
        SetVariableValue("回合数", round_num);
	}

	if (frame_fight >= 6) {
		if(playerFighter.attacking) { // AQing attack
			if (!(g_currentEnemy->can_defend(playerFighter.attack )))
                fight_win();
		}
		else {
			if(!(playerFighter.can_defend(g_currentEnemy->attack)))
                fight_fail();
		}
		frame_fight =0;
		playerFighter.attacking = ! playerFighter.attacking;
		g_currentEnemy->attacking = !g_currentEnemy->attacking;
	}
}

void fight_win()
{
	char temp[512];

    PlayWavSound(QING_VICT);
    g_currentEnemy->hp = g_currentEnemy->maxHp;
    SetVariableValue("战胜", 1);

    int addHp, addDefend, addAttack;
    if (g_currentEnemy->attack <= playerFighter.defend)
    {
        if(round_num > 2)
            addHp = rand() % round_num;
        else
            addHp = rand() % 2;
        addAttack = rand() % 2;
        addDefend = rand() % 2;
    }
    else {
        if(round_num > 2)
            addHp = g_currentEnemy->maxHp / 10 + rand() % round_num;
        else
            addHp = g_currentEnemy->maxHp / 10 + rand() % 2;
        addAttack = g_currentEnemy->attack / 10 + rand() % 2;
        addDefend = g_currentEnemy->defend / 10 + rand() % 2;
    }

    playerFighter.maxHp += addHp;
    playerFighter.attack += addAttack;
    playerFighter.defend += addDefend;

    sprintf( temp, "你战胜了%s！@最大战斗力提升%d，攻击力提升%d，防御力提升%d！", 
             g_currentEnemy->name, addHp, addAttack, addDefend);
    g_dialog.set_text(temp);
    g_dialog.show(screen);

    stateStack.pop();
    stateStack.push(GAME_MESSAGE_);
}

void fight_fail()
{
	char temp[512];
    short addHp;

    SetVariableValue("战胜", 0);
    PlayWavSound(QING_FAIL);
    g_currentEnemy->hp = g_currentEnemy->maxHp;

    if(round_num > 2)
        addHp = rand() % round_num;
    else
        addHp = rand() % 2;
    playerFighter.maxHp += addHp;

    sprintf(temp, "你输给了%s！战斗力增加%d。",
            g_currentEnemy->name, addHp );
    g_dialog.set_text(temp);
    g_dialog.show(screen);

    stateStack.pop();
    stateStack.push(GAME_MESSAGE_);
}

//系统消息显示中
void GameMessage()
{
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type = SDL_KEYUP){
		if (g_dialog.is_over()) {
            stateStack.pop();
		}
		else {			
			g_dialog.show(screen);
		}
	}
}

//自动剧情
void AutoPlay()
{
	switch(TrapNum)
	{
	case 200:	//从越国到吴国
        stateStack.pop();
        RunScripts("从越国到吴国");
		break;
	case 201:	//从吴国到越国
        stateStack.pop();
        RunScripts("从吴国到越国");
		break;
	case 202:	//第一次来到赵大爷身边
        stateStack.pop();
        RunScripts("初见张大爷");
		break;
	case 203:
        stateStack.pop();
        RunScripts("初见李大叔");
		break;

	case 205:
        stateStack.pop();
        RunScripts("遭遇匪徒");
		break;

	case 206:	//第一次见张平的剧情
        stateStack.pop();
        RunScripts("遭遇商平");
		break;
	case 207:
        stateStack.pop();
        RunScripts("卫兵阻拦");
		break;
	case 209:
        stateStack.pop();
        RunScripts("强闯吴宫");
		break;

	case 300:	//阿青来到吴王宫
        stateStack.pop();
        RunScripts("看见西施");
		break;
	default:
        stateStack.pop();
	}

}

//游戏退出
void GameExit()
{
    DrawEnd();
    SDL_Flip(screen);

	// current_map = NULL;
	// g_currentEnemy = NULL;

    if(g_script) {
        delete g_script;
        g_script = NULL;
    }

    SDL_Event e;
    SDL_WaitEvent(&e);
    if(e.type == SDL_KEYDOWN)
        game_running = 0;

    //SDL_Delay(2000);
    // static short counter = 6;
    // if(counter == 0)
    //counter --;
}

//查看作品信息
void wait_any_key_press()
{
    SDL_Event e;
    while(1){
        SDL_WaitEvent(&e);
        if(e.type == SDL_KEYDOWN)
            return;
    }
}

//****************************************************************************

//*************************更新游戏画面函数**********************************

//清屏
// inline void ClrScr()
// {
//     SDL_FillRect(screen, NULL, 0);
// }

//更新非战斗画面
void RefreshCanvas()
{
	current_map->draw_map(screen, Aqing, item);
	DrawState();
	//FlipPage();
}

void show_dialog(const char * content)
{
    g_dialog.set_text(content);
    g_dialog.show(screen);
    stateStack.push(GAME_MESSAGE_);
}

//更新战斗画面
void UpdateFight()
{
	current_map->draw_map_only(screen);
	if (!playerFighter.attacking)
	{
		playerFighter.draw_self(frame_fight, screen, g_currentEnemy->pX, g_currentEnemy->pY,g_currentEnemy->attack );
		g_currentEnemy->draw_self(frame_fight, screen, playerFighter.pX, playerFighter.pY ,playerFighter.attack);
	}
	else
	{
		g_currentEnemy->draw_self(frame_fight, screen, playerFighter.pX, playerFighter.pY ,playerFighter.attack);
		playerFighter.draw_self(frame_fight, screen, g_currentEnemy->pX, g_currentEnemy->pY,g_currentEnemy->attack );
	}
	DrawState();
	DrawRoundNum();
	//FlipPage();
	if (frame_fight == 3)
	{
		if (playerFighter.attacking)
		{
			play_sound(playerFighter.sound);
		}
		else
		{
			play_sound(g_currentEnemy->sound);
		}
	}
}


//画系统菜单
void DrawSystemMenu()
{
    RefreshCanvas();
    draw_menu_group(g_sysMenus, SYS_MENU_NUM, screen);
}


//画片头
void DrawTitle()
{
    DrawPic("./pic/title.bmp");
	draw_menu_group(g_startMenus, START_MENU_NUM, screen);
}

//画片尾
void DrawEnd()
{
    SDL_Surface *end = SDL_LoadBMP("./pic/end.bmp");
    SDL_BlitSurface(end, NULL, screen, NULL);
    SDL_FreeSurface(end);
	//DrawStartMenu();
}

//画状态面板
void DrawState()
{
    SDL_Rect rect_src, rect_dest;
    rect_src.x = 0;
    rect_src.y = 0;
    rect_src.w = 160;
    rect_src.h = 30;

    rect_dest.x = SCR_W -160;
    rect_dest.y = 0;
    rect_dest.w = 160;
    rect_dest.h = 30;

    SDL_BlitSurface(state, &rect_src, screen, &rect_dest);

	
	int width = (int)((playerFighter.get_hp_percent()*100));
	if (width)
	{
        rect_src.x = 0;
        rect_src.y = 31;
        rect_src.w = width;
        rect_src.h = 10;
        rect_dest.x = SCR_W-7-width;
        rect_dest.y = 9;
        rect_dest.w = width;
        rect_dest.h = 10;
        SDL_BlitSurface(state, &rect_src, screen, &rect_dest);
	}
	

	char temp[20];
	sprintf( temp, "%d/%d", playerFighter.hp, playerFighter.maxHp);
    SDL_BlitText(temp, screen, SCR_W-70, 7, about_font, about_color);
}

//显示详细状态信息
void DrawStateDetail()
{
    SDL_Rect rect_src, rect_dest;
    rect_src.x = 0;
    rect_src.y = 0;
    rect_src.w = 300;
    rect_src.h = 200;
    rect_dest.x = 120;
    rect_dest.y = 40;
    rect_dest.w = 300;
    rect_dest.h = 200;
    SDL_BlitSurface(info, &rect_src, screen, &rect_dest);

	char temp[50];
    SDL_BlitText("详细状态信息：", screen, 160, 70, g_menuFont, g_menuColor);
	sprintf(temp, "姓名：%s ", Aqing.Name);
    SDL_BlitText(temp, screen, 160, 100, g_menuFont, g_menuColor);
	sprintf(temp, "战斗力：%d / %d ", playerFighter.hp, playerFighter.maxHp );
    SDL_BlitText(temp, screen, 160, 120, g_menuFont, g_menuColor);
	sprintf(temp, "攻击力：%d ", playerFighter.attack);
    SDL_BlitText(temp, screen, 160, 140, g_menuFont, g_menuColor);
	sprintf(temp, "防御力：%d ", playerFighter.defend);
    SDL_BlitText(temp, screen, 160, 160, g_menuFont, g_menuColor);
}

//显示作品信息
void ShowAbout()
{
    SDL_Rect rect_src, rect_dest;
    rect_src.x = 0;
    rect_src.y = 0;
    rect_src.w = 300;
    rect_src.h = 300;

    rect_dest.x = 80;
    rect_dest.y = 5;
    rect_dest.w = 300;
    rect_dest.h = 300;

    SDL_BlitSurface(info, &rect_src, screen, &rect_dest);
	
    SDL_BlitText("作品信息", screen, 115, 40, g_menuFont, about_color);
    SDL_BlitText("作品名称：越女剑 for Linux", screen, 115, 65, about_font, about_color);
    SDL_BlitText("版    本：1.5", screen, 115, 80, about_font, about_color);
    SDL_BlitText("功能按键：方向键，空格键和ESC键", screen, 115, 95, about_font, about_color);
    SDL_BlitText("资源来源：地图元素由自己绘制", screen, 115, 110, about_font, about_color);
    SDL_BlitText("          其它图片和声音源于网络", screen, 115, 125, about_font, about_color);
    SDL_BlitText("          对这些资源的作者表示感谢！", screen, 115, 140, about_font, about_color);
    SDL_BlitText("剧    情：金庸先生《越女剑》", screen, 115, 155, about_font, about_color);
    SDL_BlitText("          情节已被本人无恶意地篡改", screen, 115, 170, about_font, about_color);
    SDL_BlitText("          希望金先生不要见怪。", screen, 115, 185, about_font, about_color);
    SDL_BlitText("作    者：吴吉庆 (jiqingwu@gmail.com)", screen, 115, 200, about_font, about_color);
    SDL_BlitText("完成时间：2013年7月27日", screen, 115, 215, about_font, about_color);
}

//画当前的回合数
void DrawRoundNum()
{
	char temp[30];
	sprintf( temp, "当前回合数：%d", round_num);
    SDL_BlitText(temp, screen, 0, 0, message_font, message_color);
}

//在屏幕上显示图片
void DrawPic(const char * path)
{
    SDL_Surface *temp = SDL_LoadBMP(path);
    if (!temp){
        printf("Unable to load bitmap.\n");
    }
    else {
        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        dest.w = temp->w;
        dest.h = temp->h;
        SDL_BlitSurface(temp, NULL, screen, &dest);
        SDL_FreeSurface(temp);
    }
}

//********************************************************************************


//****************************游戏辅助函数***************************************

//通过id得到NPC的地址
Role * GetNpcbyID(int id)
{
	switch(id)
	{
	case 1:	//1
		return &Aqing;
	case 2:	//2
		return &Fanli;
	case 3:	//3
		return &Xishi;
	case 4:	//4
		return &Yehaizi;
	case 5:	//5
		return &Jianke;
	case 10:	//6
		return &AqingMa;
	case 11:	//7
		return &ZhangDaye;
	case 12:	//8
		return &LiDashu;
	case 13:	//9
		return &YueWeibing1;
	case 14:	//10
		return &YueWeibing2;
	case 15:	//11
		return &Puren;
	case 16:	//12
		return &Wushi;
	case 101:	//13
		return &WuWeibing1;
	case 102:	//14
		return &WuWeibing2;
	case 110:	//15
		return &Wujianshi1 ;
	case 111:	//16
		return &Wujianshi2;
	case 200:	//17
		return &Sheep1;
	case 201:	//18
		return &Sheep2 ;
	case 300:	//19
		return &CaiTan;
	case 301:	//20
		return &box_fanli;
	case 302:	//21
		return &box_caoyuan;
	case 303:	//22
		return &QingBed;
	case 304:	//23
		return &box_jianke;
	case 112:	//24
		return &Feitu;
	case 113:	//25
		return &Shangping;
	case 114:	//26
		return &Shiwei;
	default:
		return 0;
	}
}

//通过主角的MapID恢复当前地图
void GetMapbyID(int id)
{
	switch(id)
	{
	case 10:
		current_map = &Map_aqing;
		break;
	case 11:
		current_map = &Map_shaoxing;
		break;
	case 12:
		current_map = &Map_citydoor;
		break;
	case 13:
		current_map = &Map_fanli;
		break;
	case 14:
		current_map = &Map_outside;
		break;
	case 15:
		current_map = &Map_caoyuan;
		break;
	case 16:
		current_map = &Map_Wuguo;
		break;
	case 17:
		current_map = &Map_Gongdian;
		break;
	case 18:
		current_map = &Map_Xiangfang;
		break;
	default:
		current_map = &Map_aqing;
	}
}

//新地图载入时，重新安排上面的NPC
void RelayoutNpc()
{
	Role * temp = NULL;
	current_map->Npc = NULL;
	current_map->NpcNum = 0;
	for(int i=0; i<15; i++)
	{
		if (current_map->Npcs[i])
		{
			temp = GetNpcbyID(current_map->Npcs[i]);
			//BorderCheck(&temp->X, &temp->Y);
			current_map->add_npc(temp, temp->X, temp->Y );
		}
	}
}

//主角走到新地图
void GotoMap(Map *m)
{
	Aqing.MapID = m->ID; 
	current_map = m;
	RelayoutNpc();
}

//场景切换函数
void SceneChange( )
{
	switch(TrapNum)
	{
	case 10:	//从绍兴街上到阿青家中
		Aqing.set_location(3,0,SCR_W/2,SCR_H-45);
		GotoMap(&Map_aqing);
		break;
		
	case 11:	
 		if(Aqing.MapID == 10)	//从阿青家到绍兴街上
		{
			Aqing.set_location(1,0,SCR_W-40, SCR_H-60);
		}
		else	//从绍兴城门到绍兴街上
		{
			Aqing.set_location(2,0,40, SCR_H-60);
		}
		GotoMap(&Map_shaoxing);
		break;
	case 12:
		
		if(Aqing.MapID == 11)	//从绍兴街到绍兴城门
		{
			Aqing.set_location(1,0, SCR_W-40, SCR_H-100);
		}
		else if(Aqing.MapID == 13)	//从范蠡家到绍兴城门
		{
			Aqing.set_location(3,0,192,SCR_H-40);			
		}
		else	//从郊外到绍兴城门
		{
			Aqing.set_location(0,0,192,100);	
		}
		GotoMap(&Map_citydoor);
		break;
	case 13:
		if(Aqing.MapID == 12)	//从绍兴城门到范蠡府上
		{
			Aqing.set_location(1,0,SCR_W-45,180);			
		}
		else	//从厢房到范蠡府院
		{
			Aqing.set_location(0,0,400,140);
		}
		GotoMap(&Map_fanli);
		break;
	case 14:
		if(Aqing.MapID == 12)	//从绍兴城门到郊外
		{
			Aqing.set_location(3,0, 220, SCR_H-40);
		}
		else if(Aqing.MapID == 15)	//从草原到郊外
		{
			Aqing.set_location(1,0,SCR_W-40, 100);		
		}
		GotoMap(&Map_outside);
		break;
	case 15:	//从越过郊外到草原
		Aqing.set_location(3,0,240,SCR_H-40);
		GotoMap(&Map_caoyuan);
		break;
	case 16:	//从吴王宫到吴国城门
		Aqing.set_location(0,0, 260,230);
		GotoMap(&Map_Wuguo);
		break;
	case 17:	//从吴国城门到吴王宫
		Aqing.set_location(3,0,240,SCR_H-40);
		GotoMap(&Map_Gongdian);
		break;

	default:
		return;
	}
}

void WaitKeyRelease()
{
  SDL_Event e;
  while(true){
    SDL_PollEvent(&e);
    if(e.type == SDL_KEYUP)
      break;
    else
      SDL_Delay(20);
  }
}

//响应在行走过程中敲空格的事件
void QueryMessage(short n)
{
	
	switch(n)
	{
	case 100:
        RunScripts("酒楼门口");
		break;
	case 101:
        RunScripts("酒楼右边房间");
		break;
	case 102:
        RunScripts("无赖掌柜");
		break;
	case 103:
        RunScripts("进厢房");
		break;
	}
	
}

//边界检测函数
void BorderCheck(int *x,int *y)
{
	if(*x-Aqing.Width/2 < 0)
		*x = Aqing.Width/2 + 1;
	if((*x+Aqing.Width/2) > SCR_W)
		*x = SCR_W-Aqing.Width/2 -1;
	if((*y-((Aqing.Height)>>3)*7)<0)
		*y = ((Aqing.Height)>>3) *7 + 1;
	if(*y+((Aqing.Height)>>3)>SCR_H)
		*y = SCR_H - ((Aqing.Height)>>3) -1;
}

short CrushCheck()	//碰撞检测
{
	
	Role * temp = current_map->Npc;
	while(temp)
	{
		if (abs(Aqing.X - temp->X )< 20 && abs(Aqing.Y - temp->Y )<20)
		{
			return 1;
		}
		temp = temp->R;
	}
	return 0;

}

Role * FindNpc()	//寻找玩家面对的npc
{

	Role *temp = current_map->Npc;
	short id = 0;
	while(temp)
	{
		switch(Aqing.Dir)
		{
		case 0:
			if(abs(Aqing.X - temp->X)<15 &&(temp->Y- Aqing.Y)<50 && (temp->Y- Aqing.Y)>0)
                return temp;
			break;
		case 1:
			if((Aqing.X - temp->X )>0 && (Aqing.X - temp->X )<50 && abs(Aqing.Y - temp->Y)<15)
                return temp;
			break;
		case 2:
			if((temp->X - Aqing.X )>0 && (temp->X - Aqing.X )<50 && abs(Aqing.Y - temp->Y)<15)
                return temp;
			break;
		case 3:
			if(abs(Aqing.X - temp->X)<15 &&(Aqing.Y - temp->Y)<50 && (Aqing.Y - temp->Y)>0)
                return temp;
			break;
		}
		temp = temp->R;
	}

	return NULL;
}



//*****************************游戏数据函数*******************************************

//初始化游戏数据
void InitData()
{
    //初始化菜单
    init_start_menus();
    init_system_menus();
    init_select_menus();
    init_record_menus();

	//初始化对话框
	g_dialog.set_dlg(dlg, dlg_font, dlg_color);
    
    InitMaps();
    InitRoles();
    init_fighters();

	//地图中加入Npc
	Map_aqing.add_npc(&AqingMa, AqingMa.X, AqingMa.Y);	
	Map_aqing.add_npc(&QingBed, QingBed.X, QingBed.Y);	
	Map_aqing.add_npc(&Sheep1, Sheep1.X, Sheep1.Y );	
	Map_aqing.add_npc(&Sheep2, Sheep2.X, Sheep2.Y );	
	Map_shaoxing.add_npc(&ZhangDaye, ZhangDaye.X, ZhangDaye.Y );	
	Map_shaoxing.add_npc(&LiDashu, LiDashu.X, LiDashu.Y);	
	Map_shaoxing.add_npc(&CaiTan, CaiTan.X, CaiTan.Y);
	Map_shaoxing.add_npc(&Wujianshi1, Wujianshi1.X, Wujianshi1.Y);
	Map_shaoxing.add_npc(&Wujianshi2, Wujianshi2.X, Wujianshi2.Y);
	Map_citydoor.add_npc(&YueWeibing1, YueWeibing1.X, YueWeibing1.Y);
	Map_citydoor.add_npc(&YueWeibing2, YueWeibing2.X, YueWeibing2.Y);
	Map_citydoor.add_npc(&Wushi, Wushi.X, Wushi.Y);
	Map_fanli.add_npc(&box_fanli, box_fanli.X, box_fanli.Y);
	Map_fanli.add_npc(&Fanli, Fanli.X, Fanli.Y);
	Map_fanli.add_npc(&Puren, Puren.X, Puren.Y);
	Map_Wuguo.add_npc(&WuWeibing1, WuWeibing1.X, WuWeibing1.Y );
	Map_Wuguo.add_npc(&WuWeibing2, WuWeibing2.X, WuWeibing2.Y);
	Map_Xiangfang.add_npc(&Jianke, Jianke.X, Jianke.Y);
	Map_Xiangfang.add_npc(&box_jianke,box_jianke.X,box_jianke.Y);
	Map_caoyuan.add_npc(&Shangping, Shangping.X, Shangping.Y);

	
	//初始化全局游戏变量
	current_map = &Map_aqing;
	g_currentEnemy = NULL;
	current_npc_id = 0;

    g_script = new CScript("scripts/script", '@');
    if (!g_script){
        cout << "Load script failed! exit!" << endl;
        exit(1);
    }

}

//读取游戏数据
short LoadData( char * path)
{
	FILE *fp;

	//打开文件
	fp = fopen(path,"rb");
	if(fp == NULL)
	{
        printf("load failed!\n");
		return 1;
	}

	//读取玩家数据
	fread(&Aqing.X, sizeof(int),1,fp);
	fread(&Aqing.Y, sizeof(int),1,fp);
	fread(&Aqing.Dir, sizeof(int),1,fp);
	fread(&Aqing.Step, sizeof(int),1,fp);
	fread(&Aqing.MapID, sizeof(int),1,fp);
	fread(&playerFighter.maxHp, sizeof(short), 1, fp);
	fread(&playerFighter.hp, sizeof(short), 1, fp);
	fread(&playerFighter.attack, sizeof(short), 1,fp);
	fread(&playerFighter.defend, sizeof(short), 1, fp);

	//读取其它NPC数据
	fread(&Puren.X, sizeof(int), 1, fp);
	fread(&Puren.Y, sizeof(int), 1, fp);
	fread(&Jianke.X, sizeof(int), 1, fp);
	fread(&Jianke.Y, sizeof(int), 1, fp);
	fread(&Yehaizi.X, sizeof(int), 1, fp);
	fread(&Yehaizi.Y, sizeof(int), 1, fp);
	fread(&Fanli.X, sizeof(int), 1, fp);
	fread(&Fanli.Y, sizeof(int), 1, fp);
	fread(&box_fanli.Step, sizeof(int), 1, fp);
	fread(&box_jianke.Step, sizeof(int), 1, fp);
	fread(&box_caoyuan.Step, sizeof(int), 1, fp);

	//读取地图陷阱数据
	int i,j;
	for(i = 0; i< 10; i++)
	{
		for(j =0; j< 15; j++)
		{
			fread(&Map_shaoxing.Trap[i][j], sizeof(short), 1, fp);
			fread(&Map_outside.Trap[i][j], sizeof(short), 1, fp);
			fread(&Map_Wuguo.Trap[i][j], sizeof(short), 1, fp);
			fread(&Map_Gongdian.Trap[i][j], sizeof(short), 1, fp);
			fread(&Map_caoyuan.Trap[i][j], sizeof(short), 1, fp);
		}
	}

    fread(&Map_aqing.Npcs, sizeof(int), 15, fp);		//1
    fread(&Map_shaoxing.Npcs, sizeof(int), 15, fp);	//2
    fread(&Map_citydoor.Npcs, sizeof(int), 15, fp);	//3
    fread(&Map_fanli.Npcs, sizeof(int), 15, fp);		//4
    fread(&Map_outside.Npcs, sizeof(int), 15, fp);	//5
    fread(&Map_caoyuan.Npcs, sizeof(int), 15, fp);	//6
    fread(&Map_Wuguo.Npcs, sizeof(int), 15, fp);		//7
    fread(&Map_Gongdian.Npcs, sizeof(int), 15, fp);	//8
    fread(&Map_Xiangfang.Npcs, sizeof(int), 15, fp);	//9

    fread(vars, sizeof(stVariable), VAR_NUMBER, fp);

	fclose(fp);
	return 0;
	
}

//存储游戏数据
short StoreData(char * path)
{
	FILE *fp;

	//打开文件
	fp = fopen(path,"wb");
	if(fp == NULL)
	{
        printf("store failed!\n");
		return 1;
	}

	//存储玩家数据
	fwrite(&Aqing.X, sizeof(int), 1, fp);
	fwrite(&Aqing.Y, sizeof(int), 1, fp);
	fwrite(&Aqing.Dir, sizeof(int), 1, fp);
	fwrite(&Aqing.Step, sizeof(int), 1, fp);
	fwrite(&Aqing.MapID, sizeof(int), 1, fp);
	fwrite(&playerFighter.maxHp, sizeof(short), 1, fp);
	fwrite(&playerFighter.hp, sizeof(short), 1, fp);
	fwrite(&playerFighter.attack, sizeof(short), 1,fp);
	fwrite(&playerFighter.defend, sizeof(short), 1, fp);

	//存储其它NPC数据
	fwrite(&Puren.X, sizeof(int), 1, fp);
	fwrite(&Puren.Y, sizeof(int), 1, fp);
	fwrite(&Jianke.X, sizeof(int), 1, fp);
	fwrite(&Jianke.Y, sizeof(int), 1, fp);
	fwrite(&Yehaizi.X, sizeof(int), 1, fp);
	fwrite(&Yehaizi.Y, sizeof(int), 1, fp);
	fwrite(&Fanli.X, sizeof(int), 1, fp);
	fwrite(&Fanli.Y, sizeof(int), 1, fp);
	fwrite(&box_fanli.Step, sizeof(int), 1, fp);
	fwrite(&box_jianke.Step, sizeof(int), 1, fp);
	fwrite(&box_caoyuan.Step, sizeof(int), 1, fp);

	//存储地图陷阱数据
	int i,j;
	for(i = 0; i< 10; i++)
	{
		for(j =0; j< 15; j++)
		{
			fwrite(&Map_shaoxing.Trap[i][j], sizeof(short), 1, fp);
			fwrite(&Map_outside.Trap[i][j], sizeof(short), 1, fp);
			fwrite(&Map_Wuguo.Trap[i][j], sizeof(short), 1, fp);
			fwrite(&Map_Gongdian.Trap[i][j], sizeof(short), 1, fp);
			fwrite(&Map_caoyuan.Trap[i][j], sizeof(short), 1, fp);
		}
	}

    fwrite(&Map_aqing.Npcs, sizeof(int), 15, fp);		//1
    fwrite(&Map_shaoxing.Npcs, sizeof(int), 15, fp);	//2
    fwrite(&Map_citydoor.Npcs, sizeof(int), 15, fp);	//3
    fwrite(&Map_fanli.Npcs, sizeof(int), 15, fp);		//4
    fwrite(&Map_outside.Npcs, sizeof(int), 15, fp);	//5
    fwrite(&Map_caoyuan.Npcs, sizeof(int), 15, fp);	//6
    fwrite(&Map_Wuguo.Npcs, sizeof(int), 15, fp);		//7
    fwrite(&Map_Gongdian.Npcs, sizeof(int), 15, fp);	//8
    fwrite(&Map_Xiangfang.Npcs, sizeof(int), 15, fp);	//9
		
    fwrite(vars, sizeof(stVariable), VAR_NUMBER, fp);
	//关闭文件
	fclose(fp);
	return 0;
}

