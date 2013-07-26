//#include "girl.h"
#include "Menu.h"
#include "SdlSys.h"

//菜单对象
Menu StartMenu[4];	//开始菜单
Menu SystemMenu[4];	//系统菜单
Menu SelectMenu[2];	//选择菜单

//***********************************************************

Menu::~Menu()
{
	Surf = NULL;
}

void Menu::set_menu (const char name[],int x,int y, int w,int h,
					int sel, SDL_Surface *surf,
                    TTF_Font *font, SDL_Color *color)
{
	strcpy(Name,name);
	X = x;
	Y = y;
	W = w;
	H = h;
	Sel = sel;
    Surf = surf;
    Font = font;
    Color = color;
}

void Menu::draw_menu(SDL_Surface *dest_surf)
{
    SDL_Rect menu_src, menu_dest;

    menu_src.x = 0;
    menu_src.w = W;
    menu_src.h = H;
	if(Sel)
        menu_src.y = 25;
	else
        menu_src.y = 0;

    menu_dest.x = X;
    menu_dest.y = Y;
    menu_dest.w = W;
    menu_dest.h = H;

    SDL_BlitSurface(Surf, &menu_src, dest_surf, &menu_dest);

    if (!TTF_WasInit()){
        printf("Truetype font engine has not been initialized!\n");
        exit(1);
    }
    SDL_BlitText(Name, dest_surf, X, Y + 5, Font, *Color);

}
