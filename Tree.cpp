#include "Tree.h"

Tree::Tree(int d){
	root = NULL;
	degree = d;
}

TreeNode* Tree::getRoot(){
    return root;
}

void Tree::clear(){
    if (root){
        //root -> clear();
        //delete root;
        root = NULL;
    }
}

void Tree::write(ostream& out){
    if (root){
        root -> write(out);
    }
}

void Tree::recorrerInorden(){
	if(root != NULL){
		root->recorrerInorden();
	}
}

TreeNode* Tree::buscar(int i){
	if(root!=NULL){
		return root->buscar(i);
	}
	return NULL;
}

void Tree::insertar(int k){
    if (root == NULL){
        root = new TreeNode(degree, true);
        root->llaves[0] = k;//inserta solo llave, falta offset
        root->nKeys = 1;
    }else{
        if (root->nKeys == 2*degree-1){
            TreeNode *s = new TreeNode(degree, false);
            s->hijos[0] = root;
            s->split(0, root);

            int i = 0;
            if (s->llaves[0] < k){
                i++;
            }
            s->hijos[i]->insertar(k);
            root = s;
        }else{
            root->insertar(k);
        }
    }
}

void Tree::remove(int k){
    if (!root){
        cout << "El árbol está vacío\n";
        return;
    }

    root->remove(k);

    if (root->nKeys==0){
        TreeNode *tmp = root;
        if (root->esHoja)
            root = NULL;
        else
            root = root->hijos[0];

        delete tmp;
    }
    return;
}
