#include <WINDOWS.H>
#include <STDLIB.H>
#define XORKEY 0x55

DWORD CopyFileBufferToImageBuffer(PVOID pFileBuffer,PVOID* pImageBuffer){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_NT_HEADERS pImageNtHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	DWORD ImageBufferSize = 0;
	DWORD i=0;
	
	// DOSͷ
	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	
	// ��׼PE
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	
	// ��ѡPE
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	
	//�ڱ���
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);
	
	//��ȡImageBufffer���ڴ��С
	ImageBufferSize = pImageOptionalHeader->SizeOfImage;

	//ΪpImageBuffer�����ڴ�ռ�
	*pImageBuffer = (PVOID)malloc(ImageBufferSize);
	
	if (*pImageBuffer == NULL)
	{
		printf("malloc failed");
		return -1;
	}


	
	//����
	memset(*pImageBuffer, 0, ImageBufferSize);
	
	// ����ͷ+�ڱ�
	memcpy(*pImageBuffer, pFileBuffer, pImageOptionalHeader->SizeOfHeaders);
	
	//ѭ�������ڱ�
	for(i=0;i<pImageFileHeader->NumberOfSections;i++){
		memcpy(
			(PVOID)((DWORD)*pImageBuffer + pImageSectionHeaderGroup[i].VirtualAddress), // Ҫ������λ�� ImageBuffer�е�ÿ�������ݵ�ƫ��λ��
			(PVOID)((DWORD)pFileBuffer + pImageSectionHeaderGroup[i].PointerToRawData), // ��������λ���� Filebuffer�е�ÿ�������ݵ�ƫ��λ��
			pImageSectionHeaderGroup[i].SizeOfRawData // �������Ĵ�СΪ ÿ�������ݵ��ļ������С
			);
	}
	
	return 0;
}	


void MyReadFile(PVOID* pFileBuffer,PDWORD BufferLenth, TCHAR* szFilePath){
	FILE* File;
	File = fopen(szFilePath,"rb");
	
	if(File == NULL){
		printf("�ļ������ʧ��");
		return;
	}
	
	//��ȡ�ļ�
	fseek(File,0,SEEK_END);
	*BufferLenth = ftell(File);
	
	//���°�Fileָ��ָ���ļ��Ŀ�ͷ
	fseek(File,0,SEEK_SET);
	
	//�����¿ռ�
	*pFileBuffer = (PVOID)malloc(*BufferLenth);
	
	//�ڴ�����
	memset(*pFileBuffer,0,*BufferLenth);
	
	//��ȡ���ڴ滺����
	fread(*pFileBuffer,*BufferLenth,1,File);// һ�ζ���*bufferlenth���ֽڣ��ظ�1��
	
	//�ر��ļ����
	fclose(File);
}

