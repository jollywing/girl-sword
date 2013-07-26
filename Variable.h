#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#define VAR_NUMBER 20

/* variable */
struct stVariable
{
	char szName[32];
	short nValue;
};

extern stVariable vars[VAR_NUMBER];

//初始化标志变量
//void InitVariables();

//clear variables
void ClearVariable();

// get variable's value
short GetVariableValue( char * szName );

//set variable's value
bool SetVariableValue( char * szName, short nValue);

// whether variable is exist
bool IsVariableExist( char * szName );

// add variable
bool AddVariable( char * szName, short nValue = 0);

#endif