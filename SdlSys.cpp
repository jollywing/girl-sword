
#include "SdlSys.h"


//页面
SDL_Surface *screen;
SDL_Surface *hero;
SDL_Surface *XiShi;
SDL_Surface *FanLi;
SDL_Surface *sheep;
SDL_Surface *other_yue;//7 越国杂项页面
SDL_Surface *item;
SDL_Surface *fight;
SDL_Surface *menu;
SDL_Surface *dlg;
SDL_Surface *info;
SDL_Surface *state;
SDL_Surface *map_tile1;
SDL_Surface *map_tile2;
SDL_Surface *map_tile3;

void InitSDL( int win )
{
    Uint32 colorkey;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);

    if (win == 0)
        screen = SDL_SetVideoMode(480, 320, 16, SDL_DOUBLEBUF|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(480, 320, 16, SDL_DOUBLEBUF);

    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    // How to set the icon
    SDL_WM_SetCaption("越女剑Linux版", "sword");

    SDL_ShowCursor(0); //hide mouse pointer

    SDL_EnableKeyRepeat(0, 30); //disable key repeat


	//3 创建菜单页面
    CreateBmpSurface(&menu, "./pic/menu.bmp");

    //create dialog background
    CreateBmpSurface(&dlg, "./pic/dlg.bmp");
    CreateBmpSurface(&info, "./pic/info.bmp");

	//4 创建状态页面
    CreateBmpSurface(&state, "./pic/state.bmp");
    colorkey = SDL_MapRGB(state->format, 0, 0, 0);
    SDL_SetColorKey(state, SDL_SRCCOLORKEY, colorkey);
	
	//5 创建精灵页面
    CreateBmpSurface(&hero, "./pic/aqing.bmp");
    colorkey = SDL_MapRGB(hero->format, 0, 0, 0);
    SDL_SetColorKey(hero, SDL_SRCCOLORKEY, colorkey);

	//6 范蠡
    CreateBmpSurface(&FanLi, "./pic/fanli.bmp");
    colorkey = SDL_MapRGB(FanLi->format, 0, 0, 0);
    SDL_SetColorKey(FanLi, SDL_SRCCOLORKEY, colorkey);

	//7 西施
    CreateBmpSurface(&XiShi, "./pic/xishi.bmp");
    colorkey = SDL_MapRGB(XiShi->format, 0, 0, 0);
    SDL_SetColorKey(XiShi, SDL_SRCCOLORKEY, colorkey);

	//8 创建绵羊页面
    CreateBmpSurface(&sheep, "./pic/sheep.bmp");
    colorkey = SDL_MapRGB(sheep->format, 0, 0, 0);
    SDL_SetColorKey(sheep, SDL_SRCCOLORKEY, colorkey);

	//9 创建越国杂项页面
    CreateBmpSurface(&other_yue, "./pic/other.bmp");
    colorkey = SDL_MapRGB(other_yue->format, 0, 0, 0);
    SDL_SetColorKey(other_yue, SDL_SRCCOLORKEY, colorkey);

	//10 创建物体页面
    CreateBmpSurface(&item, "./pic/item.bmp");
    colorkey = SDL_MapRGB(item->format, 0, 0, 0);
    SDL_SetColorKey(item, SDL_SRCCOLORKEY, colorkey);

	//11 创建战斗页面
    CreateBmpSurface(&fight, "./pic/fight.bmp");
    colorkey = SDL_MapRGB(fight->format, 0, 0, 0);
    SDL_SetColorKey(fight, SDL_SRCCOLORKEY, colorkey);

	//12 创建地图元素页面1
    CreateBmpSurface(&map_tile1, "./pic/maptile.bmp");

	//13 创建地图元素页面2
    CreateBmpSurface(&map_tile2, "./pic/grass.bmp");

	//14 创建地图元素页面2
    CreateBmpSurface(&map_tile3, "./pic/palace.bmp");
}

//释放页面和directdraw对象
void FreeSDL()
{
    SDL_FreeSurface(hero);
    SDL_FreeSurface(XiShi);
    SDL_FreeSurface(FanLi);
    SDL_FreeSurface(sheep);
    SDL_FreeSurface(other_yue);
    SDL_FreeSurface(item);
    SDL_FreeSurface(fight);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(dlg);
    SDL_FreeSurface(info);
    SDL_FreeSurface(state);
    SDL_FreeSurface(map_tile1);
    SDL_FreeSurface(map_tile2);
    SDL_FreeSurface(map_tile3);
}


// fonts
TTF_Font *menu_font;
TTF_Font *about_font;
TTF_Font *message_font;
TTF_Font *dlg_font;

//colors
SDL_Color menu_color;
SDL_Color about_color;
SDL_Color message_color;
SDL_Color dlg_color;

