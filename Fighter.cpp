#include <stdio.h>
#include <stdlib.h>
#include "Fighter.h"
#include "SdlSys.h"

#define FIGHTER_NUM 9

struct stFighterIndex
{
    char		szName[32];
    Fighter		*pFighter;
};


//战斗角色
Fighter playerFighter;		//1 战斗的阿青
Fighter warriorFighter1;	//2
Fighter warriorFighter2;	//3 战斗的两名吴国剑士
Fighter childFighter;	//4 战斗的野孩子
Fighter swordFighter;	//5 战斗的神秘剑客
Fighter teacherFighter;		//6 战斗的武师
Fighter banditFighter;		//7 战斗的匪徒
Fighter chieftainFighter; //8 悍匪商平
Fighter guardFighter;	//9 西施的侍卫

stFighterIndex fighters[FIGHTER_NUM];
Fighter * g_currentEnemy;	//当前的敌人指针

void clear_fighters()
{
	for(int i=0; i< FIGHTER_NUM; ++i)
	{
		strcpy( fighters[i].szName, "");
	}
}

bool add_fighter( const char * szName, Fighter *pAddr)
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

void init_fighters()
{
	clear_fighters();

    //初始化战斗角色
	playerFighter.init_fighter(1, 0, "阿青",100, 20,15, 250,235, 60,85, 
                        0, 1, fight, "voc/Aqing.wav");	//1
	add_fighter("阿青", &playerFighter);

	warriorFighter1.init_fighter(110, 1, "吴国剑士甲", 90, 20,5, 120,220, 60,85, 
                         1, 0,fight, "voc/Wujianshi.wav");	//2
	add_fighter("吴国剑士甲", &warriorFighter1);

	warriorFighter2.init_fighter(111, 1, "吴国剑士乙", 100, 20,10, 120,240, 70,100,
                         1,0,fight, "voc/Wujianshi.wav");	//3
	add_fighter("吴国剑士乙", &warriorFighter2);
	
	childFighter.init_fighter(4, 3, "草原上的野孩子", 465,125,100, 100,250,60,85,
                          1,0,fight, "voc/Yehaizi.wav");	//4
	add_fighter("草原上的野孩子", &childFighter);

	swordFighter.init_fighter(5, 4,"神秘剑客",1100, 160,150, 120,210, 60,85, 
                         1,0,fight, "voc/Jianke.wav");	//5
	add_fighter("神秘剑客", &swordFighter);

	teacherFighter.init_fighter(16, 5,"绍兴武师", 120, 30, 18, 140,230, 60,85,
                        1,0,fight, "voc/Wushi.wav");	//6
	add_fighter("绍兴武师", &teacherFighter);
	
	banditFighter.init_fighter(112,2, "口痴的匪徒", 200, 42,40, 140,140, 60,85,
                        1,0, fight, "voc/Feitu.wav");//7
	add_fighter("口痴的匪徒", &banditFighter);
	
	chieftainFighter.init_fighter(113, 2, "悍匪商平", 300, 75,50, 140, 220, 60,85, 
                            1,0, fight, "voc/Feitu.wav");	//8
	add_fighter("悍匪商平", &chieftainFighter);
	
	guardFighter.init_fighter(114, 6, "西施的侍卫吴吉庆", 1200, 225, 250, 140,255, 60,85,
                         1,0, fight, "voc/Jianke.wav");
	add_fighter("西施的侍卫吴吉庆", &guardFighter);	
}


Fighter * get_fighter(const char * szName)
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
	id = 0;
	rowIndex = 0;
	strcpy(name, "");
	maxHp = 0;
	hp = 0;
	attack = 0;
	defend = 0;
	pX = 0;
	pY = 0;
	width = 0;
	height = 0;
	left = 0;
	attacking = 0;
	surface = NULL;
	next = NULL;
}

Fighter::~Fighter()
{
	// surface = NULL;
	// next = NULL;
}

