
#ifndef TEXT_PARSER_
#define TEXT_PARSER_

#include <fstream>
using namespace std;

class CScript;

extern CScript * g_script;
void RunScripts( char * szName );
void RunScripts();

/* a script line struct */
struct stScriptLine
{	
	char szScriptLine[ 256 ];
};

/* a table entry, speed finding script */
struct stScriptIndex
{
	short nIndex;
	char szScriptName[32];
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
	CScript(char * szFileName, char cHeadFlag);
	~CScript();		
//	void RunScripts( char * szName );		// run scripts	
	bool FindScripts( char * szName );	//find scripts whose name is given
	void ExecuteScriptLine( );

private:
	//short GetRecordNum();	
	bool Expression();	//求表达式的值
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