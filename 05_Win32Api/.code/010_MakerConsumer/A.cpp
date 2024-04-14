#include <Windows.h>

/*
1.�������߳����������߳� ��Ҫͬ����ʹ���ź���
2.ͬʱ ���Ƕ���Ҫ���� �����������ǻ��⣬ʹ�� һ���ٽ���
3.�ڵȴ���Ϣ��ʱ����Ҫ���� ��ʱʱ�䣬������߳� ���ߵȴ�

*/



DWORD WINAPI ThreadProc1(
    LPVOID lpParameter   // thread data		
)
{

    hThread2 = CreateThread(0, 0, ThreadResAlloc, nullptr, 0, 0);
    hThread3 = CreateThread(0, 0, ThreadConsumer, (LPVOID)0, 0, 0);
    hThread4 = CreateThread(0, 0, ThreadConsumer, (LPVOID)1, 0, 0);
    hThread5 = CreateThread(0, 0, ThreadConsumer, (LPVOID)2, 0, 0);
    hThread6 = CreateThread(0, 0, ThreadConsumer, (LPVOID)3, 0, 0);

    CloseHandle(hThread1);

    return 0;
}


// ������ģ��
DWORD WINAPI ThreadResAlloc(
    LPVOID lpParameter   // thread data		
)
{
    HWND hEdit = GetDlgItem((HWND)hDlg, IDC_EDIT_RES);
    HWND hBufferList[] = { GetDlgItem((HWND)hDlg, IDC_BUFFER1),GetDlgItem((HWND)hDlg, IDC_BUFFER2) };
    DWORD dwWaitResult = 0;
    while (lstrlen(szEditText))
    {
        dwWaitResult = WaitForSingleObject(SemaphoreMaker,2000);
        if (dwWaitResult == WAIT_TIMEOUT) break;    // ��ʱ�˳�

        for (int i = 0; i < 2; i++)
        {
            EnterCriticalSection(&cs);
            if (!tcharBuffer[i][0] && lstrlen(szEditText))
            {
                tcharBuffer[i][0] = szEditText[lstrlen(szEditText) - 1];
                szEditText[lstrlen(szEditText) - 1] = 0;
                SetWindowText(hBufferList[i], &tcharBuffer[i][0]);
                ReleaseSemaphore(SemaphoreConsumer, 1, 0);
            }
            LeaveCriticalSection(&cs);

        }


        SetWindowText(hEdit, szEditText);
        Sleep(1000);

    }

    CloseHandle(hThread2);
    CloseHandle(hThread3);
    CloseHandle(hThread4);
    CloseHandle(hThread5);
    CloseHandle(hThread6);
    return 0;
}


// ������ģ��
DWORD WINAPI ThreadConsumer(
    LPVOID lpParameter   // thread data		
)
{
    HWND hBufferList[] = { GetDlgItem((HWND)hDlg, IDC_BUFFER1),GetDlgItem((HWND)hDlg, IDC_BUFFER2) };
    HWND hConsumer = hConsumerText[(DWORD)lpParameter];
    TCHAR szConsumerText[0x30] = { 0 };
    GetWindowText(hConsumer,szConsumerText,0x30);

    DWORD dwWaitResult = 0;
    while (1)
    {
        Sleep(2000);

        dwWaitResult = WaitForSingleObject(SemaphoreConsumer,2000);    // SemaphoreConsumer ������-1

        if (dwWaitResult == WAIT_TIMEOUT) break;    // ��ʱ�˳�

        for (int i = 0; i < 2; i++)
        {
            EnterCriticalSection(&cs);
            if (tcharBuffer[i][0])
            {
                wsprintf(szConsumerText, TEXT("%s %c"), szConsumerText, tcharBuffer[i][0]);
                tcharBuffer[i][0] = 0;
                SetWindowText(hBufferList[i], &tcharBuffer[i][0]);
                ReleaseSemaphore(SemaphoreMaker, 1, 0);
            }
            LeaveCriticalSection(&cs);
        }
        SetWindowText(hConsumer,szConsumerText);
    }

    return 0;
}
