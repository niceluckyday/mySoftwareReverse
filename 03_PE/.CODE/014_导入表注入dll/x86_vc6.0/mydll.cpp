#include "mydll.h"

int __stdcall test(int x,int y)
{
	return x+y;
}

// C++ ��д��ʾ��
void __stdcall MessageBox()
{
	ShellExecute(NULL, "open","cmd.exe", NULL, NULL, SW_SHOWNORMAL);
}

// dll ��ں���
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		// LoadLibrary()
		case DLL_PROCESS_ATTACH:
			MessageBox(0,0,"start",0);
			ShellExecuteA(NULL, "open", "cmd.exe", NULL,NULL, SW_SHOWNORMAL);
			//ShellExecuteW(NULL, L"open", L"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe", L"https://hao.360.com", NULL, SW_SHOWNORMAL);

			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
