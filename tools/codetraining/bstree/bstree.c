#include <stdio.h>

//bstree,http://www.cnblogs.com/skywang12345/p/3576328.html

typedef struct BSTreeNode
{
	int key;
	struct BSTreeNode *left;
	struct BSTreeNode *right;
	struct BSTreeNode *parent;
}Node;

static Node* create_bstree_node(int key,Node *parent, Node *left, Node *right)
{
	Node *p = (Node *)malloc(sizeof(Node));
	p->key = key;
	p->left = left;
	p->right = right;
	p->parent = parent;
	return p;
}



int main()
{

}