void OpenFonts()
{
    if (-1 == TTF_Init()){
        printf("TTF engine init failed!\n");
        exit(1);
    }

    menu_font = TTF_OpenFont("./font.ttf", 16);
    if (!menu_font){
        printf("Unable open font.ttf!\n");
        exit(1);
    }
    menu_color.r = 220;
    menu_color.g = 220;
    menu_color.b = 255;

    about_font = TTF_OpenFont("./font.ttf", 12);
    if (!about_font){
        printf("Unable open font.ttf!\n");
        exit(1);
    }
    about_color.r = 0;
    about_color.g = 255;
    about_color.b = 255;

    message_font = TTF_OpenFont("./font.ttf", 20);
    if (!message_font){
        printf("Unable open font.ttf!\n");
        exit(1);
    }
    message_color.r = 255;
    message_color.g = 0;
    message_color.b = 0;

    dlg_font = TTF_OpenFont("./font.ttf", 15);
    if (!dlg_font){
        printf("Unable open font.ttf!\n");
        exit(1);
    }
    dlg_color.r = 10;
    dlg_color.g = 50;
    dlg_color.b = 5;
}

void CloseFonts()
{
    TTF_CloseFont(menu_font);
    menu_font = NULL;
    TTF_CloseFont(about_font);
    about_font = NULL;
    TTF_CloseFont(message_font);
    message_font = NULL;
    TTF_CloseFont(dlg_font);
    dlg_font = NULL;

    TTF_Quit();
}

int SDL_BlitText( const char *text, SDL_Surface *dest_surf,
        int x_offset, int y_offset,
        TTF_Font *font, SDL_Color color)
{
    if (text[0] == '\0')
        return 1;

    SDL_Surface *temp = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = temp->w;
    src.h = temp->h;
    dest.x = x_offset;
    dest.y = y_offset;
    dest.w = temp->w;
    dest.h = temp->h;

    SDL_BlitSurface(temp, &src, dest_surf, &dest);
    SDL_FreeSurface(temp);

    return 0;

}

void CreateBmpSurface(SDL_Surface ** surf, const char *bmp_path)
{
    SDL_Surface *temp = SDL_LoadBMP(bmp_path);
    if (temp == NULL){
        printf("Unable to load bitmap.\n");
        exit(1);
    }
    *surf = SDL_DisplayFormat(temp);
    if (*surf == NULL){
        printf("Unable to convert bitmap.\n");
        exit(1);
    }
    SDL_FreeSurface(temp);
}

// #define SND_NUM 12

/* Structure for loaded sounds. */
typedef struct sound_s {
    Uint8 *samples;     /* raw PCM sample data */
    Uint32 length;      /* size of sound data in bytes */
} sound_t, *sound_p;

/* Structure for a currently playing sound. */
typedef struct playing_s {
    int active;         /* 1 if this sound should be played */
    sound_p sound;      /* sound data to play */
    Uint32 position;    /* current position in the sound buffer */
} playing_t, *playing_p;

/* Array for all active sound effects. */
#define MAX_PLAYING_SOUNDS       5
playing_t playing[MAX_PLAYING_SOUNDS];

/* The higher this is, the louder each currently playing sound
   will be. However, high values may cause distortion if too
   many sounds are playing. Experiment with this. */
#define VOLUME_PER_SOUND         SDL_MIX_MAXVOLUME / 2

/* This function is called by SDL whenever the sound card
   needs more samples to play. It might be called from a
   separate thread, so we should be careful what we touch. */
void AudioCallback(void *user_data, Uint8 *audio, int length)
{
    /* Clear the audio buffer so we can mix samples into it. */
    memset(audio, 0, length);

    int i;
    /* Mix in each sound. */
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        if (playing[i].active) {
            Uint8 *sound_buf;
            Uint32 sound_len;
            /* Locate this sound’s current buffer position. */
            sound_buf = playing[i].sound->samples;
            sound_buf += playing[i].position;

            /* Determine the number of samples to mix. */
            if ((playing[i].position + length) >
                playing[i].sound->length) {
                sound_len = playing[i].sound->length -
                    playing[i].position;
            } else {
                sound_len = length;
            }

            /* Mix this sound into the stream. */
            SDL_MixAudio(audio, sound_buf, sound_len,
                         VOLUME_PER_SOUND);

            /* Update the sound buffer’s position. */
            playing[i].position += length;

            /* Have we reached the end of the sound? */
            if (playing[i].position >= playing[i].sound->length) {
                playing[i].active = 0;    /* mark it inactive */
            }
        }
    }
}

/* This function loads a sound with SDL_LoadWAV and converts
   it to the specified sample format. Returns 0 on success
   and 1 on failure. */
