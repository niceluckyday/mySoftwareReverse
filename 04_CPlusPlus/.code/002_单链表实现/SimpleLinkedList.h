#ifndef SIMPLELINKEDLIST_H
#define SIMPLELINKEDLIST_H

#include <Windows.h>
#include <cstdarg>

#define SUCCESS           1 // ִ�гɹ�	
#define ERROR            -1 // ִ��ʧ��	
#define INDEX_IS_ERROR   -2 // �����������	
#define BUFFER_IS_EMPTY  -3 // �������ѿ�	



template <class T_ELE>	
class LinkedList	
{	
public:	
	LinkedList();	
	~LinkedList();	
public:	
	BOOL  empty();										//�ж������Ƿ�Ϊ�� �շ���1 �ǿշ���0	
	void  clear();										//�������	
	DWORD at(IN DWORD dwIndex,OUT T_ELE& Element);		//����������ȡԪ��	
	DWORD append(IN T_ELE Element);						//����Ԫ��	
	DWORD append(DWORD argc, IN T_ELE ...);						//����Ԫ��	
	DWORD insert(IN DWORD dwIndex, IN T_ELE Element);	//������������Ԫ��	
	DWORD erase(IN DWORD dwIndex);						//��������ɾ��Ԫ��	
	DWORD size();										//��ȡ������Ԫ�ص�����	

private:	
	typedef struct _NODE 	
	{	
		T_ELE  Data;	
		_NODE *pNext;	
	}NODE,*PNODE;	

	PNODE m_pList;		//����ͷָ�룬ָ���һ���ڵ�	
	DWORD m_dwLength;	//Ԫ�ص�����	

private:
	//PNODE GetIndexCurrentNode(DWORD dwIndex);	//��ȡ����ΪdwIndex��ָ��	
	//PNODE GetIndexPreviousNode(DWORD dwIndex);	//��ȡ����ΪdwIndex��ǰһ���ڵ�ָ��	
	//PNODE GetIndexNextNode(DWORD dwIndex);	//��ȡ����ΪdwIndex�ĺ�һ���ڵ�ָ��	
	
	//��ȡdwIndexǰ��ڵ�ĵ�ַ	
	PNODE GetIndexPreviousNode(DWORD dwIndex)
	{
		// ����һ��ѭ��	
		PNODE pnode = m_pList;
		for (int i = 0; i < dwIndex-1; i++)
		{
			pnode = pnode->pNext;
		}
		return pnode;
	}

	//��ȡdwIndex�ڵ�ĵ�ַ	
	PNODE GetIndexCurrentNode(DWORD dwIndex)
	{
		// ����һ��ѭ��	
		PNODE pnode=m_pList;
		for (int i = 0; i < dwIndex; i++)
		{
			pnode = pnode->pNext;
		}
		return pnode;

	}

	//��ȡdwIndex����ڵ�ĵ�ַ	
	PNODE GetIndexNextNode(DWORD dwIndex)
	{
		// ����һ��ѭ��	
		PNODE pnode = m_pList;
		for (int i = 0; i < dwIndex+1; i++)
		{
			pnode = pnode->pNext;
		}
		return pnode;
	}

};

#include "SimpleLinkedList.cpp"
#endif // SIMPLELINKEDLIST_H
