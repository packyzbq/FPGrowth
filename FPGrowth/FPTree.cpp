#include "FPTree.h"


FPTree::FPTree()
{
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
//ÎªtransÅÅĞò£¬Êä³öÎªsortedTrans
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