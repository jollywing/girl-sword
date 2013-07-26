

#include <assert.h>
#include <iostream>
#include "Script.h"
#include "Variable.h"
#include "Map.h"
#include "Girl.h"
#include "Dialog.h"
#include "Fighter.h"

using namespace std;

CScript * g_script;

void RunScripts(char * szName )
{
	//oldFlag = Flag;
	Flag = RUN_SCRIPT_;

	if(g_script->FindScripts(szName))
	{
		g_script->ExecuteScriptLine();
	}
}

void RunScripts()
{
	g_script->ExecuteScriptLine();
}

/* construct object with given file name
   and calculate records number */
CScript::CScript(char * szFileName, char cHeadFlag)
{
	//open given file
	pParserFile.open( szFileName );
	if( !pParserFile )
	{
        cout << "Open script file " << szFileName << " failed!" << endl;
		// MessageBox(NULL, "open script file failed !", "error", MB_OK);
		exit(1);
	}

	//get the number of records
	cRecordHead = cHeadFlag;
	nScriptNumber = 0;
	while(FindRecordHead( ))
	{	
		++ nScriptNumber;		
	}

	// create index table;
	pTable = new stScriptIndex[nScriptNumber];
	assert(pTable);

	// move pointer to file head
	GotoFileHead();

	LoadScripts();
}

CScript::~CScript()
{
	if (pScripts){
		delete [] pScripts;
		pScripts=NULL;
	}
	if (pTable) {
		delete [] pTable;
		pTable =NULL;
	}
	
	// disconnect file and file handle
	//if ( pParserFile.is_open())
	pParserFile.close();
}

/* get record number */
/*
short CScript::GetRecordNum()
{
	return nRecordNum;
}*/

void CScript::LoadScripts()
{
	char szTemp[ 256 ];
	char szName[32];
	short nScriptsIndex = 0;
//	short nValidLineIndex = -1;
	//short nLineNumber = 0;
	//short nValidLineNum = 0;
	nScriptLineNumber = 0;
	
	//calculate line number
	while( GetWholeLine(szTemp))
	{
		// add entry to index table 
		if( szTemp[0] == cRecordHead ){
			nIndexInCurLine = 1;
			ReadSubString( szTemp, szName );
			strcpy( pTable[nScriptsIndex].szScriptName, szName);
			pTable[nScriptsIndex].nIndex = nScriptLineNumber;
			++nScriptsIndex;
		}
		
		++ nScriptLineNumber;		
	}
	
	//malloc memory space to hold scripts
	pScripts = new stScriptLine [ nScriptLineNumber ];
	assert(pScripts);
	
	// read scripts into memory
	GotoFileHead();
	for ( short i = 0; i < nScriptLineNumber; ++ i )
	{
		GetWholeLine(szTemp);
		strcpy( pScripts[ i ].szScriptLine, 
			szTemp );	
	}
}

bool CScript::FindScripts(char * szName)
{
	if ( !pScripts )
	{
		return false;
	}
	
	for (int i=0; i< nScriptNumber; ++i)
	{
		if ( ! strcmp(pTable[i].szScriptName, szName) ){
			nCurrentLine = pTable[i].nIndex + 1;
			return true;
		}

	}
	
	return false;
}

/*
void CScript::RunScripts(char * szName )
{
	if(! FindScripts( szName) )
		return;
	
	while( strcmp( pScripts[ nCurrentLine].szScriptLine, "RETURN") )
	{
		//execute scripte line
		ExecuteScriptLine( pScripts[ nCurrentLine].szScriptLine );
		nCurrentLine ++;
	}
}*/

bool CScript::Expression()
{
	char szLHS[32];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szLHS);
	short nLHS = GetVariableValue(szLHS);

	char szOP[8];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szOP);

	char szRHS[32];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szRHS);
	short nRHS = atoi(szRHS);

	if (!strcmp( szOP, "="))
		return nLHS == nRHS;
	else if ( !strcmp(szOP, ">"))
		return nLHS > nRHS;
	else if ( !strcmp(szOP, "<"))
		return nLHS < nRHS;
	else
		return false;

}

