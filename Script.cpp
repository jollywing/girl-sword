

#include <assert.h>
#include <iostream>
#include "Script.h"
#include "Variable.h"
#include "Map.h"
#include "Girl.h"
#include "Dialog.h"
#include "Fighter.h"
#include <stack>

using namespace std;

CScript * g_script;

static stack<char> branchStack;

void RunScripts(const char * szName )
{
	if(g_script->FindScripts(szName))
	{
        stateStack.push(RUN_SCRIPT_);
		g_script->ExecuteScriptLine();
	}
}

void RunScripts()
{
	g_script->ExecuteScriptLine();
}

/* construct object with given file name
   and calculate records number */
CScript::CScript(const char * szFileName, char cHeadFlag)
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
	char szTemp[SCRIPT_LINE_LEN];
	char szName[SCRIPT_NAME_LEN];
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

bool CScript::FindScripts(const char * szName)
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
	char szLHS[SCRIPT_VAR_BUF_LEN];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szLHS);
	short nLHS = GetVariableValue(szLHS);

	char szOP[SCRIPT_NUM_BUF_LEN];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szOP);

	char szRHS[SCRIPT_VAR_BUF_LEN];
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

// return 0 when success, return -1 when fail
char CScript::GotoBranchEnd()
{
    short nested = 0;	//防止if的嵌套
    char szCommand[SCRIPT_CMD_LEN];
    while(1)
    {
        ++ nCurrentLine;
        if(nCurrentLine >= nScriptLineNumber){
            while(!branchStack.empty())
                branchStack.pop();
            stateStack.pop(); // exit state of running scripts
            show_dialog("There must be some error with your script!");
            return -1;
        }

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
    }
    --nCurrentLine;
    return 0;
}
  
char CScript::GotoNextBranch()
{
    short nested = 0;	//防止if的嵌套
    char szCommand[SCRIPT_CMD_LEN];
    while(1)
    {
        ++ nCurrentLine;
        if(nCurrentLine >= nScriptLineNumber){
            while(!branchStack.empty())
                branchStack.pop();
            stateStack.pop(); // exit state of running scripts
            show_dialog("There must be some error with your script!");
            return -1;
        }

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
        else if( !strcmp(szCommand, "ELSEIF") ||
                 !strcmp(szCommand, "ELSE")) 
        {
            if (nested == 0)	break;
        }
    }
    --nCurrentLine;
    return 0;
}

