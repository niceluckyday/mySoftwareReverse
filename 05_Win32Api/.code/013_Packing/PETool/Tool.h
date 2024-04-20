#include <stdio.h>

#ifndef  TOOL_H
#define TOOL_H


void __cdecl OutputDebugStringFA(const char* format, ...);
void __cdecl OutputDebugStringFW(const wchar_t* format, ...);

// ExitWindowsEx
BOOL ExitWin();


#ifdef _DEBUG  

#ifdef  UNICODE		// unicode  ����
#define DbgPrintf   OutputDebugStringFW
#else				// ���ֽ��ַ� ����
#define DbgPrintf   OutputDebugStringFA
#endif

#else  
#define DbgPrintf  
#endif 

#endif // ! TOOL_H

#include "Tool.cpp"

