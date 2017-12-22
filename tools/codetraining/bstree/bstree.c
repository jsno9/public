#include <stdio.h>
#include <stdlib.h>
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

Node* bstree_search(BSTree x,int key)
{
    if(x==NULL || x->key==key)
        return x;
    if(key<x->key)
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
        tree=z;
    else if(z->key<y->key)
        y->left=z;
    else
        y->right=z;
    
    return tree;
}

Node* insert_bstree(BSTree tree, int key)
{
    Node *z;

    z=create_bstree_node(key,NULL,NULL,NULL);
    if(z==NULL)
        return tree;

    return bstree_insert(tree,z);
}

static Node* bstree_delete(BSTree tree, Node *z)
{
    Node *x=NULL;
    Node *y=NULL;
    
    if((z->left==NULL) || (z->right==NULL))
    {
        y=z;
    }
    else
    {
        y=bstree_successor(z);
    }

    if(y->left!=NULL)
    {
        x=y->left;
    }
    else
        x=y->right;

    if(x!=NULL)
        x->parent=y->parent;

    if(y->parent==NULL)
        tree=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;

    if(y!=z)
        z->key=y->key;

    if (y!=NULL)
        free(y);

    return tree;
        

}

Node* delete_bstree(BSTree tree,int key)
{
    Node *z,*node;
    z = bstree_search(tree, key);
    if(z != NULL)
        bstree_delete(tree,z);

    return tree;
}

void print_bstree(BSTree tree,int key,int direction)
{
    if(tree!=NULL)
    {
        if(direction==0)
            printf("%2d is root\n", tree->key);
        else
            printf("%2d is %2d's %6s child\n",tree->key,key,direction==1?"right" : "left");
        
        print_bstree(tree->left, tree->key, -1);
        print_bstree(tree->right,tree->key,  1);
    }
    
}

void destroy_bstree(BSTree tree)
{
    if(tree==NULL)
        return;
    
    if(tree->left!=NULL)
        destroy_bstree(tree->left);
    if(tree->right!=NULL)
        destroy_bstree(tree->right);
    
    free(tree);
}


void main()
{
    static int arr[]= {1,5,4,3,2,6};
    int i, ilen;
    BSTree root=NULL;

    printf("== 依次添加: ");
    ilen =  (sizeof(arr)) / (sizeof(arr[0]));
    for(i=0; i<ilen; i++)
    {
        printf("%d ", arr[i]);
        root = insert_bstree(root, arr[i]);
        //print_bstree(root, root->key, 0);
    }
    printf("\n== 前序遍历: ");
    preorder_bstree(root);

    printf("\n== 中序遍历: ");
    inorder_bstree(root);

    printf("\n== 后序遍历: ");
    postorder_bstree(root);
    printf("\n");

    printf("== 最小值: %d\n", bstree_minimum(root)->key);
    printf("== 最大值: %d\n", bstree_maximum(root)->key);
    printf("== 树的详细信息: \n");
    print_bstree(root, root->key, 0);

    printf("\n== 删除根节点: %d", arr[3]);
    root = delete_bstree(root, 7);

    printf("\n== 中序遍历: ");
    inorder_bstree(root);
    printf("\n");

    // 销毁二叉树
    destroy_bstree(root);
}





