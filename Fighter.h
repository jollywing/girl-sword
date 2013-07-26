#ifndef FIGHTER_H_
#define FIGHTER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class Fighter
{
private:
	short ID;
	short Index;
	short Width;
	short Height;
	short Left;
	SDL_Surface *Surf;
    TTF_Font *Font;
public:
	char Name[32];
	short X;
	short Y;
	short bAttack;
	short Attack;
	short Defend;
	short HP;	//最大HP
	short cHP;	//当前的HP
	char SndPath[32]; //打斗的声音
	Fighter *F;	//指向Fighter的指针，用于敌人链表

public:
	Fighter();
	~Fighter();
	void init_fighter(short id, short i, const char * name, short hp, 
		short att, short def, short x, short y, short w, short h, 
		short l, short a, SDL_Surface *surf, const char * path,
        TTF_Font *font);
	void draw_self(short n, SDL_Surface *dest_surf, int ex, int ey, short eAttack);
	short can_defend(short damage);
	short get_damage(short eAttack);
	void set_hp( short hp);
	float get_hp_percent();

};

#define FIGHTER_NUM 9

extern Fighter * current_enemy;	//当前的敌人指针

//战斗角色
extern Fighter fAqing;		//1 战斗的阿青
extern Fighter fWushi1;	//2
extern Fighter fWushi2;	//3 战斗的两名吴国剑士
extern Fighter fYehaizi;	//4 战斗的野孩子
extern Fighter fJianke;	//5 战斗的神秘剑客
extern Fighter fWushi;		//6 战斗的武师
extern Fighter fFeitu;		//7 战斗的匪徒
extern Fighter fShangping; //8 悍匪商平
extern Fighter fShiwei;	//9 西施的侍卫

struct stFighterIndex
{
	char		szName[32];
	Fighter		*pFighter;
};

extern stFighterIndex fighters[FIGHTER_NUM];

void InitFighters();
void ClearFighter();
bool AddFighter( const char *, Fighter *);
Fighter * GetFighterAddr( char *);


#endif