/* 执行一条脚本语句 */
void CScript::ExecuteScriptLine()
{
	nIndexInCurLine = 0;

	// 读取命令
	char szCommand[ 32 ];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szCommand );

	char szStringBuffer[256];
	char szNumberBuffer[4];

	//格式：	RETURN
	if ( ! strcmp( szCommand, "RETURN") )
	{
		Flag = MAIN_MOVE_;
		return;
	}

	//格式：	IF variable value
	//			...
	//			ENDIF
	else if ( ! strcmp( szCommand, "IF")
			|| !strcmp(szCommand, "ELSEIF"))
	{
		
		if ( !Expression() ){	//条件不成立则跳转
			short nested = 0;	//防止if的嵌套
			while(1)
			{
				++ nCurrentLine;
				nIndexInCurLine = 0;
				ReadSubString( pScripts[nCurrentLine].szScriptLine, szCommand);
				if (!strcmp(szCommand, "IF"))
					nested +=1;
				else if (!strcmp(szCommand, "ENDIF") )
				{
					if(nested > 0)
						nested -= 1;
					else break;
				}
				else if( !strcmp(szCommand, "ELSEIF"))
				{
					if (!nested)	break;
				}
			}
			--nCurrentLine;
		}		
	}

	//格式：	GOTO scriptname
	else if(! strcmp(szCommand, "GOTO"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		FindScripts( szStringBuffer );
		--nCurrentLine;
	}

	//格式：	CLS
	else if(! strcmp(szCommand, "CLS"))
	{
		ClrScr();
		// FlipPage();
	}

	//格式：	PLAYSOUND filename
	else if(! strcmp(szCommand, "PLAYSOUND"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		play_sound(szStringBuffer);
	}

	//格式：	DRAWPIC filename width height
	else if(! strcmp(szCommand, "DRAWPIC"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short width = atoi(szNumberBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short height = atoi(szNumberBuffer);
		DrawPic(szStringBuffer/*, width, height*/);
		//FlipPage();
	}

	//格式：	SET variable value
	else if(! strcmp(szCommand, "SET"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		SetVariableValue(szStringBuffer, atoi(szNumberBuffer) );
	}

	//格式：	ADDVAR variable [value]
	else if(! strcmp(szCommand, "ADDVAR"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		if( strcmp(szNumberBuffer, ""))	//如果value不为空
			AddVariable( szStringBuffer, atoi(szNumberBuffer));
		else
			AddVariable(szStringBuffer);
	}

	//格式：	GOTOMAP mapName
	else if(! strcmp(szCommand, "GOTOMAP"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		GotoMap( GetMapAddr(szStringBuffer));
	}

	//格式：	TALK string
	else if(! strcmp(szCommand, "TALK"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		common_diag.set_text(szStringBuffer);
		common_diag.show(screen);
		// FlipPage();
		oldFlag = Flag;
		Flag = GAME_MESSAGE_;		
	}

	//格式：	FIGHT name
	else if(!strcmp( szCommand, "FIGHT"))
	{
		char temp[256];
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		current_enemy = GetFighterAddr(szStringBuffer);
		sprintf(temp, "你与%s开始战斗！", current_enemy->Name );
		common_diag.set_text(temp);
		common_diag.show(screen);
		// FlipPage();
		oldFlag = Flag;	//保存执行脚本的状态
		Flag = FIGHT_START_;		
	}

	//格式：	REMOVENPC name
	else if(!strcmp( szCommand, "REMOVENPC"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		current_map->del_npc( GetRoleAddr(szStringBuffer));
	}

	//格式：	ADDNPC name x, y
	else if( !strcmp(szCommand, "ADDNPC"))
	{
		short x, y;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		x = atoi(szNumberBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		y = atoi(szNumberBuffer);
		current_map->add_npc( GetRoleAddr(szStringBuffer),x,y);
	}

	//格式：	SETDIR name dir
	else if(! strcmp(szCommand, "SETDIR"))
	{
		Role * r;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		r = GetRoleAddr(szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		r->Dir = atoi(szNumberBuffer);		
		RefreshCanvas();
		// FlipPage();
	}

	//格式：	SETSTEP name step
	else if(! strcmp(szCommand, "SETSTEP"))
	{
		Role * r;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		r = GetRoleAddr(szStringBuffer);		
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		r->Step = atoi(szNumberBuffer);
		RefreshCanvas();
		// FlipPage();
	}

	//格式：	SETLOCATION name x y
	else if(! strcmp(szCommand, "SETLOCATION"))
	{
		Role * r;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		r = GetRoleAddr(szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		r->X = atoi(szNumberBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		r->Y = atoi(szNumberBuffer);
		RefreshCanvas();
		// FlipPage();
	}

	//格式：	MOVENPC name grid_x, grid_y
	else if(! strcmp(szCommand, "MOVENPC"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		current_npc = GetRoleAddr(szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		npc_dest_x = atoi(szNumberBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		npc_dest_y = atoi(szNumberBuffer);
		oldFlag = Flag;
		Flag = NPC_MOVE_;
	}

	//格式：	ADDHP name value
	else if(! strcmp(szCommand, "ADDHP"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		Fighter *f = GetFighterAddr( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->HP += value;
	}

	//格式：	ADDATTACK name value
	else if(! strcmp(szCommand, "ADDATTACK"))
	{
		Fighter *f;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		f = GetFighterAddr( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->Attack += value;
	}

	//格式：	ADDDEFEND name value
	else if(! strcmp(szCommand, "ADDDEFEND"))
	{
		Fighter *f;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		f = GetFighterAddr( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->Defend += value;
	}

	//格式：	REFRESH name
	else if(! strcmp(szCommand, "REFRESH"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		Fighter *f = GetFighterAddr( szStringBuffer);
		f->cHP = f->HP;
	}

	//格式：	SELECT words
	else if(! strcmp(szCommand, "SELECT"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		common_diag.set_text(szStringBuffer);
		common_diag.show(screen);
		// FlipPage();
		oldFlag = Flag;
		Flag = BEFORE_SELECT_;
	}

	++ nCurrentLine;
	//格式:		ENDIF等
	/*else
	{
		++nCurrentLine;
	}*/

}

void CScript::ReadSubString(char * stString, char * stSubString )
{
	short nIndex = 0;
	
	//ignore space before valid char
	while( stString[ nIndexInCurLine ] == ' ' || stString [ nIndexInCurLine ] == '\t')
	{
		nIndexInCurLine ++;
	}
	
	//read sub string
	while ( nIndexInCurLine < ( short )strlen(stString) )
	{
		if ( stString[ nIndexInCurLine] == ' ' || 
			stString[ nIndexInCurLine ] == '\t')
			break;
		else
		{
			stSubString[ nIndex ] = stString[ nIndexInCurLine ];
			nIndex ++;
		}	
		nIndexInCurLine ++;
	}
	
	stSubString[ nIndex ] = '\0';
	
}

/* find where a record begin */
bool CScript::FindRecordHead( )
{
	//read char until cRecordHead is readed
	char c;
	while (!pParserFile.eof())
	{
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
		if (c == cRecordHead)
			return true;
	}

	return false;
}

/* get a string from current file */
void CScript::ReadString( char * stString )
{
	//read char, place it into stString, until eof or '/n'
	char c;
	int index = 0;
	pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	
	while ((c == ' ' || c == '\t') && !pParserFile.eof())
	{
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	}
	while ( c != ' ' && c != '\t'  && !pParserFile.eof() )
	{
		stString[index] = c;
		index++;
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));		
	}
	stString[index] = '\0';
}

/* get a integer */
void CScript::ReadInteger( short * nInt )
{
	char c;
	char stInteger[10];
	short nIndex = 0;

	//ignore front ' '
	pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	while ( (c == ' ' || c == '\t') && !pParserFile.eof())
	{
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	}

	//read string until ' ' 
	while ( c != ' ' && c != '\t')
	{
		stInteger[ nIndex ] = c;
		nIndex ++;
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	}

	stInteger[ nIndex ] = '\0';

	*nInt = atoi(stInteger);
}

/* goto next line */
void CScript::GotoNextLine()
{
	char c;
	pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	while( c != '\n')
	{
		pParserFile.read(reinterpret_cast<char *>(&c), sizeof(c));
	}

}

/* get a  line */
bool CScript::GetWholeLine( char * stString )
{
	char c;

	if ( pParserFile.eof() )
		return false;

	// ignore space line
	do {
		pParserFile.read( reinterpret_cast<char *>(&c), sizeof(c));
	} while( c == '\n' && !pParserFile.eof() );

	if ( pParserFile.eof() )
		return false;

	short nIndex = 0;

	// read whole line
	do {
		stString[ nIndex ] = c;
		nIndex ++ ;
		pParserFile.read ( reinterpret_cast < char * >( &c ), sizeof( c ));
	} while( c != '\n' && !pParserFile.eof() );

	stString[ nIndex ] = '\0';

	return true;

}


/* go to file head */
void CScript::GotoFileHead()
{
	pParserFile.clear();
	pParserFile.seekg(0, ios::beg);
}
