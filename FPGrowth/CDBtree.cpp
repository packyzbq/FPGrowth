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

//��ȡitem������ģʽ��,��һ�����,С��minsup�ĳ��⣬
//paramter:
//	item_id: item��id
//	vector<cpbase*> cpb: ���ش洢���е�item_id������ģʽ�� ��С��minsup�����ж���sizeΪ0
void CDBtree::getCPB(int id, vector<cpbase*> &cpb)
{
	//ɨ�����õ�·���洢��path�У�count��Ӧ��Ƶ����
	//vector<cpbase*> cpb;
	//FPNode *root = tree->getroot();
	FPNode *root = this->tree_root;
	map<int, FPNode*>& map_large1 = this->map_item;
	if (map_large1[id]->count < this->minsup)//С��Ƶ���� ���迼��
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

//��path��ӵ����У������ڴ洢��ʽΪ����洢������ǵ�������������������е�currentΪ����
//paramter:
//	cpb:һ������ģʽ��
//	cpbtree_root: ��ӽڵ�ĸ��ڵ�
//	current:����ǰ��������
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

//�����Ѿ��ҵ�����������ģʽ��������ͬʱҪ����ȥ��С��minsup��branch
void CDBtree::build_tree(vector<cpbase*>& cpblist)
{
	vector<cpbase*>::iterator cpb_iter = cpblist.begin();
	for (; cpb_iter != cpblist.end(); cpb_iter++)
	{
		addNode(*(cpb_iter._Ptr), cpbtree_root, 0);
	}
	//������ɣ����м�֦
	cut_node(cpbtree_root);
}

void CDBtree::gen_freq_pattern()
{
	//�ж�����fptree�Ƿ��ǵ�һ·��
	if (isSingelPath(cpbtree_root))
	{
		//���Ƶ����
		Output_freq(cpbtree_root);
	}
	else
	{
		//TODO
		CDBtree *cdbtree = new CDBtree(cpbtree_root,minsup);
		cdbtree->FPgrowth(cpbtree_root);
	}
	
}

//ɾ�������ͷ��ڴ�ռ�
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

//��������Ϊ��һ��item��������fptree׼��
void CDBtree::reset_tree()
{
	destroyTree(cpbtree_root);
	cpbtree_root = new FPNode;
	cpbtree_root->father = NULL;
	cpbtree_root->count = NULL;
	cpbtree_root->item_id = NULL;
}


//�ݹ������֦
void CDBtree::cut_node(FPNode* node)
{
	if (node->child == NULL)
		return;
	if (node->father != NULL && node->count < this->minsup)
	{
		destroyTree(node);// ע�⣬��һ����ʹ��child��vector�г���nullָ��
		return;
	}
	vector<FPNode*>::iterator iter;
	for (iter = node->child->begin(); iter != node->child->end(); iter++)
	{
		cut_node(*iter._Ptr);
	}
}

//�ж��Ƿ�Ϊ��һ·��
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
	//map �д���node��itemid�ļ�
	if (node->child == NULL)
		return;
	FPNode* tmpnode;
	if (tmp_map.find(node->item_id) != tmp_map.end())
	{
		tmpnode = tmp_map.find(node->item_id)->second;
		tmpnode->count += node->count;
		tmpnode->child->push_back(node);
	}
	//�����ڸü�
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
