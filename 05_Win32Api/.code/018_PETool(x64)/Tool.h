#include <stdio.h>

#ifndef  TOOL_H
#define TOOL_H


void __cdecl OutputDebugStringFA(const char* format, ...);
void __cdecl OutputDebugStringFW(const wchar_t* format, ...);

// ExitWindowsEx
BOOL ExitWin();
BOOL CopyToClipboardA(HWND hwnd,LPCSTR lpText);
BOOL CopyToClipboardW(HWND hwnd,LPCWSTR lpText);

#ifdef _DEBUG  

#ifdef  UNICODE		// unicode  ����
#define DbgPrintf			OutputDebugStringFW
#define CopyToClipboard		 CopyToClipboardW

#else				// ���ֽ��ַ� ����
#define DbgPrintf   OutputDebugStringFA
#define CopyToClipboard		 CopyToClipboardA

#endif

#else  //_RELEASE
#define DbgPrintf  
#endif 

#endif // ! TOOL_H

#include "Tool.cpp"

