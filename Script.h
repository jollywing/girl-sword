
#ifndef TEXT_PARSER_
#define TEXT_PARSER_

#include <fstream>
using namespace std;

#define SCRIPT_NAME_LEN 36
#define SCRIPT_LINE_LEN 512
#define SCRIPT_NUM_BUF_LEN 9
#define SCRIPT_VAR_BUF_LEN 32
#define SCRIPT_CMD_LEN 64

class CScript;

extern CScript * g_script;
void RunScripts(const char * szName);
void RunScripts();

/* a script line struct */
struct stScriptLine
{	
	char szScriptLine[SCRIPT_LINE_LEN];
};

/* a table entry, speed finding script */
struct stScriptIndex
{
	short nIndex;
	char szScriptName[SCRIPT_NAME_LEN];
};


class CScript
{
private:
	ifstream	pParserFile;		// handle of parsered file
	short		nScriptNumber;		//record number
	short		nScriptLineNumber;	//script lines' number
	short		nIndexInCurLine;
	short		nCurrentLine;
	char		cRecordHead;		//record head char
	stScriptLine	*pScripts;
	stScriptIndex	*pTable;

public:
	CScript(const char * szFileName, char cHeadFlag);
	~CScript();		
//	void RunScripts( char * szName );		// run scripts	
	bool FindScripts( const char * szName );	//find scripts whose name is given
	void ExecuteScriptLine( );

private:
	//short GetRecordNum();	
	bool Expression();	//求表达式的值
    char GotoNextBranch();  // find next branch in IF .. ELSEIF .. ELSE .. ENDIF
    char GotoBranchEnd();   // find the matching ENDIF
    
	void LoadScripts( );	// load scripts	
	bool FindRecordHead(); //find where record begin
	void ReadString ( char * szString ); //read a string
	void ReadInteger ( short * nInt);	// read a integer
	void ReadSubString( char * szString, char * szSubString );	
	void GotoNextLine();	//goto next line
    
	bool GetWholeLine( char * szString );	//get a line
	void GotoFileHead ();		//goto file head
};

#endif
