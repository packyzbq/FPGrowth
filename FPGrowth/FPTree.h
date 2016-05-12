#pragma once
#ifndef FPTREE_H
#define FPTREE_H

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "FPNode.h"
using namespace std;
/*
*	
*/
class FPTree
{
public:
	

	FPTree(int);
	~FPTree();
	void addNode(vector<int>, FPNode*, int);
	void createTree();
	void add_large1(int, FPNode*);
	void sortPattern(vector<int> Trans, vector<int> &sortedTrans);

	FPNode* getroot();
	map<int, FPNode*> getLarge_1();
	int get_minsup();

private:
	map<int,FPNode*> large_1; //ÅÅºÃÐòµÄ1-itemset
	FPNode* root;
	int min_sup;

	
	
};

#endif