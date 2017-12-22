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
	while(z!=y->right && z->parent!=NULL)
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
	while(z!=y->left && z->parent!=NULL)
	{
		z=y;
		y=y->parent;
	}
	return y;
	
}

Node *bstree_insertnode(BSTree tree,Node *z)
{
	Node *x=tree;
    Node *y=NULL;
	if(tree==NULL)
		return z;
	//printf("insert 1\n");
	while(x!=NULL)
	{
        y=x;
		if(x->key>z->key)
		{
			x=x->left;
		}
		else
		{
			x=x->right;
		}
	}
	//printf("insert y->key=%d\n",y->key);
	x=y;
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
	//printf("insert 3\n");
	return tree;
	
}

Node *insert_key(BSTree tree,int key)
{
    Node *z=create_node(key,NULL,NULL,NULL);
    
    if(z!=NULL)
    {
        printf("z->key=%d\n",z->key);
        bstree_insertnode(tree,z);
    }
    else
        return tree;
}

Node *treenode_delete(BSTree tree,Node *z)
{
    Node *now=NULL;
    Node *son=NULL;
    if(z->left==NULL || z->right==NULL)
    {
        now=z;
    }
    else
    {
        now=bstree_successornode(z);
    }

    if(now->left!=NULL)
        son=now->left;
    else
        son=now->right;

    if(son!=NULL)
        son->parent=now->parent;

    if(son->parent==NULL)
        tree=now;
    else if(now==son->parent->left)
        son->parent->left=son;
    else
        son->parent->right=son;

    if(son!=z)
        z->key=son->key;

    if(now!=NULL)
        free(now);

    return tree;
    
    
}



Node *delete_key(BSTree tree,int key)
{
    Node *z=bstree_search(tree,key);
    if(z!=NULL)
        treenode_delete(tree,z);

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
        root = insert_key(root, arr[i]);
        //print_bstree(root, root->key, 0);
    }
    printf("\n== 前序遍历: ");
    preorder(root);

    printf("\n== 中序遍历: ");
    inorder(root);

    printf("\n== 后序遍历: ");
    postorder(root);
    printf("\n");

    printf("== 最小值: %d\n", bstree_minimum(root)->key);
    printf("== 最大值: %d\n", bstree_maximum(root)->key);
    printf("== 树的详细信息: \n");
    print_bstree(root, root->key, 0);

    printf("\n== 删除根节点: %d", arr[3]);
    root = delete_key(root, 7);

    printf("\n== 中序遍历: ");
    inorder(root);
    printf("\n");

    // 销毁二叉树
    destroy_bstree(root);
}