//FOA_TO_RVA:FOA ת�� RVA							
DWORD FOA_TO_RVA(PVOID FileAddress, DWORD FOA,PDWORD pRVA)
{
	int ret = 0;
	int i;
	
	PIMAGE_DOS_HEADER pDosHeader				= (PIMAGE_DOS_HEADER)(FileAddress);
	PIMAGE_FILE_HEADER pFileHeader				= (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader	= (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup			= (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);
	
	//RVA���ļ�ͷ�� �� SectionAlignment ���� FileAlignment ʱRVA����FOA
	if (FOA < pOptionalHeader->SizeOfHeaders || pOptionalHeader->SectionAlignment == pOptionalHeader->FileAlignment)
	{
		*pRVA = FOA;
		return ret;
	}
	
	//ѭ���ж�FOA�ڽ�����
	for (i=0;i < pFileHeader->NumberOfSections; i++)
	{
		if (FOA >= pSectionGroup[i].PointerToRawData && FOA < pSectionGroup[i].PointerToRawData + pSectionGroup[i].SizeOfRawData)
		{
			*pRVA = FOA - pSectionGroup[i].PointerToRawData + pSectionGroup[i].VirtualAddress;
			return *pRVA;
		}
	}
	
	//û���ҵ���ַ
	ret = -4;
	printf("func FOA_TO_RVA() Error: %d ��ַת��ʧ�ܣ�\n", ret);
	return ret;
}

//���ܣ�RVA ת�� FOA
// RVA_TO_FOA(pFileBuffer,pOptionHeader->DataDirectory[5].VirtualAddress,&FOA);
DWORD RVA_TO_FOA(PVOID FileAddress, DWORD RVA, PDWORD pFOA)
{
	int ret = 0;
	int i=0;
	PIMAGE_DOS_HEADER pDosHeader				= (PIMAGE_DOS_HEADER)(FileAddress);
	PIMAGE_FILE_HEADER pFileHeader				= (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader	= (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup			= (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);
	
	
	//RVA���ļ�ͷ�� �� SectionAlignment(�ڴ����) ���� FileAlignment(�ļ�����) ʱ RVA����FOA
	if (RVA < pOptionalHeader->SizeOfHeaders || pOptionalHeader->SectionAlignment == pOptionalHeader->FileAlignment)
	{
		// 37000
		*pFOA = RVA;
		return ret;
	}
	
	/*
		��һ����ָ����.VirtualAddress <= RVA <= ָ����.VirtualAddress + Misc.VirtualSize(��ǰ���ڴ�ʵ�ʴ�С)
		�ڶ�������ֵ = RVA - ָ����.VirtualAddress
		��������FOA = ָ����.PointerToRawData + ��ֵ
	*/

	//ѭ���ж�RVA�ڽ�����
	for (i=0;i<pFileHeader->NumberOfSections; i++)
	{
		// RVA > ��ǰ�����ڴ��е�ƫ�Ƶ�ַ ���� RVA < ��ǰ�ڵ��ڴ�ƫ�Ƶ�ַ+�ļ�ƫ�Ƶ�ַ
		if (RVA >= pSectionGroup[i].VirtualAddress && RVA < pSectionGroup[i].VirtualAddress + pSectionGroup[i].Misc.VirtualSize)
		{
			*pFOA =  RVA - pSectionGroup[i].VirtualAddress + pSectionGroup[i].PointerToRawData;
			return ret;
		}
	}
	
	//û���ҵ���ַ
	ret = -4;
	printf("func RVA_TO_FOA() Error: %d ��ַת��ʧ�ܣ�\n", ret);
	return ret;
}

DWORD GetSizeOfImage(PVOID pFileBuffer){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	PIMAGE_SECTION_HEADER NewSec = NULL;

	DWORD NewLength=0;
	PVOID LastSection = NULL;
	PVOID CodeSection = NULL;
	PVOID AddressOfSectionTable = NULL;
	
	
	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);

	return pImageOptionalHeader->SizeOfImage;
}


DWORD GetImageBase(PVOID pFileBuffer){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	PIMAGE_SECTION_HEADER NewSec = NULL;
	
	DWORD NewLength=0;
	PVOID LastSection = NULL;
	PVOID CodeSection = NULL;
	PVOID AddressOfSectionTable = NULL;
	
	
	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);
	
	return pImageOptionalHeader->ImageBase;
}

DWORD GetRelocationTable(PVOID pFileBuffer){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	PIMAGE_SECTION_HEADER NewSec = NULL;
	DWORD res = 0;

	DWORD NewLength=0;
	PVOID LastSection = NULL;
	PVOID CodeSection = NULL;
	PVOID AddressOfSectionTable = NULL;
	DWORD FOA = 0;
	
	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);

	return pImageOptionalHeader->DataDirectory[5].VirtualAddress;
}


DWORD GetOep(PVOID pFileBuffer){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	PIMAGE_SECTION_HEADER NewSec = NULL;
	PIMAGE_BASE_RELOCATION pRelocationDirectory = NULL;

	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);

	return pImageOptionalHeader->AddressOfEntryPoint;
}

void ChangesImageBase(PVOID pFileBuffer, DWORD TempImageBase){
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHeaderGroup = NULL;
	PIMAGE_SECTION_HEADER NewSec = NULL;
	
	pImageDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + 4);
	pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	pImageSectionHeaderGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);

	pImageOptionalHeader->ImageBase = TempImageBase;
}

