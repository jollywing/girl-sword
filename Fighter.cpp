#include <stdio.h>
#include "Fighter.h"
#include "SdlSys.h"

//战斗角色
Fighter fAqing;		//1 战斗的阿青
Fighter fWushi1;	//2
Fighter fWushi2;	//3 战斗的两名吴国剑士
Fighter fYehaizi;	//4 战斗的野孩子
Fighter fJianke;	//5 战斗的神秘剑客
Fighter fWushi;		//6 战斗的武师
Fighter fFeitu;		//7 战斗的匪徒
Fighter fShangping; //8 悍匪商平
Fighter fShiwei;	//9 西施的侍卫

stFighterIndex fighters[FIGHTER_NUM];
Fighter * current_enemy;	//当前的敌人指针

void InitFighters()
{
	ClearFighter();

    //初始化战斗角色
	fAqing.init_fighter(1, 0, "阿青",100, 20,15, 250,235, 60,85, 
                        0,1, fight, "voc/Aqing.wav", message_font);	//1
	AddFighter("阿青", &fAqing);

	fWushi1.init_fighter(110, 1, "吴国剑士甲", 90, 20,5, 120,220, 60,85, 
                         1, 0,fight, "voc/Wujianshi.wav", message_font);	//2
	AddFighter("吴国剑士甲", &fWushi1);

	fWushi2.init_fighter(111, 1, "吴国剑士乙", 100, 20,10, 120,240, 70,100,
                         1,0,fight, "voc/Wujianshi.wav", message_font);	//3
	AddFighter("吴国剑士乙", &fWushi2);
	
	fYehaizi.init_fighter(4, 3, "草原上的野孩子", 465,125,100, 100,250,60,85,
                          1,0,fight, "voc/Yehaizi.wav", message_font);	//4
	AddFighter("草原上的野孩子", &fYehaizi);

	fJianke.init_fighter(5, 4,"神秘剑客",1100, 160,150, 120,210, 60,85, 
                         1,0,fight, "voc/Jianke.wav", message_font);	//5
	AddFighter("神秘剑客", &fJianke);

	fWushi.init_fighter(16, 5,"绍兴武师", 120, 30, 18, 140,230, 60,85,
                        1,0,fight, "voc/Wushi.wav", message_font);	//6
	AddFighter("绍兴武师", &fWushi);
	
	fFeitu.init_fighter(112,2, "口痴的匪徒", 200, 42,40, 140,140, 60,85,
                        1,0, fight, "voc/Feitu.wav", message_font);//7
	AddFighter("口痴的匪徒", &fFeitu);
	
	fShangping.init_fighter(113, 2, "悍匪商平", 300, 75,50, 140, 220, 60,85, 
                            1,0, fight, "voc/Feitu.wav", message_font);	//8
	AddFighter("悍匪商平", &fShangping);
	
	fShiwei.init_fighter(114, 6, "西施的侍卫吴吉庆", 1200, 225, 250, 140,255, 60,85,
                         1,0, fight, "voc/Jianke.wav", message_font);
	AddFighter("西施的侍卫吴吉庆", &fShiwei);	
}

void ClearFighter()
{
	for(int i=0; i< FIGHTER_NUM; ++i)
	{
		strcpy( fighters[i].szName, "");
	}
}

bool AddFighter( const char * szName, Fighter *pAddr)
{
	for(int i=0; i< FIGHTER_NUM; ++i)
	{
		if(!strcmp( fighters[i].szName, ""))
		{
			strcpy(fighters[i].szName, szName);
			fighters[i].pFighter = pAddr;
			return true;
		}
	}
	return false;
}

Fighter * GetFighterAddr( char * szName)
{
	for(int i=0; i< FIGHTER_NUM; ++i)
	{
		if( !strcmp(fighters[i].szName, szName))
			return fighters[i].pFighter;
	}
	return NULL;
}

//---------------------------------------------------------

Fighter::Fighter()
{
	ID = 0;
	Index = 0;
	strcpy(Name, "");
	HP = 0;
	cHP = 0;
	Attack = 0;
	Defend = 0;
	X = 0;
	Y = 0;
	Width = 0;
	Height = 0;
	Left = 0;
	bAttack = 0;
	Surf = NULL;
	F = NULL;
}

