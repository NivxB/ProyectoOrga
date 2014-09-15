#ifndef TREENODE_H
#define TREENODE_H
#include <iostream>
//#include "IndexClass.h"
using namespace std;

class TreeNode{
public:
	TreeNode(int, bool);//orden, es hoja, numero pagina
	TreeNode();
    void insertNonFull(int);
    void splitChild(int, TreeNode *);
	void recorrerInorden();
	TreeNode* buscar(int);//key por buscar
    void insertar(int);
    int encontrarKey(int k);
    void split(int, TreeNode *);
    void remove(int);
    void removeFromLeaf(int);
    void removeFromNonLeaf(int);
    int getPred(int);
    int getSucc(int);
    void fill(int);
    void borrowFromPrev(int);
    void borrowFromNext(int);
    void merge(int);
	~TreeNode();
private:
	int degree;
	bool esHoja;
	int nKeys;
	int PageNum;
	//IndexClass* llaves;
	int* llaves;
	TreeNode** hijos;

	friend class Tree;
};


#endif
