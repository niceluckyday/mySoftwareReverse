#include <stdio.h>
void __cdecl OutputDebugStringFA(const char* format, ...);
void __cdecl OutputDebugStringFW(const wchar_t* format, ...);


#ifdef _DEBUG  

#ifdef  UNICODE		// unicode  ����
#define DbgPrintf   OutputDebugStringFW
#else				// ���ֽ��ַ� ����
#define DbgPrintf   OutputDebugStringFA
#endif

#else  
#define DbgPrintf  
#endif 

#include "tool.cpp"