void XorDecodeAAA(char* p_data,DWORD DecodeSize)
{
    for(DWORD i = 0; i < DecodeSize; i++)
    {
		p_data[i] = p_data[i] ^ XORKEY;
    }	
}

void GetSrcDataFromShell(PVOID pFileBufferShell, PVOID* FileBufferSrc, PDWORD FileBufferLength, PDWORD FileBufferImageBase){
	PIMAGE_DOS_HEADER pDosHeader = NULL;    
    PIMAGE_NT_HEADERS pNTHeader = NULL; 
    PIMAGE_FILE_HEADER pPEHeader = NULL;    
    PIMAGE_OPTIONAL_HEADER32 pOptionHeader = NULL;  
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
	
	
	pDosHeader = (PIMAGE_DOS_HEADER)pFileBufferShell;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFileBufferShell+pDosHeader->e_lfanew);
    pPEHeader = (PIMAGE_FILE_HEADER)(((DWORD)pNTHeader) + 4);  
    pOptionHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pPEHeader+IMAGE_SIZEOF_FILE_HEADER); 
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionHeader + IMAGE_SIZEOF_NT_OPTIONAL_HEADER);


	// (1) ��Ϊ��SHELL�ļ������һ����	
	*FileBufferSrc = (PVOID)((DWORD)pFileBufferShell + ((PIMAGE_SECTION_HEADER)&pSectionHeader[pPEHeader->NumberOfSections-1])->PointerToRawData);
	XorDecodeAAA((char*)(*FileBufferSrc),((PIMAGE_SECTION_HEADER)&pSectionHeader[pPEHeader->NumberOfSections-1])->SizeOfRawData);
	pDosHeader = (PIMAGE_DOS_HEADER)*FileBufferSrc;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)*FileBufferSrc + pDosHeader->e_lfanew);
    pPEHeader = (PIMAGE_FILE_HEADER)(((DWORD)pNTHeader) + 4);  
    pOptionHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pPEHeader+IMAGE_SIZEOF_FILE_HEADER); 
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionHeader + IMAGE_SIZEOF_NT_OPTIONAL_HEADER);

	// get SizeOfImage
	*FileBufferLength = pOptionHeader->SizeOfImage;
	
	// get ImageBase
	*FileBufferImageBase = pOptionHeader->ImageBase;
}


// ���ܿ���
PIMAGE_DOS_HEADER GetDosHeadersPtr(LPBYTE buffer)
{
	PIMAGE_DOS_HEADER p=(PIMAGE_DOS_HEADER)buffer;
	if(p)
	{
		return PIMAGE_DOS_HEADER(p);
	}else{
		return NULL;
	}
}
PIMAGE_FILE_HEADER GetFileHeadersPtr(LPBYTE buffer)
{

	PIMAGE_FILE_HEADER p=( PIMAGE_FILE_HEADER)&(( PIMAGE_NT_HEADERS)&buffer[(( PIMAGE_DOS_HEADER)buffer)->e_lfanew])->FileHeader;
	if(p)
	{
		return PIMAGE_FILE_HEADER(p);
	}else{
		return NULL;
	}
}

PIMAGE_SECTION_HEADER GetSectionHeadersPtr(LPBYTE buffer,int index)
{
	int section_num=GetFileHeadersPtr(buffer)->NumberOfSections;	
	int section_size=40;
	if(index<section_num)
	{
		return ( PIMAGE_SECTION_HEADER)&buffer[GetDosHeadersPtr(buffer)->e_lfanew+(4+20+GetFileHeadersPtr(buffer)->SizeOfOptionalHeader)+index*section_size];
	}else{
		return NULL;
	}
}
BOOL IsPackingEXE(LPBYTE hInstance)
{
    DWORD sectionNum = GetFileHeadersPtr((LPBYTE)hInstance)->NumberOfSections-1;
    PIMAGE_SECTION_HEADER lastSection = GetSectionHeadersPtr((LPBYTE)hInstance, sectionNum);
    if (strcmp((CHAR*)lastSection[0].Name, ".AAA") == 0)
    {
        return TRUE;
    }
    return FALSE;
}