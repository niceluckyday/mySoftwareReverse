#pragma once

#include "resource1.h"
#include "Tool.h"
#include "GetProcessInfo.h"
#include "LoadPE.h"
#include "LoadPE2.h"
#define MAX_LOADSTRING 100

// �����б�
vector<ProcessInfo>* pInfoList = nullptr;

HINSTANCE hAppInstance = nullptr;
HWND handDlgMain = nullptr;
HWND handDlgPE = nullptr;


// ��ʼ�б�(���ñ�ͷ)
VOID InitProcessList(HWND hDig);
// �б��в��� ����
VOID InsertProcessItem(HWND hDig);

// ��ʼ�б�(���ñ�ͷ)
VOID InitModuleList(HWND hDig);
// �б��в��� ����
VOID InsertModuleItem(HWND hDig);

INT_PTR CALLBACK DialogMainProc(
    HWND handDlg,  // handle to dialog box		
    UINT uMsg,     // message		
    WPARAM wParam, // first message parameter		
    LPARAM lParam  // second message parameter		
);

TCHAR PEPath[MAX_PATH] = { 0 };
#include "DlgPEProc.h"