Fighter::~Fighter()
{
	Surf = NULL;
	F = NULL;
}

void Fighter::set_hp(short hp)
{
	cHP = hp;
	if (cHP>HP) {
		cHP = HP;
	}
}

float Fighter::get_hp_percent()
{
	return ((float)cHP/HP);
}

short Fighter::get_damage(short eAttack)
{
	if(eAttack < Defend ) {
		return 1;
	}
	else {
		return (eAttack - Defend);
	}

}

short Fighter::can_defend(short eAttack)
{
	cHP = cHP - get_damage(eAttack);
	if(cHP > 0) {
		return 1;
	}
	else {
		cHP = 0;
		return 0;
	}
}

void Fighter::init_fighter(short id, short i, const char * name, short hp, 
						  short att, short def, short x, short y, short w, short h, 
						  short l, short a, SDL_Surface *surf, const char * path,
                          TTF_Font *font)
{
	ID = id;
	Index = i;
	strcpy(Name, name);
	HP = hp;
	cHP = hp;
	Attack = att;
	Defend = def;
	X =x;
	Y = y;
	Width = w;
	Height = h;
	Left = l;
	bAttack = a;
	Surf =surf;
    Font = font;
	strcpy( SndPath, path);
	F = NULL;
}

void Fighter::draw_self(short n, SDL_Surface *dest_surf,
        int ex, int ey,short eAttack)
{
    SDL_Rect fighter_src, fighter_dest;
    SDL_Color color;

	char damage[20];


	if (bAttack) {
		switch(n) {
		case 0:
		case 5:
            fighter_src.x = 0;
            fighter_src.y = Index * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

            fighter_dest.x = X;
            fighter_dest.y = Y - Height;
            fighter_dest.w = Width;
            fighter_dest.h = Height;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 1:
            fighter_src.x = 0;
            fighter_src.y = Index * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(Left)
                fighter_dest.x = ex - Width;
			else
                fighter_dest.x = ex + Width;

            fighter_dest.y = Y - Height;
            fighter_dest.w = Width;
            fighter_dest.h = Height;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 2:
            fighter_src.x = 61;
            fighter_src.y = Index * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(Left)
                fighter_dest.x = ex - Width/2;
			else
                fighter_dest.x = ex + Width/2;

            fighter_dest.y = Y - Height;
            fighter_dest.w = Width;
            fighter_dest.h = Height;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 3:
		case 4:
            fighter_src.x = 121;
            fighter_src.y = Index * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(Left)
                fighter_dest.x = ex - Width/2;
			else
                fighter_dest.x = ex + Width/2;

            fighter_dest.y = Y - Height;
            fighter_dest.w = Width;
            fighter_dest.h = Height;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			break;
		}
	}
	else {
        fighter_src.x = 0;
        fighter_src.y = Index*85;
        fighter_src.w = 60;
        fighter_src.h = 85;

        fighter_dest.y = Y - Height;
        fighter_dest.w = Width;
        fighter_dest.h = Height;

		sprintf(damage, "-%d", get_damage(eAttack));

		switch(n) {
		case 0:
		case 1:
		case 2:
		case 3:	
            fighter_dest.x = X;
            fighter_dest.y = Y - Height;
            fighter_dest.w = Width;
            fighter_dest.h = Height;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 4:
			if (Left)
                fighter_dest.x = X - 20;
			else
                fighter_dest.x = X + 20;

            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);

            color.r = 255;
            color.g = 255;
            color.b = 0;
            SDL_BlitText(damage, dest_surf, X+Width/2, Y - Height, Font, color);
			break;
		case 5:
            fighter_dest.x = X;
            SDL_BlitSurface(Surf, &fighter_src, dest_surf, &fighter_dest);
			
            color.r = 255;
            color.g = 255;
            color.b = 150;
            SDL_BlitText(damage, dest_surf, X+Width/2, Y - Height -20, Font, color);
			break;
		}
	}

}
