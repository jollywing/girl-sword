#include "Variable.h"
#include <string.h>
#include <iostream>

stVariable vars[VAR_NUMBER];

/*void InitVariables()
{

	ClearVariable();
	AddVariable("������ʩ����");
	AddVariable("�������ܽ���");
	AddVariable("ѯ�ʷ������");
	AddVariable("���ܽ���");
}*/

void ClearVariable()
{
	for( int i=0; i< VAR_NUMBER; ++i)
	{
		strcpy( vars[i].szName, "");
	}
}

short GetVariableValue(char * szName )
{
	for( int i=0; i< VAR_NUMBER; ++i)
	{
		if ( ! strcmp(vars[i].szName, szName))
			return vars[i].nValue;
	}
	return -1;
}

bool SetVariableValue(const char * szName, short nValue)
{
	for( int i=0; i<VAR_NUMBER; ++i)
	{
		if ( ! strcmp(vars[i].szName, szName)){
            std::cout << szName << " = " << nValue << std::endl;
			vars[i].nValue = nValue;
			return true;
		}
	}
	return false;
}

bool IsVariableExist(char * szName )
{
	for( int i=0; i< VAR_NUMBER; ++i)
	{
		if ( ! strcmp(vars[i].szName, szName))
			return true;
	}
	return false;
}

bool AddVariable(char * szName, short nValue /* = 0 */)
{
	for( int i=0; i< VAR_NUMBER; ++i)
	{
		if ( ! strcmp(vars[i].szName, ""))
		{
			strcpy(vars[i].szName, szName);
			vars[i].nValue = nValue;
			return true;
		}			
	}
	return false;
}