void Fighter::set_hp(short hp)
{
	this->hp = hp;
	if (hp > maxHp) {
		hp = maxHp;
	}
}

float Fighter::get_hp_percent()
{
	return ((float)hp / maxHp);
}

short Fighter::get_damage(short enemyAttack)
{
	if(enemyAttack < defend ) {
		return rand() % 2;
	}
	else {
		return (enemyAttack - defend + rand() % 5);
	}

}

short Fighter::can_defend(short enemyAttack)
{
	hp = hp - get_damage(enemyAttack);
	if(hp > 0) {
		return 1;
	}
	else {
		hp = 1;
		return 0;
	}
}

void Fighter::init_fighter(short id, short i, const char * name, short hp, 
						  short att, short def, short x, short y, short w, short h, 
						  char l, char a, SDL_Surface *surf, const char * path)
{
	this->id = id;
	rowIndex = i;
	strcpy(this->name, name);
	maxHp = hp;
	this->hp = hp;
	attack = att;
	defend = def;
	pX =x;
	pY = y;
	width = w;
	height = h;
	left = l;
	attacking = a;
	surface =surf;
	strcpy( sound, path);
	next = NULL;
}

void Fighter::draw_self(short n, SDL_Surface *dest_surf,
        int ex, int ey,short enemyAttack)
{
    SDL_Rect fighter_src, fighter_dest;
    SDL_Color color;

	static char damage[20];

	if (attacking) {
		switch(n) {
		case 0:
		case 5:
            fighter_src.x = 0;
            fighter_src.y = rowIndex * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

            fighter_dest.x = pX;
            fighter_dest.y = pY - height;
            fighter_dest.w = width;
            fighter_dest.h = height;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 1:
            fighter_src.x = 0;
            fighter_src.y = rowIndex * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(left)
                fighter_dest.x = ex - width;
			else
                fighter_dest.x = ex + width;

            fighter_dest.y = pY - height;
            fighter_dest.w = width;
            fighter_dest.h = height;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 2:
            fighter_src.x = 61;
            fighter_src.y = rowIndex * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(left)
                fighter_dest.x = ex - width/2;
			else
                fighter_dest.x = ex + width/2;

            fighter_dest.y = pY - height;
            fighter_dest.w = width;
            fighter_dest.h = height;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 3:
		case 4:
            fighter_src.x = 121;
            fighter_src.y = rowIndex * 85;
            fighter_src.w = 60;
            fighter_src.h = 85;

			if(left)
                fighter_dest.x = ex - width/2;
			else
                fighter_dest.x = ex + width/2;

            fighter_dest.y = pY - height;
            fighter_dest.w = width;
            fighter_dest.h = height;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			break;
		}
	}
	else {
        fighter_src.x = 0;
        fighter_src.y = rowIndex*85;
        fighter_src.w = 60;
        fighter_src.h = 85;

        fighter_dest.y = pY - height;
        fighter_dest.w = width;
        fighter_dest.h = height;


		switch(n) {
		case 0:
		case 1:
		case 2:
		case 3:	
            fighter_dest.x = pX;
            fighter_dest.y = pY - height;
            fighter_dest.w = width;
            fighter_dest.h = height;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			break;
		case 4:
			if (left)
                fighter_dest.x = pX - 20;
			else
                fighter_dest.x = pX + 20;

            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);

            color.r = 255;
            color.g = 255;
            color.b = 0;
            sprintf(damage, "-%d", get_damage(enemyAttack));
            SDL_BlitText(damage, dest_surf, pX+width/2, pY - height, message_font, color);
			break;
		case 5:
            fighter_dest.x = pX;
            SDL_BlitSurface(surface, &fighter_src, dest_surf, &fighter_dest);
			
            color.r = 255;
            color.g = 255;
            color.b = 150;
            SDL_BlitText(damage, dest_surf, pX+width/2, pY - height -20, message_font, color);
			break;
		}
	}

}
