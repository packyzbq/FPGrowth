#pragma once
#include "FPTree.h"

struct cpbase
{
	vector<int> path;
	int count;
};


class CDBtree
{
public:
	CDBtree(FPTree *root);
	CDBtree(FPNode* root, map<int, FPNode*> map_item, int min_sup);
	CDBtree::CDBtree(FPNode* root, int min_sup);
	~CDBtree();

	void getCPB(int,vector<cpbase*>&);
	void addNode(cpbase*, FPNode*, int);
	void build_tree(vector<cpbase*>&);
	void gen_freq_pattern();
	void destroyTree(FPNode*);
	void reset_tree();

	void FPgrowth(FPNode* );

private:
	//FPTree* tree;
	FPNode* tree_root;
	int minsup;
	map<int, FPNode*> map_item;
	FPNode* cpbtree_root;

	void cut_node(FPNode*);
	bool isSingelPath(FPNode*);
	void Output_freq(FPNode*);
	void gen_map_item(map<int, FPNode*>&, FPNode*);
};

