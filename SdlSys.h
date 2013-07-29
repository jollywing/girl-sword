#ifndef __SDLSYS_H__
#define __SDLSYS_H__

#include <SDL.h>
#include <SDL_ttf.h>

#define SCR_W 480
#define SCR_H 320

//页面
extern SDL_Surface *screen;
extern SDL_Surface *hero;
extern SDL_Surface *XiShi;
extern SDL_Surface *FanLi;
extern SDL_Surface *sheep;
extern SDL_Surface *other_yue;//7 越国杂项页面
extern SDL_Surface *item;
extern SDL_Surface *fight;
extern SDL_Surface *g_menuSurface;
extern SDL_Surface *dlg;
extern SDL_Surface *info;
extern SDL_Surface *state;
extern SDL_Surface *map_tile1;
extern SDL_Surface *map_tile2;
extern SDL_Surface *map_tile3;

void InitSDL( int windowed );
void FreeSDL();
void CreateBmpSurface(SDL_Surface ** surf, const char *bmp_path);


// fonts
extern TTF_Font *g_menuFont;
extern TTF_Font *about_font;
extern TTF_Font *message_font;
extern TTF_Font *dlg_font;

//colors
extern SDL_Color g_menuColor;
extern SDL_Color about_color;
extern SDL_Color message_color;
extern SDL_Color dlg_color;

void OpenFonts();
void CloseFonts();
int SDL_BlitText(const char *text, SDL_Surface *dest_surf,
        int x_offset, int y_offset,
        TTF_Font *font, SDL_Color color);


#define SND_NUM      12

#define QING_YELL    0
#define QING_FAIL    1
#define JIANKE_YELL  2
#define SELECT       3
#define QING_VICT    4
#define WUSHI_YELL   5
#define CHANGE_SEL   6
#define FEITU_YELL   7
#define OPEN_BOX     8
#define QING_REFRESH    9
#define WU_JIANSHI   10
#define CHILD_YELL   11

extern char wav_files[][64];

void InitAudio();
void PlayWavSound(char sound);
void play_sound(const char *);
void ShutdownAudio();

#endif
