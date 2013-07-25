#ifndef __MAPH__
#define __MAPH__

#include <SDL.h>
#include <SDL_ttf.h>
#include "Role.h"

#define SCR_W 480
#define SCR_H 320
//BOOL flash;

// Map 1
extern short Tile_aqing[10][15];
extern short Trap_aqing[10][15];
extern short Block_aqing[10][15];

// Map 2
extern short Tile_shaoxing[10][15];
extern short Trap_shaoxing[10][15];
extern short Block_shaoxing[10][15];

// Map 3
extern short Tile_citydoor[10][15];
extern short Trap_citydoor[10][15];
extern short Block_citydoor[10][15];

// Map 4
extern short Tile_fanli[10][15];
extern short Trap_fanli[10][15];
extern short Block_fanli[10][15];

// Map 5
extern short Tile_xiangfang[10][15];
extern short Trap_xiangfang[10][15];
extern short Block_xiangfang[10][15];

// Map 6
extern short Tile_outside[10][15];
extern short Trap_outside[10][15];
extern short Block_outside[10][15];

// Map 7
extern short Tile_wuguo[10][15];
extern short Trap_wuguo[10][15];
extern short Block_wuguo[10][15];

// Map 8
extern short Tile_caoyuan[10][15];
extern short Trap_caoyuan[10][15];
extern short Block_caoyuan[10][15];

// Map 9
extern short Trap_gongdian[10][15];
extern short Block_gongdian[10][15];

class Map;

//地图
extern Map Map_aqing;	//1
extern Map Map_shaoxing;	//2
extern Map Map_citydoor;	//3
extern Map Map_fanli;	//4
extern Map Map_Wuguo;	//5
extern Map Map_caoyuan;	//6
extern Map Map_Gongdian;	//7
extern Map Map_outside;	//8
extern Map Map_Xiangfang;	//9

//地图类声明
class Map
{
public:
	int ID;
	char Name[32];
	short Tile[10][15];
	short Trap[10][15];
	short Block[10][15];
	int count_in_row;
    SDL_Surface * Surf;
	int NpcNum;
	int Npcs[15];
	Role *Npc;
    TTF_Font *Font;
    SDL_Color *Color;

public:
	Map();
	~Map();
	void init_map(int id, const char name[],short tile[10][15],
				 short trap[][15], short block[][15],
                 SDL_Surface *surf, int n,
                 TTF_Font *font, SDL_Color *color);
	void draw_map(SDL_Surface *dest_surf, Role& player,
            SDL_Surface *assist_surf);
	void draw_map_only(SDL_Surface *dest_surf);
	void add_npc(Role *, int x, int y);
	void add_npc_id(int id);
	void del_npc_id(int id);
	void del_npc(Role *);
	void del_all_npc();
	void del_trap(int x, int y);
	void add_trap(int grid_x, int grid_y, int trap_num);
	void del_trap_by_num(short num);
	short if_block(int xCurrent,int yCurrent);
	short check_trap(int xCurrent,int yCurrent);
	short npc_in_map(Role *);
};

#endif
