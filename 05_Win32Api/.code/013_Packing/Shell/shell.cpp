#include "StdAfx.h"

// shell.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

int main(int argc, char* argv[])
{
	//--------------------------------------���ܹ���--------------------------------------
	//��ȡ��ǰ��������·��
	char FilePathSelf[MAX_PATH] = {0};
	GetModuleFileName(NULL, FilePathSelf, MAX_PATH);

	// 1����ȡ��ǰ���ӳ����� ����
	PVOID pFileBufferShell = NULL;
	DWORD dwBufferLengthShell = 0;
	MyReadFile(&pFileBufferShell,&dwBufferLengthShell,FilePathSelf);
	
	// �ж��Ƿ��� ���ܿ���
	if(!IsPackingEXE((LPBYTE)pFileBufferShell))
	{
		return 0;
	}

	// 2������Դ�ļ�,��ȡԴ�ļ���imagebase sizeofimage����
	PVOID pFileBufferSrc = NULL;	
	DWORD dwBufferLengthSrc = 0;
	DWORD dwBufferImageBaseSrc = 0;
	// dwBufferLengthSrc = GetSizeOfImage(pFileBufferShell);
	GetSrcDataFromShell(pFileBufferShell, &pFileBufferSrc, &dwBufferLengthSrc,&dwBufferImageBaseSrc);
	
	// 3������PE  pImageBufferSrc
	PVOID pImageBufferSrc = NULL;
	CopyFileBufferToImageBuffer(pFileBufferSrc,&pImageBufferSrc);

	// 4���Թ���ʽ���пǳ������
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	::CreateProcess(FilePathSelf,NULL,NULL,NULL,NULL,CREATE_SUSPENDED, NULL,NULL,&si,&pi);
	printf("error is %d\n", GetLastError());

	DWORD dwImageBaseShell = GetImageBase(pFileBufferShell); // ��ȡ���ӳ��������imagebase
	
	//5��ж����ǳ�����ļ�����
	typedef long NTSTATUS;
	typedef NTSTATUS(__stdcall *pfnZwUnmapViewOfSection)(unsigned long ProcessHandle, unsigned long BaseAddress);
	
	pfnZwUnmapViewOfSection ZwUnmapViewOfSection = NULL;
	HMODULE hModule = LoadLibrary("ntdll.dll");
	if(hModule){
		ZwUnmapViewOfSection = (pfnZwUnmapViewOfSection)GetProcAddress(hModule, "ZwUnmapViewOfSection");
		if(ZwUnmapViewOfSection){
			if(ZwUnmapViewOfSection((unsigned long)pi.hProcess, dwImageBaseShell)){ // ж�ص� ���ӳ��������ImageBase ��ַ
				printf("ZwUnmapViewOfSection success\n");
			}
		}
		FreeLibrary(hModule);
	}
	
	//6����ָ����λ��(src��ImageBase)����ָ����С(src��SizeOfImage)���ڴ�(VirtualAllocEx)
	LPVOID status = NULL;
	status = VirtualAllocEx(pi.hProcess, (LPVOID)dwBufferImageBaseSrc,dwBufferLengthSrc,MEM_RESERVE | MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	printf("VirtualAllocEx: %x\n",status);
	printf("error is %d\n", GetLastError());


	if(status != NULL){
		printf("7777777\n");
		//7������ɹ�����Src��PE�ļ����� ���Ƶ��ÿռ���
		WriteProcessMemory(pi.hProcess, (LPVOID)dwBufferImageBaseSrc, pImageBufferSrc, dwBufferLengthSrc, NULL);

	}else{
		//8���������ռ�ʧ�ܣ������ض�λ��������λ������ռ䣬Ȼ��PE�ļ����졢���ơ��޸��ض�λ��
		printf("8888888\n");
		PIMAGE_BASE_RELOCATION pRelocationDirectory = NULL;
		DWORD pRelocationDirectoryVirtual = 0;
		
		DWORD NumberOfRelocation;
		PWORD Location;
		DWORD RVA_Data;
		WORD reloData;
		DWORD FOA;
		DWORD dwTempImageBaseSrc = dwBufferImageBaseSrc + 0x50000;
		
		pRelocationDirectoryVirtual = GetRelocationTable(pFileBufferSrc); //��ǰ�ض�λ��������ַ
		printf("%x\n",pRelocationDirectoryVirtual);
		if(pRelocationDirectoryVirtual){
			RVA_TO_FOA(pFileBufferSrc, pRelocationDirectoryVirtual, &FOA);
			pRelocationDirectory = (PIMAGE_BASE_RELOCATION)((DWORD)pFileBufferSrc + FOA);
			//����ռ�
			status = VirtualAllocEx(pi.hProcess, (LPVOID)dwTempImageBaseSrc,dwBufferLengthSrc,MEM_RESERVE | MEM_COMMIT,PAGE_EXECUTE_READWRITE);
			ChangesImageBase(pFileBufferSrc, dwTempImageBaseSrc);
			WriteProcessMemory(pi.hProcess, (LPVOID)dwTempImageBaseSrc, pImageBufferSrc, dwBufferLengthSrc, NULL);
			while(pRelocationDirectory->SizeOfBlock && pRelocationDirectory->VirtualAddress){				
				NumberOfRelocation = (pRelocationDirectory->SizeOfBlock - 8)/2;// ÿ���ض�λ���е������������
				Location = (PWORD)((DWORD)pRelocationDirectory + 8); // ����8���ֽ�
				for(DWORD i=0;i<NumberOfRelocation;i++){
					if(Location[i] >> 12 != 0){ //�ж��Ƿ�����������
						// WORD���͵ı������н���
						reloData = (Location[i] & 0xFFF); //������������ ֻȡ4�ֽ� �����Ƶĺ�12λ
						RVA_Data = pRelocationDirectory->VirtualAddress + reloData; //�����RVA�ĵ�ַ
						RVA_TO_FOA(pFileBufferSrc,RVA_Data,&FOA);
						//������������ �����޸��ض�λ�������Imagebase���Ǹĳ���TempImageBase,��ô�ı��ֵ���� TempImageBase-dwBufferImageBaseSrc
						*(PDWORD)((DWORD)pFileBufferSrc+(DWORD)FOA) = *(PDWORD)((DWORD)pFileBufferSrc+(DWORD)FOA) + dwTempImageBaseSrc - dwBufferImageBaseSrc;	 // ����λ�� - Origin ImageBase			
					}
				}
				pRelocationDirectory = (PIMAGE_BASE_RELOCATION)((DWORD)pRelocationDirectory + (DWORD)pRelocationDirectory->SizeOfBlock); //�����forѭ�����֮����ת���¸��ض�λ�� �������ϵĲ���
			}
			
			dwBufferImageBaseSrc = dwTempImageBaseSrc;
		}else{
			// 9�������6������ռ�ʧ�ܣ����һ�û���ض�λ��ֱ�ӷ��أ�ʧ��.
			printf("999999\n");
			return -1;	
		}
	}


	printf("10000000\n");

	
	// 10���޸���ǳ����Context:
	CONTEXT cont;
	cont.ContextFlags = CONTEXT_FULL; 
	::GetThreadContext(pi.hThread, &cont);

    DWORD dwEntryPoint = GetOep(pFileBufferSrc); // get oep
	cont.Eax = dwEntryPoint + dwBufferImageBaseSrc; // set origin oep

	DWORD theOep = cont.Ebx + 8;
	DWORD dwBytes=0;
	WriteProcessMemory(pi.hProcess, &theOep, &dwBufferImageBaseSrc,4, &dwBytes);

    SetThreadContext(pi.hThread, &cont);
	//�ǵûָ��߳�
    ResumeThread(pi.hThread);
	ExitProcess(0);
	return 0;
}
