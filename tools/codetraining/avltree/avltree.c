#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int key;
    struct node *left;
    struct node *right;
    int height
}Node,Tree;

static Node *creat_node(int key,Node *left,Node *right)
{
    Node *p=(Node *)malloc(sizeof(Node));
    if(p==NULL)
        return NULL;
    
    p->key=key;
    p->left=left;
    p->right=right;
    p->height=0;
    return p;
}

int heighttree(Node *p)
{
    if(p==NULL)
        return 0;
    return p->height;
}

int max(int a,int b)
{
    if(a>b)
        return a;
    else
        return b;
}

static Node *left_left_rotation(Node *p)
{
    Node *pleft;
    
    pleft=p->left;
    p->left=pleft->right;
    pleft->right=p;
    
    p->height=1+max(heighttree(p->left),heighttree(p->right));
    pleft->height=1+max(heighttree(pleft->right),heighttree(pleft->left));
    
    return pleft;
}

static Node *right_right_rotation(Node *p)
{
    Node *pright;
    
    pright=p->right;
    p->right=pright->left;
    pright->left=p;
    
    p->height=1+max(heighttree(p->left),heighttree(p->right));
    pright->height=1+max(heighttree(pright->right),heighttree(pright->left));
    
    return pright;
}

static Node *left_right_rotation(Node *p)
{
	p->left=right_right_rotation(p->left);

	return left_left_rotation(p);
}

static Node *right_left_rotation(Node *p)
{
	p->left=left_left_rotation(p->left);

	return right_right_rotation(p);
}

Node *insert_node(Node *tree,int key)
{
	if(tree==NULL)
	{
        tree = creat_node(key,NULL,NULL);
	}
    else if(key<tree->key)
    {
        tree->left = insert_node(tree->left,key);
        if(heighttree(tree->left)-heighttree(tree->right) == 2)
        {
            if(key<tree->left->key)
                tree=left_left_rotation(tree);
            else
                tree=left_right_rotation(tree);
        }
    }
    else if(key>tree->key)
    {
        tree->right = insert_node(tree->right,key);
        if(heighttree(tree->right)-heighttree(tree->left) == 2)
        {
            if(key>tree->right->key)
                tree=right_right_rotation(tree);
            else
                tree=right_left_rotation(tree);
        }
    }
    else
    {
        printf("the key was in the tree already\n");
    }

    tree->height=1+max(heighttree(tree->right),heighttree(tree->left));
	

}

Node *delete_node(Node *tree,Node *p)
{
    if(tree==NULL || p==NULL)
        return NULL;

    if(p->key<tree->key)
    {
        tree->left = delete_node(tree->left,p);     
        if(heighttree(tree->right)-heighttree(tree->left) == 2)
        {
            if(key>tree->right->key)
                tree=right_right_rotation(tree);
            else
                tree=right_left_rotation(tree);
        } 
    }
    else if(p->key>tree->key)
    {
        tree->right = delete_node(tree->right,p);
        if(heighttree(tree->left)-heighttree(tree->right) == 2)
        {
            if(key<tree->left->key)
                tree=left_left_rotation(tree);
            else
                tree=left_right_rotation(tree);
        }
    }
    else
    {
        if((tree->left != NULL) && (tree->right != NULL))
        {
            if(heighttree(tree->left)>heighttree(tree->right))
            //Node *max=
        }
    }

}
















