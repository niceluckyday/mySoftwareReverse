#include "Vector.h"												

template <class T_ELE>
Vector<T_ELE>::Vector()
	:m_dwInitSize(100), m_dwIncrement(5)
{
	//1.��������Ϊm_dwInitSize��T_ELE����											
	this->m_pVector = new T_ELE[this->m_dwInitSize];
	//2.���´����Ŀռ��ʼ��											
	memset(this->m_pVector, 0, this->m_dwInitSize * (sizeof T_ELE));
	//3.��������ֵ											
	this->m_dwLen = this->m_dwInitSize;
	this->m_dwIndex = 0;
}
template <class T_ELE>
Vector<T_ELE>::Vector(DWORD dwSize)
	:m_dwIncrement(5)
{
	this->m_dwInitSize = dwSize;
	//1.��������Ϊm_dwInitSize��T_ELE����											
	this->m_pVector = new T_ELE[this->m_dwInitSize];
	//2.���´����Ŀռ��ʼ��											
	memset(this->m_pVector, 0, this->m_dwInitSize * (sizeof T_ELE));
	//3.��������ֵ											
	this->m_dwLen = this->m_dwInitSize;
	this->m_dwIndex = 0;

}
template <class T_ELE>
Vector<T_ELE>::~Vector()
{
	//�ͷſռ� delete[]											
	delete[] this->m_pVector;
	this->m_pVector = nullptr;

}

template <class T_ELE>
BOOL Vector<T_ELE>::expand()
{
	// 1. �������Ӻ�ĳ���											
	int newLen = this->m_dwLen + this->m_dwIncrement;
	// 2. ����ռ�											
	T_ELE* temp = new T_ELE[newLen];
	memset(temp, 0, newLen);
	if (temp)
	{
		// 3. �����ݸ��Ƶ��µĿռ�										
		// *** ����ʱѡ�� release��	***									
		memcpy(temp, this->m_pVector, this->m_dwLen * (sizeof T_ELE));

		// 4. �ͷ�ԭ���ռ�										
		delete[] this->m_pVector;
		this->m_pVector = temp;
		temp = nullptr;
		// 5. Ϊ�������Ը�ֵ										
		this->m_dwLen += this->m_dwIncrement;
		return SUCCESS;
	}
	return ERROR;
}

template <class T_ELE>
DWORD  Vector<T_ELE>::append(T_ELE Element)
{
	//1.�ж��Ƿ���Ҫ���ݣ������Ҫ�͵������ݵĺ���											
	if ((this->m_dwIndex + 1) > this->m_dwLen)
	{
		if (this->expand() != SUCCESS)
		{
			return MALLOC_ERROR;
		}
	}
	//2.���µ�Ԫ�ظ��Ƶ����������һ��λ��											
	memcpy(&this->m_pVector[this->m_dwIndex], &Element, sizeof Element);
	//3.�޸�����ֵ											
	this->m_dwIndex++;
	return SUCCESS;
}
template <class T_ELE>
DWORD Vector<T_ELE>::append(DWORD argc, T_ELE...)
{
	//1.�ж��Ƿ���Ҫ���ݣ������Ҫ�͵������ݵĺ���											
	if ((this->m_dwIndex + 1*argc) > this->m_dwLen)
	{
		if (this->expand() != SUCCESS)
		{
			return MALLOC_ERROR;
		}
	}

	va_list va_args;
	va_start(va_args,argc);
	T_ELE va_argv;
	for (int i = 0; i < argc; i++)
	{
		va_argv = va_arg(va_args, T_ELE);
		//2.���µ�Ԫ�ظ��Ƶ����������һ��λ��	
		memcpy(&this->m_pVector[this->m_dwIndex], &va_argv, sizeof T_ELE);
		//3.�޸�����ֵ											
		this->m_dwIndex++;
	}

	return SUCCESS;
	
}


// *** ��������� [],����ֵ��Ϊ ���ã�������ָ��һ������ ***												
template<class T_ELE>
T_ELE& Vector<T_ELE>::operator[](DWORD dwIndex)
{
	// �����ں���������Χ ����											
	if (dwIndex >= this->m_dwLen || dwIndex < 0)
	{
		int error = INDEX_ERROR;
		std::cout << "Error: inedex error." << std::endl;
		return (T_ELE&)error;
	}

	if (dwIndex == this->m_dwIndex)
	{
		this->m_dwIndex++;
		return this->m_pVector[dwIndex];
	}
	return this->m_pVector[dwIndex];
}


