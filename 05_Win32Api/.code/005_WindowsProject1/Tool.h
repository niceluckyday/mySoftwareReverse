#include <stdio.h>
void __cdecl OutputDebugStringFA(const char* format, ...);
void __cdecl OutputDebugStringFW(const wchar_t* format, ...);

// ExitWindowsEx
BOOL ExitWin();
// 0:shutdown 1:logout 2:reboot
BOOL ExitWin(BYTE flag);


#ifdef _DEBUG  

#ifdef  UNICODE		// unicode  环境
#define DbgPrintf   OutputDebugStringFW
#else				// 多字节字符 环境
#define DbgPrintf   OutputDebugStringFA
#endif

#else  
#define DbgPrintf  
#endif 

#include "Tool.cpp"

