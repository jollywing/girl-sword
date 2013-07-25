
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
void ShutdownAudio();
