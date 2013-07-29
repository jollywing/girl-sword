#ifndef __MENUH__
#define __MENUH__

#include <SDL.h>
#include <SDL_ttf.h>

#define START_MENU_NUM 4
#define SYS_MENU_NUM 4
#define SELECT_MENU_NUM 2
#define RECORD_MENU_NUM 3

class Menu;
extern Menu g_startMenus[START_MENU_NUM];	//开始菜单
extern Menu g_sysMenus[SYS_MENU_NUM];	//系统菜单
extern Menu g_selectMenus[SELECT_MENU_NUM];	//选择菜单
extern Menu g_recordMenus[RECORD_MENU_NUM];

void init_start_menus();
void init_system_menus();
void init_select_menus();
void init_record_menus();

short get_selected_menu(Menu menuGroup[], short size);
void change_selected_menu(Menu menuGroup[], short size, char offset);
void draw_menu_group(Menu menuGroup[], short groupSize,
                     SDL_Surface *surface);


class Menu
{
public:
	char name[32];
	int x;
	int y;
	int w;
	int h;
	char selected;
    SDL_Surface * surface;
    TTF_Font *font;
    SDL_Color color;
    void (*on_click)();
    
public:
	~Menu();
	void draw_menu(SDL_Surface *destSurface);
	void set_menu(const char name[], int x, int y, int w, int h,
				 char sel, SDL_Surface *surf,
                 TTF_Font *font, SDL_Color color);
    char is_selected();
    void set_selected(char value);
    void add_listener(void (*function)());
    void remove_listener();
};


#endif
