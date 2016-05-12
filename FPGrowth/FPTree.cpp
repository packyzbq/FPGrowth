#include "FPTree.h"
#include "DBoperator.h"

FPTree::FPTree(int min_sup)
{
	root = new FPNode;
	this->min_sup = min_sup;
}


FPTree::~FPTree()
{
}

//for the test
void FPTree::add_large1(int id, FPNode* node)
{
	large_1[id] = node;
}

struct tempNode
{
	int id;
	int count;
	bool operator > (const tempNode &other) const
	{
		return count > other.count;
	}
};

bool cmp(tempNode *a, tempNode *b)
{
	if (a->count > b->count)
		return true;
	else
		return false;
}
//Ϊtrans�������ΪsortedTrans
void FPTree::sortPattern(vector<int> Trans, vector<int> &sortedTrans)
{
	int trans_size = Trans.size();
	vector<tempNode*> tmpTrans;
	for (int i = 0; i < trans_size; i++)
	{
		tempNode *node = new tempNode;
		node->id = Trans[i];
		node->count = large_1[Trans[i]]->count;
		tmpTrans.push_back(node);
	}
	sort(tmpTrans.begin(), tmpTrans.end(), cmp);
	for (int i = 0; i < trans_size; i++)
	{
		sortedTrans.push_back(tmpTrans[i]->id);
	}
}

//��transΪ��λ�����뵽����
//paramter:
//	trans: ��������trans
//	father: ����ĸ��ڵ�
//	int: ��ǰ������Ľű�
void FPTree::addNode(vector<int> trans, FPNode* father, int current)
{
	int size_trans = trans.size();
	if (current >= size_trans)
		return;
	FPNode* tmpNode = NULL;
	for (vector<FPNode*>::iterator child_iter = father->child.begin(); child_iter != father->child.end(); child_iter++)
	{
		if ((*(child_iter._Ptr))->item_id == trans[current])
		{
			tmpNode = *(child_iter._Ptr);
			tmpNode->count++;
			break;
		}
	}
	//û�д����Ѵ��ڵĽڵ�
	if (tmpNode == NULL)
	{
		tmpNode = new FPNode;
		tmpNode->item_id = trans[current];
		tmpNode->father = father;
		tmpNode->count = 1;
		father->child.push_back(tmpNode);
		large_1[trans[current]]->child.push_back(tmpNode);
	}
	//�ݹ������ӽڵ�
	addNode(trans, tmpNode, ++current);	
}

//����
void FPTree::createTree()
{
	vector<int> curr_trans;
	vector<int> sorted_trans;
	int rowSize = getRowSize();
	for (int i = 0; i < rowSize; i++)
	{
		getRowTrans(i, curr_trans);
	}
	sortPattern(curr_trans, sorted_trans);
	addNode(sorted_trans, root, 0);
}