#ifndef TREE_CPP
#define TREE_CPP

class Monster
{
public:
	int ID;
	int Level;
	char Name[20];
public:
	Monster()
	{
		ID = 0;
		Level = 0;
		memcpy(&Name, "null", 5);
	}
	Monster(int ID, int Level, char* Name)
	{
		this->ID = ID;
		this->Level = Level;
		memcpy(&this->Name, Name, strlen(Name) + 1);
	}
};



template<class T>	
class TreeNode{	
public:	
	T element;//��ǰ�ڵ�洢������	
	TreeNode<T>* pLeft;//ָ�����ӽڵ��ָ��	
	TreeNode<T>* pRight;//ָ�����ӽڵ��ָ��	
	
	TreeNode(T& ele){	
	//��ʼ��Node�ڵ�	
	memset(&element,0,sizeof(TreeNode));
	//ΪԪ�ظ�ֵ	
	memcpy(&element,&ele,sizeof(T));
	pLeft = pRight = nullptr;
	}
};

// ����������
template<class T>	
class BSortTree{	
public:	
	BSortTree();//���캯��	
	~BSortTree();//��������	
public:	
	void clear(TreeNode<T>* pNode);
	void InOrderTraverse(TreeNode<T>* pNode);//�������	
	void PreOrderTraverse(TreeNode<T>* pNode);//ǰ�����	
	void PostOrderTraverse(TreeNode<T>* pNode);//�������	
	TreeNode<T>* GetRoot();//���ظ��ڵ�	
	int GetDepth(TreeNode<T>* pNode);//����ĳ���ڵ�ĸ߶�/���	
private:	
	void Init();
private:	
	TreeNode<T>* m_pRoot;//�����ָ��	
	int size;//����Ԫ���ܸ���	
};
	
template<class T> 	
BSortTree<T>::BSortTree()	
{	
	Init();
}

template<class T> 	
BSortTree<T>::~BSortTree(){	
	clear(m_pRoot);
}

template<class T>
void BSortTree<T>::clear(TreeNode<T>* pNode)
{
	if (pNode == nullptr)
	{
		return;
	}
	else {

		if (pNode->pLeft)
		{
			clear(pNode->pLeft);
		}
		if (pNode->pRight)
		{
			clear(pNode->pRight);
		}
		delete pNode;
		pNode = nullptr;
		size--;
	}
}
template<class T> 	
void BSortTree<T>::Init()	
{	
	
	Monster m1(1,1,(char*)"���");
	Monster m2(2,2, (char*)"Ұ��");
	Monster m3(3, 3, (char*)"Ұ��");
	Monster m4(4, 4, (char*)"ʿ��");
	Monster m5(5, 5, (char*)"����");
	Monster m6(6, 6, (char*)"������");
	Monster m7(7, 7, (char*)"�������");
	
	
	TreeNode<Monster>* n1 = new TreeNode<Monster>(m1);
	TreeNode<Monster>* n2 = new TreeNode<Monster>(m2);
	TreeNode<Monster>* n3 = new TreeNode<Monster>(m3);
	TreeNode<Monster>* n4 = new TreeNode<Monster>(m4);
	TreeNode<Monster>* n5 = new TreeNode<Monster>(m5);
	TreeNode<Monster>* n6 = new TreeNode<Monster>(m6);
	TreeNode<Monster>* n7 = new TreeNode<Monster>(m7);
	
	m_pRoot = n5;
	n5->pLeft = n4;
	n5->pRight = n6;
	n4->pLeft = n1;
	n1->pRight = n2;
	n6->pLeft = n3;
	n3->pRight = n7;
	size = 7;
	/*	
	
	
	*/	
}
template<class T> 	
TreeNode<T>* BSortTree<T>::GetRoot()	
{	
	return m_pRoot;
}
template<class T>	
int BSortTree<T>::GetDepth(TreeNode<T>* pNode)	
{	
    if(pNode==NULL) 	
    {	
	return 0;  	
    }
    else  	
    {  	
        int m = GetDepth(pNode->pLeft);  	
        int n = GetDepth(pNode->pRight);  	
        return (m > n) ? (m+1) : (n+1);   	
    }  	
}

template<class T> 
void BSortTree<T>::InOrderTraverse(TreeNode<T>* pNode)	
{	
	
	//�ݹ� ����������й���,�г��ֵ�����,�� �� ��
	Monster m;
	if (pNode == nullptr)
	{
		return ;
	}
	else {
		if (pNode->pLeft)
		{
			InOrderTraverse(pNode->pLeft);
		}
		m = pNode->element;
		cout << m.ID << endl;
		if (pNode->pRight)
		{
			InOrderTraverse(pNode->pRight);
		}
	}
	
}


//���� 5 4 1 2 6 3 7

//���� 1 2 4 5 3 7 6

//���� 2 1 4 7 3 6 5
	
template<class T> 	
void BSortTree<T>::PreOrderTraverse(TreeNode<T>* pNode)	
{	
	//ǰ��������й���,�г��ֵ�����	
	Monster m;
	if (pNode == nullptr)
	{
		return;
	}
	else {
		m = pNode->element;
		cout << m.ID << endl;
		if (pNode->pLeft)
		{
			PreOrderTraverse(pNode->pLeft);
		}
		if (pNode->pRight)
		{
			PreOrderTraverse(pNode->pRight);
		}
	}
}
	
template<class T> 	
void BSortTree<T>::PostOrderTraverse(TreeNode<T>* pNode)	
{	
	
	//����������й���,�г��ֵ�����	
	Monster m;
	if (pNode == nullptr)
	{
		return;
	}
	else {
		
		if (pNode->pLeft)
		{
			PostOrderTraverse(pNode->pLeft);
		}
		if (pNode->pRight)
		{
			PostOrderTraverse(pNode->pRight);
		}
		m = pNode->element;
		cout << m.ID << endl;
	}
}

#endif // TREE_CPP