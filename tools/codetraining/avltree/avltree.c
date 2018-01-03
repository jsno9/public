#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int key;
    struct node *left;
    struct node *right;
    int height;
}Node,Tree;

static Node *create_node(int key,Node *left,Node *right)
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
	p->right=left_left_rotation(p->right);

	return right_right_rotation(p);
}

Node *insert_node(Node *tree,int key)
{
	if(tree==NULL)
	{
        tree = create_node(key,NULL,NULL);
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
            {
                tree=right_right_rotation(tree);
                
            }
            else
            {
                tree=right_left_rotation(tree);
            }
        }
    }
    else
    {
        printf("the key was in the tree already\n");
    }

    tree->height=1+max(heighttree(tree->right),heighttree(tree->left));
    return tree;
	

}

static Node *maximum_node(Node *p)
{
    if(p==NULL)
        return NULL;
    
    while(p->right!=NULL)
    {
        p=p->right;
    }
    
    return p;
    
}

static Node *minimum_node(Node *p)
{
    if(p==NULL)
        return NULL;
    
    while(p->left!=NULL)
    {
        p=p->left;
    }
    
    return p;
    
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
            if(p->key>tree->right->key)
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
            if(p->key<tree->left->key)
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
            {
                Node *max=maximum_node(tree->left);
                tree->key=max->key;
                tree->left=delete_node(tree->left,max);
            }
            else
            {
                Node *min=minimum_node(tree->right);
                tree->key=min->key;
                tree->right=delete_node(tree->right,min);
            }
        }
        else
        {
            if(tree->left!=NULL)
            {
                free(tree->left);
            }
            else
                free(tree->right);
        }
    }

    return tree;
}

static Node *search_key(Node *tree,int key)
{
    if(tree==NULL)
        return NULL;
    
    if(key<tree->key)
        search_key(tree->left,key);
    else if(key>tree->key)
        search_key(tree->right,key);
    
    return tree;
}

Node *delete_key(Node *tree,int key)
{
    Node *p=search_key(tree,key);
    if(p!=NULL)
        tree=delete_node(tree,p);
    
    return tree;
}

void print_tree(Node *tree, int key, int direction)
{
    //printf("print tree\n");
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
            printf("%2d is root\n", tree->key);
        else                // tree是分支节点
            printf("%2d is %2d's %6s child\n", tree->key, key, direction==1?"right" : "left");

        print_tree(tree->left, tree->key, -1);
        print_tree(tree->right,tree->key,  1);
    }
}

void preorder_tree(Node *tree)
{
    if(tree!=NULL)
    {
        printf("%d ",tree->key);
        preorder_tree(tree->left);
        preorder_tree(tree->right);
    }
}

void inorder_tree(Node *tree)
{
    if(tree!=NULL)
    {
        inorder_tree(tree->left);
        printf("%d ",tree->key);
        inorder_tree(tree->right);
    }
}

void postorder_tree(Node *tree)
{
    if(tree!=NULL)
    {
        postorder_tree(tree->left);
        postorder_tree(tree->right);
        printf("%d ",tree->key);
    }
}

#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )
void main()
{
    static int arr[]= {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};

    int i,ilen;
    Node *root=NULL;

    printf("== 高度: %d\n", heighttree(root));
    //printf("main\n");
    printf("== 依次添加: ");
    ilen = TBL_SIZE(arr);
    for(i=0; i<ilen; i++)
    {
        printf("%d ", arr[i]);
        
        root = insert_node(root, arr[i]);
      //  print_tree(root, root->key, 0);
    }

    printf("\n== 前序遍历: ");
    preorder_tree(root);

    printf("\n== 中序遍历: ");
    inorder_tree(root);

    printf("\n== 后序遍历: ");
    postorder_tree(root);
    printf("\n");

    printf("== 高度: %d\n", heighttree(root));
    printf("== 最小值: %d\n", minimum_node(root)->key);
    printf("== 最大值: %d\n", maximum_node(root)->key);
    printf("== 树的详细信息: \n");
    print_tree(root, root->key, 0);


    i = 8;
    printf("\n== 删除根节点: %d", i);
    root = delete_key(root, i);

    printf("\n== 高度: %d", heighttree(root));
    printf("\n== 中序遍历: ");
//    inorder_avltree(root);
    printf("\n== 树的详细信息: \n");
    print_tree(root, root->key, 0);

}











