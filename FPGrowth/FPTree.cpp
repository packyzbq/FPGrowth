#include "FPTree.h"
#include "DBoperator.h"

FPTree::FPTree(int min_sup)
{
	root = new FPNode;
	root->child = new vector<FPNode*>;
	root->father = NULL;
	root->count = NULL;
	root->item_id = NULL;
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
//为trans排序，输出为sortedTrans
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

//以trans为单位，加入到树中
//paramter:
//	trans: 所需加入的trans
//	father: 加入的父节点
//	int: 当前所处理的脚标
void FPTree::addNode(vector<int> trans, FPNode* father, int current)
{
	int size_trans = trans.size();
	if (current >= size_trans)
		return;
	FPNode* tmpNode = NULL;
	if (father->child->size() != 0)
	{
		vector<FPNode*>::iterator child_iter = father->child->begin();
		for (; child_iter != father->child->end(); child_iter++)
		{
			if ((*(child_iter._Ptr))->item_id == trans[current])
			{
				tmpNode = *(child_iter._Ptr);
				tmpNode->count++;
				break;
			}
		}
	}
	
	//没有存在已存在的节点
	else if (tmpNode == NULL)
	{
		tmpNode = new FPNode;
		tmpNode->item_id = trans[current];
		tmpNode->father = father;
		tmpNode->count = 1;
		tmpNode->child = new vector<FPNode*>;
		father->child->push_back(tmpNode);
		large_1[trans[current]]->child->push_back(tmpNode);
	}
	//递归调用添加节点
	addNode(trans, tmpNode, ++current);	
}

//建树
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


FPNode* FPTree::getroot()
{
	return root;
}
map<int, FPNode*> FPTree::getLarge_1()
{
	return large_1;
}
int FPTree::get_minsup()
{
	return min_sup;
}