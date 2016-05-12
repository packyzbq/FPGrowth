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
	FPTree();
	~FPTree();
	void addNode();
	void createTree();
	void add_large1(int, FPNode*);
	void sortPattern(vector<int> Trans, vector<int> &sortedTrans);

private:
	map<int,FPNode*> large_1; //ÅÅºÃÐòµÄ1-itemset
	FPNode* root;

	
	
};

#endif