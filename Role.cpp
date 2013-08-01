#include "Role.h"
//#include "girl.h"

short current_npc_id;	//当前的Npc的id
Role *current_npc;	//当前的NPC指针
short npc_dest_x, npc_dest_y;	//npc的目的地

//角色
Role Aqing;		//1 主角阿青
Role QingBed;	//2 阿青的床
Role AqingMa;	//3	阿青妈
Role Sheep1;	//4
Role Sheep2;	//5 两只绵羊
Role ZhangDaye;	//6 爱唠叨的张大爷
Role LiDashu;	//7	卖菜的李大叔
Role CaiTan;	//8 李大叔的菜摊
Role Wujianshi1;//9	
Role Wujianshi2;//10 两名在绍兴城撒野的吴国剑士
Role YueWeibing1;	//11
Role YueWeibing2;	//12 城门的卫兵甲和卫兵乙
Role Puren;		//13 仆人
Role Fanli;		//14 范蠡
Role box_fanli;	//15 范蠡家的宝箱
Role WuWeibing1;//16 
Role WuWeibing2;//17 吴国的卫兵甲和卫兵乙
Role Xishi;		//18 美人西施
Role Yehaizi;	//19 草原上的野孩子
Role box_caoyuan;	//20 草原上宝箱
Role Jianke;	//21 范府厢房中的神秘剑客
Role box_jianke;	//22 剑客的宝箱
Role Wushi;		//23 绍兴城中的武师
Role Feitu;		//24 郊外的匪徒
Role Shangping;	//25 悍匪商平
Role Shiwei;	//26 西施的侍卫

stRoleIndex roles[ROLE_NUM];

bool AddRole(char * name, Role * r)
{
	for(int i=0; i < ROLE_NUM; ++i)
	{
		if( !strcmp(roles[i].szName, "") )
		{
			strcpy( roles[i].szName, name);
			roles[i].pRole = r;
			return true;
		}
	}
	return false;
}

void ClearRole()
{
	for(int i=0; i < ROLE_NUM; ++i)
	{
		strcpy( roles[i].szName, "");		
	}
}

Role * GetRoleAddr(char * name)
{
	for(int i=0; i < ROLE_NUM; ++i)
	{
		if( !strcmp(roles[i].szName, name) )
			return roles[i].pRole;
	}
	return NULL;
}

