
/***************************************************
 * Author: Jolly Wing (jiqingwu@gmail.com)
 * Update: 2013-07-29
 ***************************************************/

#ifndef FIGHTER_H_
#define FIGHTER_H_

#include <SDL.h>
#include <SDL_ttf.h>

class Fighter;

extern Fighter * g_currentEnemy;	//当前的敌人指针
extern Fighter playerFighter;		//1 战斗的阿青

void init_fighters();
Fighter * get_fighter(const char * name);


class Fighter
{
private:
    // about sprite
    short rowIndex;
    short width;
    short height;
    char left;
    SDL_Surface *surface;
public:
    short pX;
    short pY;
    char attacking;

    short id;  // useless?
    char name[32]; // useless?
    short attack;
    short defend;
    short maxHp;	//最大HP
    short hp;	//当前的HP
    char sound[32]; //打斗的声音
    Fighter *next;	//指向Fighter的指针，用于敌人链表, useless now.

public:
    Fighter();
    ~Fighter();
    void init_fighter(short id, short i, const char * name, short hp, 
                      short att, short def, short x, short y, short w, short h, 
                      char l, char a, SDL_Surface *surf, const char * path);
    void draw_self(short n, SDL_Surface *destSurface, int ex, int ey, short enemyAttack);
    short can_defend(short damage);
    short get_damage(short enemyAttack);
    void set_hp( short hp);
    float get_hp_percent();

};


#endif
