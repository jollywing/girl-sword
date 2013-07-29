#include "Girl.h"
#include "Menu.h"
#include "SdlSys.h"

//菜单对象
Menu g_startMenus[START_MENU_NUM];	//开始菜单
Menu g_sysMenus[SYS_MENU_NUM];	//系统菜单
Menu g_selectMenus[SELECT_MENU_NUM];	//选择菜单
Menu g_recordMenus[RECORD_MENU_NUM];    //存档菜单

void init_start_menus()
{
	g_startMenus[0].set_menu("新建游戏", (SCR_W/2)-32, SCR_H-105,
                             70, 25, 1, g_menuSurface,
                             g_menuFont, g_menuColor);
    g_startMenus[0].add_listener(new_game);

	g_startMenus[1].set_menu ("读    档", (SCR_W/2)-32, SCR_H-80,
                              70, 25, 0, g_menuSurface,
                              g_menuFont, g_menuColor);
    g_startMenus[1].add_listener(start_load_game);

	g_startMenus[2].set_menu ("作品信息", (SCR_W/2)-32, SCR_H-55,
                              70, 25, 0, g_menuSurface,
                              g_menuFont, g_menuColor);
    g_startMenus[2].add_listener(check_game_info);

	g_startMenus[3].set_menu ("退    出", (SCR_W/2)-32, SCR_H-30,
                              70, 25, 0, g_menuSurface,
                              g_menuFont, g_menuColor);
    g_startMenus[3].add_listener(exit_game);
}

void init_system_menus()
{
	g_sysMenus[0].set_menu("状 态", 10, 10, 50, 25, 1,
                           g_menuSurface, g_menuFont, g_menuColor);
    g_sysMenus[0].add_listener(check_player_info);

	g_sysMenus[1].set_menu("存 档", 10, 35, 50, 25, 0,
                           g_menuSurface, g_menuFont, g_menuColor);
    g_sysMenus[1].add_listener(start_save_game);

	g_sysMenus[2].set_menu("读 档", 10, 60, 50, 25, 0,
                           g_menuSurface, g_menuFont, g_menuColor);
    g_sysMenus[2].add_listener(start_load_game);

	g_sysMenus[3].set_menu("退 出", 10, 85, 50, 25, 0,
                           g_menuSurface, g_menuFont, g_menuColor);
    g_sysMenus[3].add_listener(exit_game);
}

void init_select_menus()
{
	g_selectMenus[0].set_menu("是", 365, 260, 40, 25, 1,
                              g_menuSurface, g_menuFont, g_menuColor);
    g_selectMenus[0].add_listener(select_yes);

	g_selectMenus[1].set_menu("否", 365, 286, 40, 25, 0,
                              g_menuSurface, g_menuFont, g_menuColor);
    g_selectMenus[1].add_listener(select_no);
}

void init_record_menus()
{
    g_recordMenus[0].set_menu("RECORD A", 160, 220, 90, 25, 1,
                              g_menuSurface, g_menuFont, g_menuColor);
    g_recordMenus[0].add_listener(read_or_write_record);
    g_recordMenus[1].set_menu("RECORD B", 160, 245, 90, 25, 0,
                              g_menuSurface, g_menuFont, g_menuColor);
    g_recordMenus[1].add_listener(read_or_write_record);
    g_recordMenus[2].set_menu("RECORD C", 160, 270, 90, 25, 0,
                              g_menuSurface, g_menuFont, g_menuColor);
    g_recordMenus[2].add_listener(read_or_write_record);
}

short get_selected_menu(Menu menuGroup[], short groupSize)
{
    // 无法使用数组做参数，menuGroup会从数组首地址蜕变成一个指针，
    // 所以 sizeof(menuGroup) = 4，无法计算数组元素个数，
    // 无奈，传元素个数作为参数吧。
    // short menuGroupSize;
    // menuGroupSize = sizeof(menuGroup) / sizeof(Menu);

    short i, selected = 0;
    for (i = 0; i < groupSize; i ++) {
        if(menuGroup[i].is_selected()){
            selected = i;
            break;
        }
    }
    return selected;
}

/* in cpp, such as "Menu [] menuGroup" is not allowed. */
void change_selected_menu(Menu menuGroup[], short groupSize, char offset)
{
    short selected;
    selected = get_selected_menu(menuGroup, groupSize);
    menuGroup[selected].set_selected(0);

    selected = (selected + offset) % groupSize;
    if (selected < 0)
        selected = groupSize + selected;
    menuGroup[selected].set_selected(1);
}

void draw_menu_group(Menu menuGroup[], short groupSize, SDL_Surface *surface)
{
    short i;
    for(i = 0; i < groupSize; i++)
        menuGroup[i].draw_menu(surface);
}

//***********************************************************

Menu::~Menu()
{
	surface = NULL;
}

void Menu::set_menu (const char name[],int x,int y, int w,int h,
					char sel, SDL_Surface *surf,
                    TTF_Font *font, SDL_Color color)
{
	strcpy(this->name, name);
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	selected = sel;
    surface = surf;
    this->font = font;
    this->color = color;
}

inline char Menu::is_selected()
{
    return selected;
}

inline void Menu::set_selected(char value)
{
    if (value >= 1)
        selected = 1;
    else
        selected = 0;
}

inline void Menu::add_listener(void (*function)())
{
    on_click = function;
}

inline void Menu::remove_listener()
{
    on_click = NULL;
}

void Menu::draw_menu(SDL_Surface *destSurface)
{
    SDL_Rect srcRect, destRect;

    srcRect.x = 0;
    srcRect.w = w;
    srcRect.h = h;
	if(selected)
        srcRect.y = 25;
	else
        srcRect.y = 0;

    destRect.x = x;
    destRect.y = y;
    destRect.w = w;
    destRect.h = h;

    SDL_BlitSurface(surface, &srcRect, destSurface, &destRect);

    if (!TTF_WasInit()){
        printf("Truetype font engine has not been initialized!\n");
        exit(1);
    }
    SDL_BlitText(name, destSurface, x + 5, y + 5, font, color);

}
