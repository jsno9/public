#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int key;
    struct node *left;
    struct node *right;
    int height
}Node;

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

