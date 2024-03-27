#ifndef SIMPLELINKEDLIST_CPP
#define SIMPLELINKEDLIST_CPP

#include "SimpleLinkedList.h"


//�޲ι��캯�� ��ʼ����Ա	
template<class T_ELE> 
LinkedList<T_ELE>::LinkedList()	
:m_pList(nullptr),m_dwLength(0)	
{	

}	
//�������� ���Ԫ��	
template<class T_ELE> 
LinkedList<T_ELE>::~LinkedList()	
{	
	// 1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		return;
	}
	// 2. ѭ��ɾ�������еĽڵ㣬��ǰ����ɾ
	int i = 0;
	PNODE pnode = nullptr;
	for (i = 0; i < m_dwLength; i++)
	{
		pnode = m_pList->pNext;
		delete m_pList;
		m_pList = pnode;
	}
	// 3. ɾ�����һ���ڵ㲢����������Ϊ0	
	m_dwLength -= i;
	return;
}	
//�ж������Ƿ�Ϊ��	
template<class T_ELE> 
BOOL LinkedList<T_ELE>::empty()	
{	
	return m_dwLength == 0 ? true : false;

}	

//�������	
template<class T_ELE> 
void LinkedList<T_ELE>::clear()	
{	
	// 1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		return ;
	}
	// 2. ѭ��ɾ�������еĽڵ㣬��ǰ����ɾ
	int i = 0;
	PNODE pnode = nullptr;
	for (i = 0; i < m_dwLength; i++)
	{
		pnode = m_pList->pNext;
		delete m_pList;
		m_pList = pnode;
	}
	// 3. ɾ�����һ���ڵ㲢����������Ϊ0	
	m_dwLength -= i;
	return ;
	
}	

//����������ȡԪ��	
template<class T_ELE> 
DWORD LinkedList<T_ELE>::at(IN DWORD dwIndex,OUT T_ELE& Element)	
{	
	// 1. �ж������Ƿ���Ч	
	if (dwIndex<0 || dwIndex>=m_dwLength)
	{
		std::cout << "Error: Index error." << endl;
		return INDEX_IS_ERROR;
	}
	
	// 2. ȡ������ָ��Ľڵ�	
	PNODE pnode = GetIndexCurrentNode(dwIndex);
	
	// 3. ������ָ��ڵ��ֵ���Ƶ�OUT����	
	memcpy(&Element, &pnode->Data, sizeof T_ELE);
	return SUCCESS;
	
}	

//������β�������ڵ�	
template<class T_ELE> 
DWORD LinkedList<T_ELE>::append(IN T_ELE Element)	
{	
	// 1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		m_pList = new NODE;
		memcpy(&m_pList->Data, &Element, sizeof T_ELE);
		m_pList->pNext = nullptr;
		m_dwLength++;
		return SUCCESS;
	}
	// 2. ����������Ѿ���Ԫ��	
	PNODE pnode=GetIndexCurrentNode(m_dwLength-1);
	pnode->pNext = new NODE;
	pnode=pnode->pNext;

	memcpy(&pnode->Data, &Element, sizeof T_ELE);
	pnode->pNext = nullptr;
	m_dwLength++;
	return SUCCESS;
}

template<class T_ELE>
DWORD LinkedList<T_ELE>::append(DWORD argc, IN T_ELE ...)
{
	va_list va_argvs;
	va_start(va_argvs, argc);
	T_ELE va_argv;
	PNODE pnode = nullptr;

	// 1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		m_pList = new NODE;
		pnode = m_pList;
		for (int i = 0; i < argc; i++)
		{
			va_argv = va_arg(va_argvs, T_ELE);
			memcpy(&pnode->Data, &va_argv, sizeof T_ELE);
			if (i == (argc - 1))
			{
				pnode->pNext = nullptr;
				m_dwLength++;
				break;
			}
			pnode->pNext = new NODE;
			pnode = pnode->pNext;
			m_dwLength++;
		}
		return SUCCESS;
	}

	// 2. ����������Ѿ���Ԫ��	
	pnode = GetIndexCurrentNode(m_dwLength - 1);
	for (int i = 0; i < argc; i++)
	{
		va_argv = va_arg(va_argvs, T_ELE);

		pnode->pNext = new NODE;
		pnode = pnode->pNext;
		memcpy(&pnode->Data, &va_argv, sizeof T_ELE);
		if (i == (argc - 1))
		{
			pnode->pNext = nullptr;	
			m_dwLength++;
			break;
		}
		m_dwLength++;
	}
	return SUCCESS;

}

