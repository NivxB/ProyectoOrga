#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "TreeNode.h"

class Tree{
    TreeNode *root;
    int degree;
public:
    Tree(int);
    void recorrerInorden(); 
    TreeNode* buscar(int);
    void insertar(int);
    void clear();
    void remove(int);
    TreeNode* getRoot();
    void write(ostream&);
};

#endif