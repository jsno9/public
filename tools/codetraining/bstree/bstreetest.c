#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int key;
	struct node *parent;
	struct node *left;
	struct node *right;
}Node,*BSTree;

static struct node *create_node(int key, Node *parent, Node *left, Node *right)
{
	Node *p=(Node *)malloc(sizeof(Node));
	if(p==NULL)
		return NULL;
	
	p->key=key;
	p->parent=NULL;
	p->left=NULL;
	p->right=NULL;
	
	return p;
}

void preorder(BSTree tree)
{
	if(tree!=NULL)
	{
		printf("%d,",tree->key);
		preorder(tree->left);
		preorder(tree->right);
	}
}

void inorder(BSTree tree)
{
	if(tree!=NULL)
	{
		preorder(tree->left);	
		printf("%d,",tree->key);
		preorder(tree->right);
	}
}

void postorder(BSTree tree)
{
	if(tree!=NULL)
	{
		preorder(tree->left);
		preorder(tree->right);
		printf("%d,",tree->key);
	}
}

Node *bstree_search(BSTree tree,int key)
{
	if(tree==NULL)
		return NULL;
	
	if(key<tree->key)
		return bstree_search(tree->left,key);
	else if(key>tree->key)
		return bstree_search(tree->right,key);
	else
		return tree;
	
}

Node *bstree_maximum(BSTree tree)
{
	if(tree==NULL)
		return NULL;
	
	if(tree->right!=NULL)
		return bstree_maximum(tree->right);
	else
		return tree;
}

Node *bstree_minimum(BSTree tree)
{
	if(tree==NULL)
		return NULL;
	
	if(tree->left!=NULL)
		return bstree_minimum(tree->right);
	else
		return tree;
}

//前驱
Node *bstree_prenode(BSTree z)
{
	if(z==NULL)
		return NULL;
	if(z->left!=NULL)
		return bstree_maximum(z->left);
	
	
	Node *y=(Node *)malloc(sizeof(Node));
	y=z->parent;
	while(z!=y>right && z->parent!=NULL)
	{
		z=y;
		y=y->parent;
	}
	return y;
	
}

Node *bstree_successornode(BSTree z)
{
	if(z==NULL)
		return NULL;
	if(z->right!=NULL)
		return bstree_minimum(z->right);
	
	
	Node *y=(Node *)malloc(sizeof(Node));
	y=z->parent;
	while(z!=y>left && z->parent!=NULL)
	{
		z=y;
		y=y->parent;
	}
	return y;
	
}

Node *bstree_insertnode(BSTree tree,Node *z)
{
	Node *x=tree;
	if(tree==NULL)
		return z;
	
	while(x!=NULL)
	{
		if(x->key>z->key)
		{
			x=x->left;
		}
		else
		{
			x=x->right;
		}
	}
	
	x=x->parent;
	if(x->key>z->key)
	{
		x->left=z;
		z->parent=x;
	}
	else
	{
		x->right=z;
		z->parent=x;
	}
	
	return tree;
	
}