void InitRoles()
{
	//初始化角色
	Aqing.role_set (1,"阿青",90,55,250,150,0,0,hero,10, 0);	//1
    AddRole(Aqing.Name, &Aqing);

	QingBed.role_set(303, "阿青的小床", 35, 35, 430,80,4,0, item, 10, 0);	//2
    AddRole(QingBed.Name, &QingBed);

	AqingMa.role_set(10,"阿青的娘", 95,60, 100, 85, 0, 3, other_yue,10,0);	//3
    AddRole(AqingMa.Name, &AqingMa);

	Sheep1.role_set(200,"绵羊1", 48,48, 400,210, 3,0, sheep, 10,0);	//4
    AddRole(Sheep1.Name, &Sheep1);

	Sheep2.role_set(201,"绵羊2", 48,48, 380,220, 0,0, sheep, 10, 0);	//5
    AddRole(Sheep2.Name, &Sheep2);

	ZhangDaye.role_set(11,"张大爷", 95, 60, 400, 240, 0, 2, other_yue, 11, 0);//6
    AddRole(ZhangDaye.Name, &ZhangDaye);

	LiDashu.role_set(12,"李大叔", 95,60, 150,307, 1, 2, other_yue, 11, 0);	//7
    AddRole(LiDashu.Name, &LiDashu);

	Puren.role_set(15, "仆人晓月", 95,60, 400,130, 0,6, other_yue, 13, 0);	//8
    AddRole(Puren.Name, &Puren);

	CaiTan.role_set(300,"李大叔的菜摊", 95, 120, 100, 305, 1, 0, other_yue, 11, 0);	//9
    AddRole(CaiTan.Name, &CaiTan);

	Wujianshi1.role_set(110, "吴国剑士甲", 95,60, 150, 270, 1, 4, other_yue, 11, 110);	//10
    AddRole(Wujianshi1.Name, &Wujianshi1);

	Wujianshi2.role_set(111, "吴国剑士乙", 95,60, 100, 240, 1, 5,other_yue, 11, 111);	//11
    AddRole(Wujianshi2.Name, &Wujianshi2);

	YueWeibing1.role_set(13,"越国卫兵1", 95,60, 105,150, 0, 0, other_yue, 12, 0);	//12
    AddRole(YueWeibing1.Name, &YueWeibing1);

	YueWeibing2.role_set(14,"越国卫兵2", 95,60, 305,150, 0, 1, other_yue, 12, 0);	//13
    AddRole(YueWeibing2.Name, &YueWeibing2);

	WuWeibing1.role_set(101,"吴国卫兵1", 95, 60, 200, 240,0, 4, other_yue, 16, 0);	//14
    AddRole(WuWeibing1.Name, &WuWeibing1);

	WuWeibing2.role_set(102,"吴国卫兵2", 95,60, 310, 240, 0,5, other_yue, 16, 0);	//15
    AddRole(WuWeibing2.Name, &WuWeibing2);

	box_fanli.role_set(301,"范大夫的宝箱", 35,35, 150,200, 0,0, item, 13, 0);	//16
    AddRole(box_fanli.Name, &box_fanli);

	Fanli.role_set(2,"范蠡大夫", 80,40, 100, 100, 2,0, FanLi, 13, 0);	//17
    AddRole(Fanli.Name, &Fanli);

	Xishi.role_set(3,"西施", 85, 50, 180, 220, 0,0, XiShi, 17, 0);	//18
    AddRole(Xishi.Name, &Xishi);

	Yehaizi.role_set(4,"草原上的野孩子",95,60, 380, 180, 1, 3, other_yue,15,4 );	//19
    AddRole(Yehaizi.Name, &Yehaizi);

	box_caoyuan.role_set(302,"草原上的宝箱", 35,35, 400, 160, 2,0, item, 15, 0);	//20
    AddRole(box_caoyuan.Name, &box_caoyuan);

	Jianke.role_set(5, "神秘剑客", 95,60, 120,150, 1,6,other_yue,18,5);	//21
    AddRole(Jianke.Name, &Jianke);

	box_jianke.role_set(304, "剑客的宝箱", 35,35,110,110, 1,0, item,18,0);	//22
    AddRole(box_jianke.Name, &box_jianke);

	Wushi.role_set(16, "武师", 95,60, 400,200, 2,0, other_yue, 12,16);	//23
    AddRole(Wushi.Name, &Wushi);

	Feitu.role_set(112, "匪徒", 95,60, 330,150, 2,2, other_yue, 14,112);	//24
    AddRole(Feitu.Name, &Feitu);

	Shangping.role_set(113, "悍匪商平", 95,60, 270,190, 2,1,other_yue, 15,113);	//25
    AddRole(Shangping.Name, &Shangping);

	Shiwei.role_set(114, "吴吉庆", 95,60, 225,225,2,3, other_yue,16, 114);	//26
    AddRole(Shiwei.Name, &Shiwei);

}

Role::Role()
{
	ID = 0;
	strcpy(Name,"");
	Height = 0;
	Width = 0;
	X = 0;
	Y = 0;
	Dir = 0;
	Step = 0;
	Surf = NULL;
	MapID = 10;
	EnemyID = 0;
    R = NULL;
}

Role::~Role()
{
	R = NULL;
	Surf = NULL;
}

void Role::set_location(int dir, int step, int x, int y)
{
	Dir = dir;
	Step = step;
	X = x;
	Y = y;
}

void Role::role_set (short id, const char *name,int h,int w,int x,int y,
					int dir,int step, SDL_Surface * surf,
					int map_id, short enemy_id)
{
	ID = id;
	strcpy(Name,name);
	Height = h;
	Width = w;
	X = x;
	Y = y;
	Dir = dir;
	Step = step;
	Surf = surf;
	MapID = map_id;
	EnemyID = enemy_id;
	R = NULL;
}


void Role::draw_self(SDL_Surface * dest_surf)
{
    SDL_Rect role_src, role_dest;
	role_src.x = Step*Width;
	role_src.y = Dir*Height;
	role_src.w = Width;
	role_src.h = Height;
    role_dest.x = X - (Width/2);
    role_dest.y = Y - ((7*Height)>>3);
    role_dest.w = Width;
    role_dest.h = Height;
    SDL_BlitSurface(Surf, &role_src, dest_surf, &role_dest);
}

/* 当移动完成时返回0 */
int Role::move_to(int grid_x, int grid_y)
{
	int xGrid = X >>5;
	int yGrid = Y >>5;

	if(xGrid < grid_x)
	{
		Dir = 2;
		Step++;
		if (Step >2)
			Step = 1;
		X += 16;
		return 1;
	}

	if(xGrid > grid_x)
	{
		Dir = 1;
		Step++;
		if (Step >2)
			Step = 1;
		X -= 16;
		return 1;
	}

	if(yGrid < grid_y)
	{
		Dir = 0;
		Step++;
		if (Step >2)
			Step = 1;
		Y += 16;
		return 1;
	}

	if(yGrid > grid_y)
	{
		Dir = 3;
		Step++;
		if (Step >2)
			Step = 1;
		Y -= 16;
		return 1;
	}
	
	Step = 0;
	return 0;
}