/* 执行一条脚本语句 */
void CScript::ExecuteScriptLine()
{
    cout << pScripts[nCurrentLine].szScriptLine << endl;

	nIndexInCurLine = 0;

	// 读取命令
	char szCommand[SCRIPT_CMD_LEN];
	ReadSubString( pScripts[nCurrentLine].szScriptLine, szCommand );

	char szStringBuffer[SCRIPT_LINE_LEN];
	char szNumberBuffer[SCRIPT_NUM_BUF_LEN];

	//格式：	RETURN
	if ( ! strcmp( szCommand, "RETURN") )
	{
        while(!branchStack.empty())
            branchStack.pop();
        stateStack.pop();
		return;
	}

	//格式：	IF variable value
	//			...
	//			ENDIF
	else if ( !strcmp(szCommand, "IF")){
        if (Expression())
            branchStack.push(1);
        else {
            branchStack.push(0);
            GotoNextBranch();
        }
    }

    else if (!strcmp(szCommand, "ELSEIF"))
	{
        if(branchStack.top())
            GotoBranchEnd();
        else if(Expression()){
            branchStack.pop();
            branchStack.push(1);
        }
        else
            GotoNextBranch();
	}

    else if (!strcmp(szCommand, "ELSE"))
	{
        if(branchStack.top())
            GotoBranchEnd();
	}

    else if (!strcmp(szCommand, "ENDIF"))
        branchStack.pop();

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

	//格式：	UPDATESCREEN
	else if(! strcmp(szCommand, "UPDATESCREEN"))
	{
		RefreshCanvas();
		// FlipPage();
	}

    //格式：    WAIT n(ms)
	else if(! strcmp(szCommand, "WAIT"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short time = atoi(szNumberBuffer);
		SDL_Delay(time);
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
		// ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		// short width = atoi(szNumberBuffer);
		// ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		// short height = atoi(szNumberBuffer);
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
        show_dialog(szStringBuffer);
	}

	//格式：	FIGHT name
	else if(!strcmp( szCommand, "FIGHT"))
	{
		char temp[256];
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		g_currentEnemy = get_fighter(szStringBuffer);
		sprintf(temp, "你与%s开始战斗！", g_currentEnemy->name );
        stateStack.push(FIGHTING_);
        InitFight();
        show_dialog(temp);
	}

	//格式：	REMOVENPC name
	else if(!strcmp( szCommand, "REMOVENPC"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
        if(!strcmp(szStringBuffer, ""))
            current_map->del_all_npc();
        else
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

    //格式:      DELTRAP trapnum
    else if(! strcmp(szCommand, "DELTRAP"))
    {
        int trapNum;
        ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
        trapNum = atoi(szNumberBuffer);
        current_map->del_trap_by_num(trapNum);
    }

    //格式：    ADDTRAP trapnum x y
    else if(! strcmp(szCommand, "ADDTRAP"))
    {
        int trapNum, x, y;
        ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
        trapNum = atoi(szNumberBuffer);
        ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
        x = atoi(szNumberBuffer);
        ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
        y = atoi(szNumberBuffer);
        current_map->add_trap(x, y, trapNum);
    }

	//格式:	     SETDIR name dir
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
		// RefreshCanvas();
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
		// RefreshCanvas();
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
        stateStack.push(NPC_MOVE_);
		// oldFlag = Flag;
		// Flag = NPC_MOVE_;
	}

	//格式：	ADDHP name value
	else if(! strcmp(szCommand, "ADDHP"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		Fighter *f = get_fighter( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->maxHp += value;
	}

	//格式：	ADDATTACK name value
	else if(! strcmp(szCommand, "ADDATTACK"))
	{
		Fighter *f;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		f = get_fighter( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->attack += value;
	}

	//格式：	ADDDEFEND name value
	else if(! strcmp(szCommand, "ADDDEFEND"))
	{
		Fighter *f;
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		f = get_fighter( szStringBuffer);
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szNumberBuffer);
		short value = atoi(szNumberBuffer);
		f->defend += value;
	}

	//格式：	REFRESH name
	else if(! strcmp(szCommand, "REFRESH"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
		Fighter *f = get_fighter( szStringBuffer);
		f->hp = f->maxHp;
	}

	//格式：	SELECT words
	else if(! strcmp(szCommand, "SELECT"))
	{
		ReadSubString( pScripts[nCurrentLine].szScriptLine, szStringBuffer);
        g_menuFlag = SELECT_MENU;
        // stateStack.push(WAIT_SELECT_);
        stateStack.push(GAME_MENU_);
        show_dialog(szStringBuffer);
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

    if(nIndexInCurLine >= strlen(stString)){
        strcpy(stSubString, "");
        return;
    }

	//ignore space before valid char
	while( stString[ nIndexInCurLine ] == ' ' || stString [ nIndexInCurLine ] == '\t')
	{
		nIndexInCurLine ++;
	}
	
	//read sub string
	while ( nIndexInCurLine < (short)strlen(stString) )
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
	} while( (c == '\r' || c == '\n') && !pParserFile.eof() );

	if ( pParserFile.eof() )
		return false;

	short nIndex = 0;

	// read whole line
	do {
		stString[ nIndex ] = c;
		nIndex ++ ;
		pParserFile.read ( reinterpret_cast < char * >( &c ), sizeof( c ));
	} while( c != '\r' && c != '\n' && !pParserFile.eof() );

	stString[ nIndex ] = '\0';

	return true;

}


/* go to file head */
void CScript::GotoFileHead()
{
	pParserFile.clear();
	pParserFile.seekg(0, ios::beg);
}