template <class T_ELE>
DWORD  Vector<T_ELE>::insert(DWORD dwIndex, T_ELE Element)
{
	//1.�ж������Ƿ��ں�������											
	if (dwIndex > this->m_dwIndex)
	{
		std::cout << "Error: Index out of range." << std::endl;
		return INDEX_ERROR;
	}
	else if (dwIndex < 0)
	{
		std::cout << "Error: Index error." << std::endl;
		return INDEX_ERROR;
	}

	//2.�ж��Ƿ���Ҫ���ݣ������Ҫ�͵������ݵĺ���											
	if (this->m_dwIndex >= this->m_dwLen)
	{
		if (this->expand() != SUCCESS)
		{
			return MALLOC_ERROR;
		}
	}

	//3.��dwIndexֻ���Ԫ�غ���											
	for (int i = this->m_dwIndex; i > dwIndex; i--)
	{
		memcpy(&this->m_pVector[i], &this->m_pVector[i-1], sizeof T_ELE);
	}

	//4.��ElementԪ�ظ��Ƶ�dwIndexλ��											
	memcpy(&this->m_pVector[dwIndex], &Element, sizeof T_ELE);
	//5.�޸�����ֵ											
	this->m_dwIndex++;
	return SUCCESS;
}

template <class T_ELE>
DWORD Vector<T_ELE>::at(DWORD dwIndex, OUT T_ELE* pEle)
{
	//�ж������Ƿ��ں�������											
	if (dwIndex >= this->m_dwIndex)
	{
		std::cout << "Error: Index out of range." << std::endl;
		return INDEX_ERROR;
	}
	else if (dwIndex < 0)
	{
		std::cout << "Error: Index error." << std::endl;
		return INDEX_ERROR;
	}

	//��dwIndex��ֵ���Ƶ�pEleָ�����ڴ�											
	//pEle[0] = this->m_pVector[dwIndex];
	memcpy(pEle,&this->m_pVector[dwIndex],sizeof T_ELE);
	return SUCCESS;
}

template<class T_ELE>
void Vector<T_ELE>::pop()
{
	if (this->m_dwIndex != 0)
	{
		memset(&this->m_pVector[--this->m_dwIndex], 0, sizeof T_ELE);
	}
}

template<class T_ELE>
void Vector<T_ELE>::pop(DWORD dwLen)
{
	if (this->m_dwIndex != 0 && this->m_dwIndex >= dwLen)
	{
		while (dwLen--) memset(&this->m_pVector[--this->m_dwIndex], 0, sizeof T_ELE);
	}
}

template<class T_ELE>
DWORD Vector<T_ELE>::capacity()
{
	return this->m_dwLen - this->m_dwIndex;
}

template<class T_ELE>
void Vector<T_ELE>::clear()
{
	DWORD dwLen = this->m_dwIndex;
	while (dwLen--) memset(&this->m_pVector[--this->m_dwIndex], 0, sizeof T_ELE);
}

template<class T_ELE>
BOOL Vector<T_ELE>::empty()
{
	if (this->m_dwIndex == 0) return true;
	return false;
}

template<class T_ELE>
void Vector<T_ELE>::erase(DWORD dwIndex)
{
	//1.�ж������Ƿ��ں�������											
	if (dwIndex >= this->m_dwIndex)
	{
		std::cout << "Error: Index out of range." << std::endl;
		return;
	}
	else if (dwIndex < 0)
	{
		std::cout << "Error: Index error." << std::endl;
		return;
	}

	//2.��dwIndexֻ���Ԫ��ǰ��											
	for (int i = dwIndex; i < this->m_dwIndex; i++)
	{
		memcpy(&this->m_pVector[i],&this->m_pVector[i+1],sizeof T_ELE);
	}

	//3.�޸�����ֵ											
	this->m_dwIndex--;
}

template<class T_ELE>
DWORD Vector<T_ELE>::size()
{
	return this->m_dwIndex;
}

