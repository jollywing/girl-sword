
/***************************************************
 * Author: Jolly Wing (jiqingwu@gmail.com)
 * Update: 2013-07-29
 ***************************************************/

#include "Dialog.h"
#include "SdlSys.h"

Dialog g_dialog;	//公用消息框

Dialog::Dialog()
{
	strcpy(words,"");
	pointer = 0;

	height = 60;
	width = 300;
	x = 60;
	y = 260;

	charHeight = 20;
    // show 34 ascii chars or 17 chinese chars in a line
	charCountInRow = 34;
	rowCount = 2;

	charStartX = 20;
	charStartY = 10;
	surface = NULL;
    font = NULL;
    // color = NULL;
}

Dialog::~Dialog()
{
}

short Dialog::is_over()
{
	if(words[pointer] == '\0')
		return 1;
	else
		return 0;
}

void Dialog::show(SDL_Surface *destSurface)
{
 	if(words[pointer] == '\0')
		return;

    SDL_Rect srcRect, destRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;

    SDL_BlitSurface(surface, &srcRect, destSurface, &destRect);

    if (!TTF_WasInit()){
        printf("Truetype font engine has not been initialized!\n");
        exit(1);
    }

	char temp[100];
	short printX = x + charStartX;
	short printY = y + charStartY;

	for(short i = 0; i < rowCount; i++)
	{
        int lineWidth = 0;
		memset(temp, 0, 100);
		for(short j = 0; j < 100; j++)
		{
            // avoid utf-8 chinese char is splited foolishly.
            if (words[pointer] < 0) { // it is first byte of a utf-8 chinese char.
                if (lineWidth + 2 > charCountInRow)
                    break;
                else {
                    temp[j] = words[pointer];
                    temp[j+1] = words[pointer+1];
                    temp[j+2] = words[pointer+2];
                    j += 2;
                    pointer += 3;
                    lineWidth += 2;
                }
            }
			else if (words[pointer] == '\0')
			{
				i = rowCount - 1;
				break;
			}
			else if(words[pointer] == '@')
			{
				i = rowCount - 1;
				pointer++;
				break;
			}
			else
			{
                if(lineWidth >= charCountInRow)
                    break;
                else {
                    temp[j] = words[pointer];
                    pointer++;
                    lineWidth ++;
                }
			}
		}

        SDL_BlitText(temp, destSurface, printX, printY, font, color);
		printY += charHeight;
	}
}

void Dialog::set_text(const char * text)
{
	strcpy(words, text);
    pointer = 0;
}

void Dialog::set_dlg(SDL_Surface *surf, TTF_Font *font,
        SDL_Color color)
{
	surface = surf;
    this->font = font;
    this->color =color;
}


