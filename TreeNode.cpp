#include "TreeNode.h"

TreeNode::TreeNode(){

}

TreeNode::TreeNode(int o, bool h){
	degree = o;
	esHoja = h;
	nKeys = 0;

	llaves = new int[2*t-1];
    hijos = new TreeNode *[2*t];
}

TreeNode::~TreeNode(){

}

void TreeNode::recorrerInorden(){
	int i;
    for (i=0; i<nKeys; i++){
        if (!esHoja){
            hijos[i]->recorrerInorden();
        }
        cout<<" "<<llaves[i];
    }
 
    if (!esHoja){
        hijos[i]->recorrerInorden();
    }
}

TreeNode* TreeNode::buscar(int ll){
	int i = 0;

	while(i<nKeys && ll>llaves[i]){
		i++;
	}

	if(esHoja){
		return NULL;
	}

	if(ll = llaves[i]){
		return this;
	}

	return llaves[i]->buscar(ll);
}


void TreeNode::insertar(int k){
    int i = nKeys-1;
    if (esHoja){
        while (i >= 0 && keys[i] > k){
            keys[i+1] = keys[i];
            i--;
        }

        llaves[i+1] = k;
        nKeys = nKeys+1;
    }else{
        while (i >= 0 && llaves[i] > k){
            i--;
        }
 
        if (hijos[i+1]->nKeys == 2*degree-1){
            split(i+1, hijos[i+1]);
            if (llaves[i+1] < k){
                i++;
            }
        }
        hijos[i+1]->insertar(k);
    }
}
 

void TreeNode::splitChild(int i, TreeNode *y){
    TreeNode *z = new TreeNode(y->degree, y->esHoja);
    z->nKeys = degree - 1;
 
    for (int j = 0; j < degree-1; j++)
        z->llaves[j] = y->llaves[j+degree];
 
    if (!y->esHoja){
        for (int j = 0; j < degree; j++)
            z->hijos[j] = y->hijos[j+degree];
    }

    y->nKeys = degree - 1;
 
    for (int j = nKeys; j >= i+1; j--)
        hijos[j+1] = hijos[j];
 
    hijos[i+1] = z;
 
    for (int j = nKeys-1; j >= i; j--)
        llaves[j+1] = llaves[j];
 
    llaves[i] = y->llaves[degree-1];
 
    nKeys = nKeys + 1;
}

void TreeNode::remove(int k){
    int idx = findKey(k);
 
    if (idx < nKeys && llaves[idx] == k){
 
        if (esHoja)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else{ 
        if (esHoja){
            cout << "La llave"<< k <<" no existe en el arbol\n";
            return;
        }

        bool flag = ( (idx==nKeys)? true : false );
 
        if (hijos[idx]->nKeys < degree)
            fill(idx);
 
        if (flag && idx > nKeys)
            hijos[idx-1]->remove(k);
        else
            hijos[idx]->remove(k);
    }
    return;
}

void TreeNode::removeFromLeaf (int idx){
 
    for (int i=idx+1; i<nKeys; ++i)
        llaves[i-1] = llaves[i];
 
    nKeys--;
 
    return;
}
 
void TreeNode::removeFromNonLeaf(int idx){
 
    int k = llaves[idx];

    if (hijos[idx]->nKeys >= degree){
        int pred = getPred(idx);
        llaves[idx] = pred;
        hijos[idx]->remove(pred);
    }else if(hijos[idx+1]->nKeys >= degree){
        int succ = getSucc(idx);
        llaves[idx] = succ;
        hijos[idx+1]->remove(succ);
    }
 
    else{
        merge(idx);
        hijos[idx]->remove(k);
    }
    return;
}
 
int TreeNode::getPred(int idx){
    TreeNode *cur=hijos[idx];
    while (!cur->esHoja)
        cur = cur->hijos[cur->nKeys];
 
    return cur->llaves[cur->nKeys-1];
}
 
int TreeNode::getSucc(int idx){
 
    TreeNode *cur = hijos[idx+1];
    while (!cur->esHoja)
        cur = cur->hijos[0];
 
    return cur->llaves[0];
}
 
void TreeNode::fill(int idx){
 
    if (idx!=0 && hijos[idx-1]->nKeys>=degree)
        borrowFromPrev(idx);

    else if (idx!=nKeys && hijos[idx+1]->nKeys>=degree)
        borrowFromNext(idx);
 
    else{
        if (idx != nKeys)
            merge(idx);
        else
            merge(idx-1);
    }
    return;
}
 

void TreeNode::borrowFromPrev(int idx){
 
    TreeNode *child=hijos[idx];
    TreeNode *sibling=hijos[idx-1];

    for (int i=child->nKeys-1; i>=0; --i)
        child->llaves[i+1] = child->llaves[i];
 
    if (!child->esHoja){
        for(int i=child->nKeys; i>=0; --i)
            child->hijos[i+1] = child->hijos[i];
    }
 
    child->llaves[0] = llaves[idx-1];
 
    if (!esHoja)
        child->hijos[0] = sibling->hijos[sibling->nKeys];
 
    llaves[idx-1] = sibling->llaves[sibling->nKeys-1];
 
    child->nKeys += 1;
    sibling->nKeys -= 1;
 
    return;
}
 
void TreeNode::borrowFromNext(int idx){
 
    TreeNode *child=hijos[idx];
    TreeNode *sibling=hijos[idx+1];
 
    child->llaves[(child->nKeys)] = llaves[idx];
 
    if (!(child->esHoja))
        child->hijos[(child->nKeys)+1] = sibling->hijos[0];
 
    llaves[idx] = sibling->llaves[0];
 
    for (int i=1; i<sibling->nKeys; ++i)
        sibling->llaves[i-1] = sibling->llaves[i];
 
    if (!sibling->esHoja){
        for(int i=1; i<=sibling->nKeys; ++i)
            sibling->hijos[i-1] = sibling->hijos[i];
    }
 
    child->nKeys += 1;
    sibling->nKeys -= 1;
 
    return;
}
 
void TreeNode::merge(int idx){
    TreeNode *child = hijos[idx];
    TreeNode *sibling = hijos[idx+1];
 
    child->llaves[degree-1] = llaves[idx];
 
    for (int i=0; i<sibling->nKeys; ++i)
        child->llaves[i+degree] = sibling->llaves[i];
 
    if (!child->esHoja){
        for(int i=0; i<=sibling->nKeys; ++i)
            child->hijos[i+degree] = sibling->hijos[i];
    }
 
    for (int i=idx+1; i<nKeys; ++i)
        llaves[i-1] = llaves[i];
 
    for (int i=idx+2; i<=nKeys; ++i)
        hijos[i-1] = hijos[i];

    child->nKeys += sibling->nKeys+1;
    nKeys--; 
    delete(sibling);
    return;
}