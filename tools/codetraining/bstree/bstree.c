#include <stdio.h>

//bstree,http://www.cnblogs.com/skywang12345/p/3576328.html

typedef struct BSTreeNode
{
    int key;
    struct BSTreeNode *left;
    struct BSTreeNode *right;
    struct BSTreeNode *parent;
}Node,*BSTree;

static Node* create_bstree_node(int key,Node *parent, Node *left, Node *right)
{
    Node *p = (Node *)malloc(sizeof(Node));
    p->key = key;
    p->left = left;
    p->right = right;
    p->parent = parent;
    return p;
}

void preorder_bstree(BSTree tree)
{
    if(tree!=NULL)
    {
        printf("%d ",tree->key);
        preorder_bstree(tree->left);
        preorder_bstree(tree->right);
    }
}

void inorder_bstree(BSTree tree)
{
    if(tree!=NULL)
    {
        preorder_bstree(tree->left);
        printf("%d ",tree->key);
        preorder_bstree(tree->right);
    }
}

void postorder_bstree(BSTree tree)
{
    if(tree!=NULL)
    {
        preorder_bstree(tree->left);
        preorder_bstree(tree->right);
        printf("%d ",tree->key);
    }
}

Node* bstree_search(BSTree tree,int key)
{
    if(tree==NULL || x->key==key)
        return NULL;
    while(key<x->key)
        return bstree_search(x->left,key);
    else
        return bstree_search(x->right,key);
}

Node* bstree_maximum(BSTree tree)
{
    if(tree==NULL)
        return NULL;
    
    while(tree->right!=NULL)
        tree=tree->right;
    return tree;
}

Node* bstree_minimum(BSTree tree)
{
    if(tree==NULL)
        return NULL;
    
    while(tree->left!=NULL)
        tree=tree->left;
    return tree;
}

Node* bstree_predecessor(Node *x)
{
    if(x->left!=NULL)
        return bstree_maximum(x->left);
    
    Node* y=x->parent;
    while((y!=NULL) && (x==y->left))
    {
        x=y;
        y=y->parent;
    }
    
    return y;
}

Node* bstree_successor(Node *x)
{
    if(x->right!=NULL)
        return bstree_minimum(x->right);
    
    Node* y=x->parent;
    while((y!=NULL) && (x==y->right))
    {
        x=y;
        y=y->parent;
    }
    
    return y;
}

static Node* bstree_insert(BSTree tree,Node *z)
{
    Node *y=NULL;
    Node *x=tree;
    
    while(x!=NULL)
    {
        y=x;
        if(z->key<x->key)
        {
            x=x->left;
        }
        else
        {
            x=x->right;
        }
    }
    
    z->parent=y;
    
    if(y==NULL)
        tree=Z;
    else if(z->key<y->key)
        y->left=z;
    else
        y->right=z;
}

Node* insert_bstree(BSTree tree, int key)
{
    Node *z;

    z=create_bstree_node(key,NULL,NULL,NULL);
    if(z==NULL)
        return tree;

    return bstree_insert(tree,z);
}

int main()
{

}
