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
	browserPi = { 0 };
	si.cb = sizeof(si);

	// ����ǰ�����̡����̱߳��̳�
	sa_p.nLength = sizeof(sa_p);
	sa_p.lpSecurityDescriptor = nullptr;
	sa_p.bInheritHandle = TRUE;
	sa_t.nLength = sizeof(sa_t);
	sa_t.lpSecurityDescriptor = nullptr;
	sa_t.bInheritHandle = TRUE;

	TCHAR szCmdline[] = TEXT(" ");

	BOOL res = CreateProcess(
		exePath,
		szCmdline,
		&sa_p,
		&sa_t,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		TEXT("C:\\Users\\yui\\source"), &si, &browserPi);

	// ��ȡ�����𴴽��Ľ��̵���ڵ�
	// ��ȡ����Ľ��� ���߳������ģ�ebp��ֵ������ڵ�ַ(ImgBase+EntryPoint)
	// �ý��̵� ebp+8��ַ�洢�� ImaBase(���ܴ���peͷ�� imgbase)��
	CONTEXT threadContext = { 0 };
	threadContext.ContextFlags = CONTEXT_FULL;
	GetThreadContext(browserPi.hThread, &threadContext);
	TCHAR str[0x50] = { 0 };
	DWORD imgbase = 0;
	ReadProcessMemory(browserPi.hProcess, (LPCVOID)(threadContext.Ebx+8),&imgbase,4,0);
	wsprintf(str,TEXT("entryPoint:%x\nimgBase:%x"), threadContext.Eax, imgbase);
	MessageBox(0, str, TEXT("info"), MB_OK);

	// ����Ƿ�ɹ���������
	if (res)
	{
		// ���̴����ɹ����رվ��
		//CloseHandle(browserPi.hProcess);
		//CloseHandle(browserPi.hThread);
	}
}


// 
VOID TestCreateProcessCloneMe()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR szCmdline[0x20] = { 0 };
	wsprintf(szCmdline,TEXT(" %d %d"), (DWORD)browserPi.hProcess,(DWORD)browserPi.hThread);

	wsprintf(cd,TEXT("%s/%s"),cd,TEXT("WinDesktopTest.exe"));

	BOOL res = CreateProcess(
		cd,
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
