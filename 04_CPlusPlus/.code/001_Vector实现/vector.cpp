#include "vector.h"

template <class T_ELE>
Vector<T_ELE>::Vector()
	:m_dwInitSize(100), m_dwIncrement(5)
{
	//1.��������Ϊm_dwInitSize��T_ELE����	
	this->m_pVector = new T_ELE[this->m_dwInitSize];
	//2.���´����Ŀռ��ʼ��										
	memset(this->m_pVector, 0, this->m_dwInitSize*(sizeof T_ELE));
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
	int newLen= this->m_dwLen+this->m_dwIncrement;
	// 2. ����ռ�										
	T_ELE* temp = new T_ELE[newLen];
	memset(temp,0,newLen);
	if (temp)
	{
		// 3. �����ݸ��Ƶ��µĿռ�	
		// *** ����ʱѡ�� release��	***								
		memcpy(temp, this->m_pVector, this->m_dwLen*(sizeof T_ELE));

		// 4. �ͷ�ԭ���ռ�										
		delete[] this->m_pVector;
		this->m_pVector = temp;
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
		if (this->expand()!= SUCCESS)
		{
			return MALLOC_ERROR;
		}
	}
	//2.���µ�Ԫ�ظ��Ƶ����������һ��λ��										
	this->m_pVector[this->m_dwIndex] = Element;
	// std::cout << this->m_dwIndex << " " << this->m_pVector[this->m_dwIndex] << std::endl;
	//3.�޸�����ֵ										
	this->m_dwIndex++;
	return SUCCESS;
}

// *** ��������� [],����ֵ��Ϊ ���ã�������ָ��һ������ ***
template<class T_ELE>
T_ELE& Vector<T_ELE>::operator[](DWORD dwIndex)
{	
	// �����ں���������Χ ����
	if (dwIndex >= this->m_dwLen||dwIndex<0)
	{
		int error = INDEX_ERROR;
		std::cout << "Error: inedex error." << std::endl;
		return error;
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
	//1.�ж��Ƿ���Ҫ���ݣ������Ҫ�͵������ݵĺ���										
	if ((this->m_dwIndex + 1) > this->m_dwLen)
	{
		if (this->expand() != SUCCESS)
		{
			return MALLOC_ERROR;
		}
	}

	//2.�ж������Ƿ��ں�������										
	if (dwIndex == this->m_dwIndex)
	{
		//4.��ElementԪ�ظ��Ƶ�dwIndexλ��										
		this->m_pVector[this->m_dwIndex] = Element;
		//5.�޸�����ֵ				
		this->m_dwIndex++;
		return SUCCESS;
	}
	else if(dwIndex > this->m_dwIndex)
	{
		std::cout << "Error: Index out of range." << std::endl;
		return INDEX_ERROR;
	}
	else if (dwIndex < 0)
	{
		std::cout << "Error: Index error." << std::endl;
		return INDEX_ERROR;
	}

	//3.��dwIndexֻ���Ԫ�غ���		
	T_ELE temp = this->m_pVector[dwIndex + 1];
	for (int i = dwIndex; i < (int)this->m_dwIndex+1;i++)
	{
		if (i == dwIndex) {
			this->m_pVector[i + 1] = this->m_pVector[i];
		}
		else {
			temp += this->m_pVector[i + 1];
			this->m_pVector[i + 1] = temp - this->m_pVector[i + 1];
			temp -= this->m_pVector[i + 1];
		}
	}

	//4.��ElementԪ�ظ��Ƶ�dwIndexλ��										
	this[0][dwIndex] = Element;

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
	pEle[0] = this->m_pVector[dwIndex];
	//pEle[0] = (T_ELE)&this->m_pVector[dwIndex];
	return SUCCESS;
}

template<class T_ELE>
void Vector<T_ELE>::pop()
{
	if (this->m_dwIndex != 0)
	{
		this->m_pVector[--this->m_dwIndex] = 0;
	}
}

template<class T_ELE>
void Vector<T_ELE>::pop(DWORD dwLen)
{
	if (this->m_dwIndex != 0&&this->m_dwIndex>=dwLen)
	{
		while(dwLen--) this->m_pVector[--this->m_dwIndex] = 0;
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
	while (dwLen--) this->m_pVector[--this->m_dwIndex] = 0;
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
	if ((dwIndex+1) >this->m_dwIndex)
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
	T_ELE temp = this->m_pVector[dwIndex+1];
	for (int i = dwIndex; i < (int)this->m_dwIndex + 1; i++)
	{
		if (i == dwIndex) {
			this->m_pVector[i] = this->m_pVector[i+1];
		}
		else {
			temp += this->m_pVector[i + 1];
			this->m_pVector[i] = temp - this->m_pVector[i];
			temp -= this->m_pVector[i];
		}
	}

	//3.�޸�����ֵ				
	this->m_dwIndex--;
}

template<class T_ELE>
DWORD Vector<T_ELE>::size()
{
	return this->m_dwIndex;
}

