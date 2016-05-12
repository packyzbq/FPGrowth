#ifndef FPNODE_H

#define FPNODE_H
#include <vector>

struct FPNode
{
	int item_id;
	int count;
	FPNode *father;
	//FPNode *next;
	std::vector<FPNode*> child;

	bool operator < (const FPNode & other) const
	{
		return count < other.count;
	}
};


#endif