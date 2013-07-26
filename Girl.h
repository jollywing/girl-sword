#ifndef __GIRLH__
#define __GIRLH__

#include "Role.h"
#include "Map.h"
#include "Record.h"
#include "Dialog.h"
#include "Fighter.h"
#include "Menu.h"







//ȫ����Ϸ����
int current_npc_id;	//��ǰ��Npc��id
Map * current_map;	//��ǰ�ĵ�ͼָ��
Fighter * current_enemy;	//��ǰ�ĵ���ָ��
short TrapNum; //��Ҳ��е�������
long old_time, new_time; //���ڼ���֡ʱ��ı���
short frame_fight;	//ս��ʱ��֡����
int fight_frame_num;	//ս������֡��
short round_num;	//�غ���
short Flag;//��Ϸ����״̬��־


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
void FlipPage();		//13 ������ҳ������ݴ��͵���ҳ��
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
void WaitKeyRelease();

//������Ϸ���ݺ���
void InitData();					//1 ��ʼ������
short LoadData(char *);				//2 ��ȡ����
short StoreData(char *);				//3 �洢����



#endif
