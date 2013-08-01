
/***************************************************
 * Author: Jolly Wing (jiqingwu@gmail.com)
 * Update: 2013-07-29
 ***************************************************/

#ifndef DIALOG_H_
#define DIALOG_H_

#include <SDL.h>
#include <SDL_ttf.h>

#define DIALOG_CONTENT_SIZE 2048

class Dialog
{
private:
	char  words[DIALOG_CONTENT_SIZE];	//对话内容
	short height;	//高度
	short width;	//宽度
	short x;	//在屏幕中显示的位置
	short y;

    // In fact these variable should be caculated after font is set.
	short charHeight; //字符高度
	short charCountInRow; //一行中的字符数
	short rowCount; // 对话框内显式多少行文本

	short charStartX;	//字符在屏幕中显示的起始位置
	short charStartY;
	int pointer; //计数器

    SDL_Surface *surface;
    TTF_Font *font;
    SDL_Color color;

public:
	Dialog();
	~Dialog();
	short is_over();
	void show(SDL_Surface *destSurf);
	void set_text(const char * content);
	void set_dlg(SDL_Surface *surf, TTF_Font *font, SDL_Color color);

};

extern Dialog g_dialog;

#endif
