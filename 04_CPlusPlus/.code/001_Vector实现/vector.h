#ifndef VECTOR_H
#define VECTOR_H

//#include <iostream>
#include <Windows.h>  // Ϊ��ʹ�� DWORD ����
#include <string.h>
#include <cstdarg>

#define SUCCESS           	 1 // �ɹ�								
#define ERROR            	-1 // ʧ��								
#define MALLOC_ERROR		-2 // �����ڴ�ʧ��								
#define INDEX_ERROR		 	-3 // �����������								

using namespace std;


template <class T_ELE>
class Vector
{
public:
	Vector();
	Vector(DWORD dwSize);
	~Vector();
public:
	T_ELE& operator[](DWORD dwIndex);

	DWORD	at(DWORD dwIndex, OUT T_ELE* pEle);		//���ݸ����������õ�Ԫ��				
	DWORD   append(T_ELE Element);					//��Ԫ�ش洢���������һ��λ��	
	DWORD	append(DWORD argc, T_ELE...);
	VOID	pop();									//ɾ�����һ��Ԫ��				
	VOID	pop(DWORD dwLen);						//ɾ�����һ��Ԫ��				
	DWORD	insert(DWORD dwIndex, T_ELE Element);	//��ָ��λ������һ��Ԫ��				
	DWORD	capacity();								//�����ڲ����ݵ�����£����ܴ洢����Ԫ��				
	VOID	clear();								//�������Ԫ��				
	BOOL	empty();								//�ж�Vector�Ƿ�Ϊ�� ����trueʱΪ��				
	VOID	erase(DWORD dwIndex);					//ɾ��ָ��Ԫ��				
	DWORD	size();									//����VectorԪ�������Ĵ�С				
private:
	BOOL	expand();
private:
	DWORD  m_dwIndex;							//��һ����������				
	DWORD  m_dwIncrement;						//ÿ�����ݵĴ�С				
	DWORD  m_dwLen;								//��ǰ�����ĳ���				
	DWORD  m_dwInitSize;						//Ĭ�ϳ�ʼ����С				
	T_ELE* m_pVector;							//����ָ��				
};
#include "vector.cpp"
#endif  // VECTOR_H