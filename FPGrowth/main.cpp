#include "FPTree.h"

int main()
{
	FPTree *tree = new FPTree(10);
	FPNode *a1 = new FPNode;
	FPNode *a2 = new FPNode;
	FPNode *a3 = new FPNode;
	FPNode *a4 = new FPNode;
	a1->count = 2;
	a1->item_id = 0;
	a2->item_id = 1;
	a2->count = 5;
	a3->item_id = 2;
	a3->count = 3;
	a4->item_id = 3;
	a4->count = 9;
	tree->add_large1(0, a1);
	tree->add_large1(1, a2);
	tree->add_large1(2, a3);
	tree->add_large1(a4->item_id, a4);
	vector<int> sTrans;
	vector<int> sTrans2;
	vector<int> Trans;
	Trans.push_back(0);
	Trans.push_back(1);
	Trans.push_back(3);

	tree->sortPattern(Trans, sTrans);

	Trans.clear();
	Trans.push_back(2);
	Trans.push_back(3);
	Trans.push_back(1);

	tree->sortPattern(Trans, sTrans2);

	//cout << sortedTrans.size() << endl;
	for (int i = 0; i < sTrans.size(); i++)
		cout << sTrans[i] << "  " ;
	cout << endl;
	for (int i = 0; i < sTrans2.size(); i++)
		cout << sTrans2[i] << "  ";

	tree->addNode(sTrans, tree->root, 0);
	tree->addNode(sTrans2, tree->root, 0);
	getchar();
	return 0;
}