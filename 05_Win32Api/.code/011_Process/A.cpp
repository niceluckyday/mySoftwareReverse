#include <Windows.h>


// ͨ������·������(param1)
VOID TestCreateProcessByAPPName()
{
	// �����ʼ��si,�Ҹ�si��cb��Ա��ֵ ��СΪ sizeof(si)�����������չ
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szApplicationName[] = TEXT("c://program files//internet explorer//iexplore.exe");

	BOOL res = CreateProcess(
		szApplicationName,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

	// ����Ƿ�ɹ���������
	if (res)
	{
		// ���̴����ɹ����رվ��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

}

// ͨ��cmd��������(param2)
VOID TestCreateProcessByCmdline()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[] = TEXT("c://program files//internet explorer//iexplore.exe http://www.ifeng.com");

	BOOL res = CreateProcess(
		NULL,
		szCmdline,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

	// ����Ƿ�ɹ���������
	if (res)
	{
		// ���̴����ɹ����رվ��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

// ͨ��param1,param2 ����
VOID TestCreateProcess()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[] = TEXT(" http://www.ifeng.com");

	BOOL res = CreateProcess(
		TEXT("c://program files//internet explorer//iexplore.exe"),
		szCmdline,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

	// ����Ƿ�ɹ���������
	if (res)
	{
		// ���̴����ɹ����رվ��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}


// 
VOID TestCreateProcessCloneMe()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[0x10] = { 0 };
	wsprintf(szCmdline,TEXT(" %d"), (DWORD)hThread);

	BOOL res = CreateProcess(
		TEXT("C:\\Users\\yui\\source\\repos\\WinDesktopTest\\Debug\\WinDesktopTest.exe"),
		szCmdline,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);

	// ����Ƿ�ɹ���������
	if (res)
	{
		// ���̴����ɹ����رվ��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

DWORD WINAPI ThreadPro(LPVOID lpvoidparam)
{
	while (1)
	{
		MessageBox(0, dlgTitle, TEXT("Tip"), MB_OK);
	}
	return 0;
}
