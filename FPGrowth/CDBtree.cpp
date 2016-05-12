#include "CDBtree.h"
#include <vector>
#include <map>
using namespace std;

/*
CDBtree::CDBtree(FPTree *tree)
{
	this->tree = tree;
	cpbtree_root = new FPNode;
	cpbtree_root->father = NULL;
	cpbtree_root->count = NULL; 
	cpbtree_root->item_id = NULL;
}
*/


CDBtree::CDBtree(FPNode* root, map<int, FPNode*> map_item ,int min_sup)
{
	tree_root = root;
	this->map_item = map_item;
	this->minsup = min_sup;

}

CDBtree::CDBtree(FPNode* root, int min_sup)
{
	tree_root = root;
	if (map_item.size() == 0)
		gen_map_item(this->map_item, root);
	this->minsup = min_sup;

}

CDBtree::~CDBtree()
{
}

void CDBtree::FPgrowth(FPNode* root)
{
	//TODO
	map<int, FPNode*>::iterator map_iter;
	for (map_iter = map_item.begin(); map_iter != map_item.end(); map_iter++)
	{
		vector<cpbase*> cpb;
		getCPB(map_iter->first, cpb);
		vector<cpbase*>::iterator it;
		build_tree(cpb);
		gen_freq_pattern();
	}
}

//获取item的条件模式基,第一项除外,小于minsup的除外，
//paramter:
//	item_id: item的id
//	vector<cpbase*> cpb: 返回存储所有的item_id的条件模式基 若小于minsup，则判断其size为0
void CDBtree::getCPB(int id, vector<cpbase*> &cpb)
{
	//扫描树得到路径存储在path中，count对应其频繁度
	//vector<cpbase*> cpb;
	//FPNode *root = tree->getroot();
	FPNode *root = this->tree_root;
	map<int, FPNode*>& map_large1 = this->map_item;
	if (map_large1[id]->count < this->minsup)//小于频繁集 则不予考虑
		return;
	vector<FPNode*>::iterator node_iter;
	for (node_iter = map_large1[id]->child->begin(); node_iter != map_large1[id]->child->end(); node_iter++)
	{
		FPNode *tmpNode = *(node_iter._Ptr);
		cpbase *tmp_cpb = new cpbase;
		tmp_cpb->count = tmpNode->count;
		tmp_cpb->path.push_back(tmpNode->item_id);
		while (tmpNode->father != root)
		{
			tmp_cpb->path.push_back(tmpNode->father->item_id);
			tmpNode = tmpNode->father;
		}
		cpb.push_back(tmp_cpb);
	}

}

//将path添加到树中，但由于存储方式为倒序存储，因此是倒序遍历，但函数参数中的current为正序
//paramter:
//	cpb:一个条件模式基
//	cpbtree_root: 添加节点的父节点
//	current:处理当前结点的索引
void CDBtree::addNode(cpbase* cpb, FPNode* father, int curr)
{
	vector<FPNode*>::iterator child_iter;
	int current = cpb->path.size() - (curr-1);
	if (current >= cpb->path.size())
		return;
	FPNode* tmpNode = NULL;
	for (child_iter = father->child->begin(); child_iter != father->child->end(); child_iter++)
	{
		if ((*(child_iter._Ptr))->item_id == cpb->path[current])
		{
			tmpNode = *(child_iter._Ptr);
			tmpNode->count+=cpb->count;
			break;
		}
	}
	if (tmpNode == NULL)
	{
		tmpNode = new FPNode;
		tmpNode->item_id = cpb->path[current];
		tmpNode->father = father;
		tmpNode->count = cpb->count;
		father->child->push_back(tmpNode);
	}
	addNode(cpb, tmpNode, ++curr);
}

//根据已经找到的所有条件模式基建树，同时要剪裁去掉小于minsup的branch
void CDBtree::build_tree(vector<cpbase*>& cpblist)
{
	vector<cpbase*>::iterator cpb_iter = cpblist.begin();
	for (; cpb_iter != cpblist.end(); cpb_iter++)
	{
		addNode(*(cpb_iter._Ptr), cpbtree_root, 0);
	}
	//建树完成，进行剪枝
	cut_node(cpbtree_root);
}

void CDBtree::gen_freq_pattern()
{
	//判断条件fptree是否是单一路径
	if (isSingelPath(cpbtree_root))
	{
		//输出频繁集
		Output_freq(cpbtree_root);
	}
	else
	{
		//TODO
		CDBtree *cdbtree = new CDBtree(cpbtree_root,minsup);
		cdbtree->FPgrowth(cpbtree_root);
	}
	
}

//删除树，释放内存空间
void CDBtree::destroyTree(FPNode* d_root)
{
	if (d_root != NULL)
	{
		vector<FPNode *> *childNode = d_root->child;
		unsigned int size = childNode->size();
		for (int i = 0; i<size; i++)
		{
			destroyTree((*childNode)[i]);
		}
		delete d_root;
	}
	d_root = NULL;
}

//重置树，为下一个item建立条件fptree准备
void CDBtree::reset_tree()
{
	destroyTree(cpbtree_root);
	cpbtree_root = new FPNode;
	cpbtree_root->father = NULL;
	cpbtree_root->count = NULL;
	cpbtree_root->item_id = NULL;
}


//递归遍历剪枝
void CDBtree::cut_node(FPNode* node)
{
	if (node->child == NULL)
		return;
	if (node->father != NULL && node->count < this->minsup)
	{
		destroyTree(node);// 注意，这一步会使得child的vector中出现null指针
		return;
	}
	vector<FPNode*>::iterator iter;
	for (iter = node->child->begin(); iter != node->child->end(); iter++)
	{
		cut_node(*iter._Ptr);
	}
}

//判断是否为单一路径
bool CDBtree::isSingelPath(FPNode* node)
{
	if (node->child == NULL)
		return true;
	if (node->child->size() == 1)
		return isSingelPath((*(node->child))[0]);
	else
		return false;
}

void CDBtree::Output_freq(FPNode* node)
{
	cout << node->item_id << "  ";
	if (node->child == NULL)
		return;
	else
		Output_freq((*(node->child))[0]);
}

void CDBtree::gen_map_item(map<int, FPNode*> &tmp_map, FPNode* node)
{
	//map 中存在node的itemid的键
	if (node->child == NULL)
		return;
	FPNode* tmpnode;
	if (tmp_map.find(node->item_id) != tmp_map.end())
	{
		tmpnode = tmp_map.find(node->item_id)->second;
		tmpnode->count += node->count;
		tmpnode->child->push_back(node);
	}
	//不存在该键
	else
	{
		tmpnode = new FPNode;
		tmpnode->count = node->count;
		tmpnode->item_id = node->item_id;
		tmpnode->father = NULL;
		tmpnode->child->push_back(node);
		//tmp_map[node->count]
	}
	vector<FPNode*>::iterator iter;
	for (iter = node->child->begin(); iter != node->child->end(); iter++)
		gen_map_item(tmp_map, *(iter._Ptr));

}
