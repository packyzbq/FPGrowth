#include "FPTree.h"

int main()
{
	FPTree *tree = new FPTree();
	FPNode *a1 = new FPNode;
	FPNode *a2 = new FPNode;
	FPNode *a3 = new FPNode;
	a1->count = 2;
	a1->item_id = 0;
	a2->item_id = 1;
	a2->count = 5;
	a3->item_id = 2;
	a3->count = 3;
	tree->add_large1(0, a1);
	tree->add_large1(1, a2);
	tree->add_large1(2, a3);
	vector<int> sortedTrans;
	vector<int> Trans;
	Trans.push_back(0);
	Trans.push_back(1);

	tree->sortPattern(Trans, sortedTrans);
	cout << sortedTrans.size() << endl;
	cout << sortedTrans[0] << "  " << sortedTrans[1] << endl;

	getchar();
	return 0;
}