int LoadAndConvertSound(char *filename, SDL_AudioSpec *spec,
                        sound_p sound)
{
    SDL_AudioCVT cvt;       /* format conversion structure */
    SDL_AudioSpec loaded;   /* format of the loaded data */
    Uint8 *new_buf;

    /* Load the WAV file in its original sample format. */
    if (SDL_LoadWAV(filename,
                    &loaded, &sound->samples,
                    &sound->length) == NULL) {
        printf("Unable to load sound: %s\n", SDL_GetError());
        return 1;
    }

    /* Build a conversion structure for converting the samples.
       This structure contains the data SDL needs to quickly
       convert between sample formats. */
    if (SDL_BuildAudioCVT(&cvt, loaded.format,
                          loaded.channels, loaded.freq,
                          spec->format, spec->channels,
                          spec->freq) < 0) {
        printf("Unable to convert sound: %s\n", SDL_GetError());
        return 1;
    }

    /* Since converting PCM samples can result in more data
       (for instance, converting 8-bit mono to 16-bit stereo),
       we need to allocate a new buffer for the converted data.
       Fortunately SDL_BuildAudioCVT supplied the necessary
       information. */
    cvt.len = sound->length;
    new_buf = (Uint8 *) malloc(cvt.len * cvt.len_mult);
    if (new_buf == NULL) {
        printf("Memory allocation failed.\n");
        SDL_FreeWAV(sound->samples);
        return 1;
    }

    /* Copy the sound samples into the new buffer. */
    memcpy(new_buf, sound->samples, sound->length);
    /* Perform the conversion on the new buffer. */
    cvt.buf = new_buf;
    if (SDL_ConvertAudio(&cvt) < 0) {
        printf("Audio conversion error: %s\n", SDL_GetError());
        free(new_buf);
        SDL_FreeWAV(sound->samples);
        return 1;
    }

    /* Swap the converted data for the original. */
    SDL_FreeWAV(sound->samples);
    sound->samples = new_buf;
    sound->length = sound->length * cvt.len_mult;

    /* Success! */
    printf("’%s’ was loaded and converted successfully.\n",
           filename);
    return 0;
}

/* Removes all currently playing sounds. */
void ClearPlayingSounds(void)
{
    int i;
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        playing[i].active = 0;
    }
}

/* Adds a sound to the list of currently playing sounds.
   AudioCallback will start mixing this sound into the stream
   the next time it is called (probably in a fraction
   of a second). */
int PlaySound(sound_p sound)
{
    int i;
    /* Find an empty slot for this sound. */
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        if (playing[i].active == 0)
            break;
    }
    /* Report failure if there were no free slots. */
    if (i == MAX_PLAYING_SOUNDS)
        return 1;
    /* The ’playing’ structures are accessed by the audio
       callback, so we should obtain a lock before
       we access them. */
    SDL_LockAudio();
    playing[i].active = 1;
    playing[i].sound = sound;
    playing[i].position = 0;
    SDL_UnlockAudio();
    return 0;
}

SDL_AudioSpec desired, obtained;

char wav_files[][64] = {"voc/Aqing.wav", "voc/Fail.wav", "voc/Jianke.wav",
                        "voc/PushButton.wav", "voc/victory.wav", "voc/Wushi.wav",
                        "voc/ChangeButton.wav", "voc/Feitu.wav", "voc/OpenBox.wav",
                        "voc/Refresh.wav", "voc/Wujianshi.wav", "voc/Yehaizi.wav"};

sound_t wav_sounds[SND_NUM];

void InitAudio()
{
    /* init sdl audio */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL init audio failed: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_CloseAudio);

    /* open audio device */
    desired.freq = 44100; /* desired output sample rate */
    desired.format = AUDIO_S16; /*request signed 16-bit samples*/
    desired.samples = 4096;     /* this is somewhat arbitrary */
    desired.channels = 2;       /* ask for stereo */
    desired.callback = AudioCallback;
    desired.userdata = NULL;        /* we don’t need this */

    if (SDL_OpenAudio(&desired, &obtained) < 0) {
        printf("Unable to open audio device: %s\n",
               SDL_GetError());
        exit(1);
    }

    /* Load our sound files and convert them to
       the sound card’s format. */
    int i;
    for(i = 0; i < SND_NUM; ++ i){
        if (LoadAndConvertSound(wav_files[i], &obtained,
                                &wav_sounds[i]) != 0) {
            printf("Unable to load sound %s!\n", wav_files[i]);
            exit(1);
        }
    }

    /* Clear the list of playing sounds. */
    ClearPlayingSounds();

    /* SDL’s audio is initially paused. Start it. */
    SDL_PauseAudio(0);

}

void PlayWavSound(char sound)
{
    if(sound < 0 || sound >= SND_NUM)
        return;

    PlaySound(&wav_sounds[sound]);
}

//播放声音
void play_sound(const char * path)
{
    int i;
    for (i = 0; i < SND_NUM; i++) {
        if(!strcmp(path, wav_files[i]))
            break;
    }
    PlayWavSound(i);
    //PlaySound(NULL, rpg_app, SND_PURGE);
    //PlaySound(path, rpg_app, SND_ASYNC|SND_NOWAIT);
}


void ShutdownAudio()
{
    /* SDL_CloseAudio(); */
    SDL_PauseAudio(1);
    SDL_LockAudio();

    int i;
    for (i = 0; i < SND_NUM; ++ i) {
        SDL_FreeWAV(wav_sounds[i].samples);
    }

    /* At this point the output is paused and we know for certain
       that the callback is not active, so we can safely unlock
       the audio system. */
    SDL_UnlockAudio();
}
