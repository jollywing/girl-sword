//role.h
//01 以后在定义类的时候应该注意，类的数据成员尽量声明为私有。
//01 外部只能通过类的接口函数才能访问类的数据成员。
#ifndef __ROLEH__
#define __ROLEH__

#include <SDL/SDL.h>
//#include "map.h"

class Role
{
public:
	char Name[32];
	//int Life;
	//int Energy;
	short ID;
	int Height;
	int Width;
	int X;	//相对坐标
	int Y;	
	int Dir;
	int Step;
    SDL_Surface * Surf;
	int MapID;
	short EnemyID;
	Role * R;

public:
	Role();
	~Role();
	void set_location(int dir, int step, int x, int y);
	void role_set(short id, const char *name, int h, int w, 
            int x,int y, int dir, int step,
            SDL_Surface * surf,int map_id, short enemy_id);
	void draw_self(SDL_Surface * dest_surf);
	int move_to(int grid_x, int grid_y);

};

//角色
extern Role Aqing;		//1 主角阿青
extern Role QingBed;	//2 阿青的床
extern Role AqingMa;	//3	阿青妈
extern Role Sheep1;	//4
extern Role Sheep2;	//5 两只绵羊
extern Role ZhangDaye;	//6 爱唠叨的张大爷
extern Role LiDashu;	//7	卖菜的李大叔
extern Role CaiTan;	//8 李大叔的菜摊
extern Role Wujianshi1;//9	
extern Role Wujianshi2;//10 两名在绍兴城撒野的吴国剑士
extern Role YueWeibing1;	//11
extern Role YueWeibing2;	//12 城门的卫兵甲和卫兵乙
extern Role Puren;		//13 仆人
extern Role Fanli;		//14 范蠡
extern Role box_fanli;	//15 范蠡家的宝箱
extern Role WuWeibing1;//16 
extern Role WuWeibing2;//17 吴国的卫兵甲和卫兵乙
extern Role Xishi;		//18 美人西施
extern Role Yehaizi;	//19 草原上的野孩子
extern Role box_caoyuan;	//20 草原上宝箱
extern Role Jianke;	//21 范府厢房中的神秘剑客
extern Role box_jianke;	//22 剑客的宝箱
extern Role Wushi;		//23 绍兴城中的武师
extern Role Feitu;		//24 郊外的匪徒
extern Role Shangping;	//25 悍匪商平
extern Role Shiwei;	//26 西施的侍卫


#endif
