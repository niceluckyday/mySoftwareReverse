

#define SUCCESS   1 // ִ�гɹ�
#define ERROR	 -1 // ִ��ʧ��	         	
	
template<class T>	
class TreeNode{	
public:	
	T element;				//��ǰ�ڵ�洢������	
	TreeNode<T>* pLeft;		//ָ�����ӽڵ��ָ��	
	TreeNode<T>* pRight;	//ָ�����ӽڵ��ָ��	
	TreeNode<T>* pParent;	//ָ�򸸽���ָ��	
	
	TreeNode(T& ele){
		//��ʼ��Node�ڵ�	
		memset(&element,0,sizeof(TreeNode));
		//ΪԪ�ظ�ֵ	
		memcpy(&element,&ele,sizeof(T));
		pLeft = pRight = pParent = nullptr;
	}

	//����== �Ƚ�������Ƿ����
	bool operator==(TreeNode<T>* node){ 
		return node->element == element?true:false;
	}
};
	



template<class T>	
class BSortTree{	
public:	
	BSortTree();//���캯��
	~BSortTree();//��������	
public:
	bool IsEmpty();//�ж����Ƿ�Ϊ��
	DWORD Insert(T element);//�����ڵ�
	void Delete(T element);//ɾ���ڵ�
	TreeNode<T>* Search(T element);//���ҽڵ�
	void InOrderTraverse(TreeNode<T>* pNode);//�������
	void PreOrderTraverse(TreeNode<T>* pNode);//ǰ�����
	void PostOrderTraverse(TreeNode<T>* pNode);//�������
private:	
	TreeNode<T>* SearchNode(TreeNode<T>* pNode,T element);//��ȡ��pNodeΪ������С�ڵ�
	DWORD InsertNode(T element, TreeNode<T>* pNode);//�����ڵ�
	TreeNode<T>* DeleteNode(TreeNode<T>* pNode);//ɾ���ڵ�
	void Clear(TreeNode<T>* pNode);//������нڵ�
	
	
	//��ȡ��pNodeΪ�������ڵ�
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode)
	{
		if (pNode->pLeft == nullptr && pNode->pRight == nullptr) return pNode;
		else return GetMinNode(pNode->pRight);
	}
	
	//��ȡ��pNodeΪ������С�ڵ�
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode)
	{
		if (pNode->pLeft == nullptr && pNode->pRight == nullptr) return pNode;
		else return GetMinNode(pNode->pLeft);
	}

private:	
	TreeNode<T>* m_pRoot;//�����ָ��
	int size;//����Ԫ���ܸ���
};
	
template<class T> 	
BSortTree<T>::BSortTree()	
{	
	m_pRoot = nullptr;
	size = 0;
}
template<class T> 	
BSortTree<T>::~BSortTree(){	
	Clear(m_pRoot);
}