//���ڵ�������ָ��������λ��	0 1 2 3 4	
template<class T_ELE> 
DWORD LinkedList<T_ELE>::insert(IN DWORD dwIndex, IN T_ELE Element)	
{	
	//  1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		m_pList = new NODE;
		memcpy(&m_pList->Data, &Element, sizeof T_ELE);
		m_pList->pNext = nullptr;
		m_dwLength++;
		return SUCCESS;
	}

	//  2. �ж�����ֵ�Ƿ���Ч	
	if (dwIndex < 0 || dwIndex > m_dwLength)
	{
		std::cout << "Error: Index error." << endl;
		return INDEX_IS_ERROR;
	}
	
	//  3. �������Ϊ0	
	//  4. �������Ϊ����β	
	//  5. �������Ϊ������	
	if (dwIndex==0)
	{
		PNODE pnode = new NODE;
		pnode->pNext = m_pList;
		memcpy(&pnode->Data,&Element,sizeof T_ELE);
		m_pList = pnode;
		m_dwLength++;
		return SUCCESS;
	}
	else if (dwIndex == m_dwLength)
	{
		PNODE pnode = new NODE;
		pnode->pNext = nullptr;
		memcpy(&pnode->Data, &Element, sizeof T_ELE);

		PNODE pnode_ = GetIndexCurrentNode(m_dwLength-1);
		pnode_->pNext = pnode;
		m_dwLength++;
		return SUCCESS;
	}
	else {
		PNODE pnode = new NODE;
		memcpy(&pnode->Data, &Element, sizeof T_ELE);

		PNODE _pnode = GetIndexPreviousNode(dwIndex);
		// _pnode->pNext = pnode; �ò������ж� ��������
		pnode->pNext = _pnode->pNext;

		 _pnode->pNext = pnode;
		m_dwLength++;
		return SUCCESS;
	}
}	

//��������ɾ���ڵ�	
template<class T_ELE> 
DWORD LinkedList<T_ELE>::erase(IN DWORD dwIndex)	
{	
	
	//  1. �ж������Ƿ�Ϊ��	
	if (m_dwLength == 0)
	{
		std::cout << "Error: Empty LinkedList." << endl;
		return ERROR;
	}
	
	//  2. �ж�����ֵ�Ƿ���Ч	
	if (dwIndex < 0 || dwIndex >= m_dwLength)
	{
		std::cout << "Error: Index error." << endl;
		return INDEX_IS_ERROR;
	}

	//  3. ���������ֻ��ͷ�ڵ㣬��Ҫɾ��ͷ�ڵ�	
	//  4. ���Ҫɾ��ͷ�ڵ�	
	//  5. ������������	
	if (m_dwLength == 1)
	{
		delete m_pList;
		m_pList = nullptr;
		m_dwLength--;
		return SUCCESS;
	}
	else if (dwIndex==0) {
		PNODE pnode = GetIndexCurrentNode(1);
		delete m_pList;
		m_pList = pnode;
		m_dwLength--;
		return SUCCESS;
	}
	else {
		PNODE _pnode = GetIndexPreviousNode(dwIndex);
		PNODE pnode = _pnode->pNext;
		_pnode->pNext = pnode->pNext;
		delete pnode;
		m_dwLength--;
		return SUCCESS;
	}
	
}	

//��ȡ�����нڵ������	
template<class T_ELE> 
DWORD LinkedList<T_ELE>::size()	
{	
	return m_dwLength;
}	


#endif // SIMPLELINKEDLIST_CPP