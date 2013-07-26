#ifndef __GIRLH__
#define __GIRLH__

#include "SdlSys.h"
#include "Role.h"
#include "Map.h"
#include "Record.h"
#include "Dialog.h"
#include "Fighter.h"
#include "Menu.h"


//��Ϸ����״̬�ĺ궨��
#define GAME_LOAD_	0	//��Ϸ����װ��
#define GAME_TITLE_	1	//��Ϸ��ʼ����
#define MAIN_MOVE_	2	//����ҿ�������
#define READ_RECORD_	3	//��ȡ��¼
#define GAME_EXIT_	4	//�����˳���Ϸ
#define SYSTEM_MENU_	5	//��Ϸ�����е���ϵͳ�˵�
#define WRITE_RECORD_	6	//��д��¼
#define GAME_MESSAGE_	7	//ϵͳ��Ϣ
#define FIGHT_START_	8	//ս����ʼ
#define FIGHTING_	9	//ս������
#define FIGHT_END_	10	//ս������
#define AUTO_PLAY_	11	//�Զ�����
#define TREAT_NPC_	12	//����NPC�¼�
#define BEFORE_SELECT_	13	//�����ȴ����ѡ��𰸵�ҳ��
#define WAIT_SELECT_	14	//�ȴ��������ѡ��
#define	SELECT_YES_		15	//��������ǵ�ѡ��
#define SELECT_NO_		16	//����������ѡ��
#define CHECK_STATE_	17	//�鿴״̬
#define CHECK_ABOUT_	18	//�鿴��Ʒ��Ϣ
#define RUN_SCRIPT_     19  //���нű�
#define NPC_MOVE_		20	//�ƶ�NPC

extern short Flag, oldFlag;

//**************************��������**********************************

//����Ϸ״̬��Ӧ�ĺ���
void MainLoop();	//0 ��Ϸ��ѭ��
void InitGame();	//1 ��Ϸ��ʼ��
void MainMove();	//2 �����ڵ�ͼ�߶��ĺ���
void StartFight();	//3 ��ʼս��
void Fighting();	//4	ս����
void FightEnd();	//5 ս������
void TreatNpc();	//6 ����Npc�¼�
void GameTitle();	//7
void GameExit();	//8 ��Ϸ�˳�
void System_Menu();	//9 ��Ϸ�����е���ϵͳ�˵�
void GameMessage();	//10 ��Ϸ��������ʾ��Ϣ
void AutoPlay();	//11 �Զ�����
void Load();	//12 װ�ص���
void Store();	//13 �洢����	
void BeforeSelect();	//14 �������ѡ��Ĳ˵�
void WaitSelect();	//15 �ȴ����ѡ��
void SelectYes();	//16 ��������ǵ�ѡ��
void SelectNo();	//17 ����������ѡ��
void CheckState();	//18 �鿴���״̬
void CheckAbout();	//19 �鿴��Ʒ��Ϣ

//������Ϸ���溯��
void DrawState();		//1 �����ǵ�״̬
void DrawStateDetail();	//2 ��ʾ��ϸ״̬��Ϣ
void DrawStartMenu();	//3 ����ʼ�˵�
void DrawSystemMenu();	//4 ��ϵͳ�˵�
void DrawSelectMenu();	//5 ��ѡ��˵�
void DrawRecord();		//6 ����¼
void RefreshCanvas();	//7 ���·�ս������
void UpdateFight();		//8 ����ս������
void DrawRoundNum();	//9 ��ս���غ���
void ClrScr();			//10 ����
void DrawPic(const char *);	//11 ����Ļ����ʾͼƬ
void ShowAbout();		//12 ��ʾ��Ʒ����Ϣ	
// void FlipPage();		//13 ������ҳ������ݴ��͵���ҳ��
void DrawTitle();		//14 ������ҳ���ϻ�Ƭͷ
void DrawEnd();			//15 ��Ƭβ

//��Ϸ��������
void BorderCheck(int *x,int *y);	//1 �߽���
short CrushCheck();					//2 ��ײ���
void SceneChange();					//3 �����л�����
void QueryMessage(short);			//4 ��Ӧ������ض��ص㰴�¿ո��¼�
short FindNpc();					//5 Ѱ�������Ե�Npc,����Npc��ID
void GetMapbyID(int);				//6 ͨ����ͼID���ı�current_map
Role * GetNpcbyID(int);				//7 �õ�npc��ָ��
void RelayoutNpc();					//8 �л���ͼʱ�����¼���NPC
void GotoMap(Map *);				//9 �����ߵ��µĵ�ͼ
//void PressKey(DWORD key);			//10�ȴ������ɿ�
void MoveNpc();

void WaitKeyRelease();

//������Ϸ���ݺ���
void InitData();					//1 ��ʼ������
short LoadData(char *);				//2 ��ȡ����
short StoreData(char *);				//3 �洢����



#endif