template<class T> 	
DWORD BSortTree<T>::Insert(T element)	
{	
	//������ڵ�Ϊ��
	if ( !m_pRoot )
	{
		m_pRoot = new TreeNode<T>(element);
		size++;
		return SUCCESS;
	}
	//������ڵ㲻Ϊ��
	return InsertNode(element, m_pRoot);
}
template<class T> 	
DWORD BSortTree<T>::InsertNode(T element, TreeNode<T>* pNode)	
{	
	//��Ԫ�ط�װ���ڵ���
	TreeNode<T>* pNewNode = new TreeNode<T>(element);
	//���element == ��ǰ�ڵ� ֱ�ӷ���
	if(element == pNode->element)
	{
		return SUCCESS;
	}
	//���pNode�����ӽڵ�Ϊnullptr ����element < ��ǰ�ڵ�
	if(pNode->pLeft == nullptr && element < pNode->element)
	{
		pNode->pLeft = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//���pNode�����ӽڵ�Ϊnullptr ����element > ��ǰ�ڵ�
	if(pNode->pRight == nullptr && element > pNode->element){
		pNode->pRight = pNewNode;
		pNewNode->pParent = pNode;
		size++;
		return SUCCESS;
	}
	//���element<��ǰ�ڵ� �ҵ�ǰ�ڵ����������Ϊ��
	if(element < pNode->element)
	{
		InsertNode(element,pNode->pLeft);
	}
	else
	{
		InsertNode(element,pNode->pRight);
	}
	return SUCCESS;
}
	
template<class T> 	
void BSortTree<T>::Clear(TreeNode<T>* pNode)	
{	
	if(pNode!=nullptr)
	{
		Clear(pNode->pLeft);
		Clear(pNode->pRight);
		delete pNode;
		pNode=nullptr;
	}
}
	
template<class T> 	
bool BSortTree<T>::IsEmpty()	
{	
	return size==0?true:false;
}
	
template<class T> 	
TreeNode<T>* BSortTree<T>::Search(T element)	
{	
	return SearchNode(m_pRoot, element);
}
template<class T> 	
TreeNode<T>* BSortTree<T>::SearchNode(TreeNode<T>* pNode,T element)	
{	
	if(pNode == nullptr)	//����ڵ�Ϊnullptr	
	{
		return nullptr;
	}
	else if(element == pNode->element)	//������	
	{
		return pNode;
	}//����Ƚڵ��Ԫ��С ������	
	else if(element < pNode->element)
	{
		return SearchNode(pNode->pLeft,element);
	}
	else	//����Ƚڵ��Ԫ�ش� ������	
	{
		return SearchNode(pNode->pRight,element);
	}
}


template<class T> 	
void BSortTree<T>::Delete(T element)
{	
	TreeNode<T>* pnode = SearchNode(m_pRoot, element);
	if (!pnode) {
		return;
	}
	DeleteNode(pnode);

}
template<class T> 	
TreeNode<T>* BSortTree<T>::DeleteNode(TreeNode<T>* pNode)	
{	
	//���һ��Ҷ/*�ӽڵ�

	//	1��ɾ���ýڵ�

	//	2�������ڵ�(�������)ָ����NULL

	//	���2��ֻ��һ������

	//	1��ɾ���ýڵ�

	//	2�������ڵ�(�������)ָ��ָ������

	//	���3��������������

	//	1������������С�Ľڵ�ȡ��Դ�ڵ�

	//	2���ٵݹ�ɾ����С�ڵ�*/
	
	// ���и��ڵ���ڵ�

	if (pNode->pLeft==nullptr&&pNode->pRight==nullptr)
	{
		if (pNode == m_pRoot)
		{
			delete pNode;
		}
		else {
			(pNode->pParent->pLeft == pNode) ? pNode->pParent->pLeft = nullptr : pNode->pParent->pRight = nullptr;
			delete pNode;
		}
	}
	else if (pNode->pLeft != nullptr && pNode->pRight == nullptr) {
		if (pNode == m_pRoot)
		{
			m_pRoot = pNode->pLeft;
			delete pNode;
		}
		else {
			(pNode->pParent->pLeft == pNode) ? pNode->pParent->pLeft = pNode->pLeft : pNode->pParent->pRight = pNode->pLeft;
			delete pNode;
		}
	}
	else if (pNode->pLeft == nullptr && pNode->pRight != nullptr) {
		if (pNode == m_pRoot)
		{
			m_pRoot = pNode->pRight;
			delete pNode;
		}
		else {
			(pNode->pParent->pLeft == pNode) ? pNode->pParent->pLeft = pNode->pRight : pNode->pParent->pRight = pNode->pRight;
			delete pNode;
		}
	}
	else {
		TreeNode<T>* pnode = GetMinNode(pNode->pRight);
		if (pNode == m_pRoot)
		{
			m_pRoot = pNode->pRight;
			pnode->pLeft = pNode->pLeft;
			delete pNode;
		}
		else {
			(pNode->pParent->pLeft == pNode) ? pNode->pParent->pLeft = pNode->pRight : pNode->pParent->pRight = pNode->pRight;
			pnode->pLeft = pNode->pLeft;
			delete pNode;
		}
	}

	size--;
	return nullptr;
}

