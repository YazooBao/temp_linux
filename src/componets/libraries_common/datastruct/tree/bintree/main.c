#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAMESIZE 20

//data
struct stu_st {
	int id;
	char name[NAMESIZE];
};
//node
struct node_st {
	struct stu_st data;
	struct node_st *left;
	struct node_st *right;
};
static struct node_st *rootp = NULL;	

/*功能：向二叉树插入一个新的结点
 *@root:存储指向根结点地址的指针变量的地址
 *@stu:存储的数据
 * */
static int tree_insert_r(struct node_st **root, struct stu_st stu)
{
	struct node_st *new = NULL;	

	if (*root == NULL) {
		new = malloc(sizeof(*new));
		if (new == NULL)
			return -1;
		new->data = stu;
		new->left = new->right = NULL;
		*root = new;
		return 0;
	}

	if ((*root)->data.id >= stu.id)
		return tree_insert_r(&(*root)->left, stu);
 	else 
		return tree_insert_r(&(*root)->right, stu);
}

static int tree_insert(struct node_st **root, struct stu_st stu)
{
	struct node_st *new = NULL;	

	new = malloc(sizeof(*new));
	if (new == NULL)
		return -1;
	new->data = stu;
	new->left = new->right = NULL;

	while (*root != NULL) {
		if ((*root)->data.id >= stu.id)
			root = &(*root)->left;
		else 
			root = &(*root)->right;
	}	
	*root = new;

	return 0;
}

/*二叉树的中序遍历结果
 *@root:根结点的地址
 * */
static void tree_traval_mid(struct node_st *root)
{
	if (root == NULL)	
		return;
	tree_traval_mid(root->left);	
	printf("%d %s\n", root->data.id, root->data.name);
	tree_traval_mid(root->right);	
}
/*
 *打印二叉树
 *		右子树---》根结点---》左子树
 * */
static void __tree_draw(struct node_st *root, int index)
{
	int i;

	if (root == NULL)
		return ;
	__tree_draw(root->right, index+1);
	for (i = 0; i < index; i++) 
		printf("      ");
	printf("%d %s\n", root->data.id, root->data.name);
	__tree_draw(root->left, index+1);	
}

static void tree_draw(struct node_st *root)
{
	__tree_draw(root, 0);
}

static struct node_st *find_max(struct node_st *root)
{
	if (root->right == NULL)
		return root;
	return find_max(root->right);
}

static struct node_st *find_min(struct node_st *root)
{
	if (root->left == NULL)
		return root;
	return find_min(root->left);
}

static void __tree_delete(struct node_st **root)
{
	struct node_st *cur = *root;
	struct node_st *l = cur->left;
	struct node_st *r = cur->right;	

	if (l == NULL) {
		*root = r;	
	} else {	
		*root = l;
		find_max(l)->right = r;
	}
	cur->left = cur->right = NULL;
	free(cur);

	return ;
}

/*
 * 从树中删除一个结点
 * @root:存储树根的地址的指针变量的地址
 * @key:要删除结点
 * */
static int tree_delete(struct node_st **root, int key)
{
	if (*root == NULL)
		return -1;
	if ((*root)->data.id == key) {	
		__tree_delete(root);
		return 0;
	}
	if ((*root)->data.id > key)
		return tree_delete(&(*root)->left, key);
	else
		return tree_delete(&(*root)->right, key);
}

/*
 *树的搜索
 * */
static struct stu_st *tree_search(struct node_st *root, int key)
{
	if (root == NULL)	
		return NULL;
	if (root->data.id == key)
		return &root->data;
	if (root->data.id < key)
		return tree_search(root->right, key);
	else 
		return tree_search(root->left, key);
}

static void turn_left(struct node_st **root)
{
	struct node_st *cur = *root;

	*root = cur->right;
	cur->right = NULL;

	if ((*root)->left != NULL)	
		find_min((*root)->left)->left = cur;
	else
		(*root)->left = cur;

	tree_draw(rootp);
//	getchar();
	usleep(500000);
}

static void turn_right(struct node_st **root)
{
	struct node_st *cur = *root;

	*root = cur->left;
	cur->left = NULL;

	if ((*root)->right != NULL)
		find_max((*root)->right)->right = cur;
	else 
		(*root)->right = cur;

	tree_draw(rootp);
	//getchar();
	usleep(500000);
}

static int tree_get_num(struct node_st *root)
{
	if (root == NULL)
		return 0;
	return 1+tree_get_num(root->left)+tree_get_num(root->right);
}

/*
 *平衡的二叉树
 * */
static void tree_balance(struct node_st **root)
{
	int sub;

	if (*root == NULL)
		return;
	while (1) {
		sub = tree_get_num((*root)->left) - tree_get_num((*root)->right);	
		if (sub > 1) {
			turn_right(root);
		} else if (sub < -1) {
			turn_left(root);
		} else
			break;
	}

	tree_balance(&(*root)->left);
	tree_balance(&(*root)->right);
}

int main(void)
{
//	struct node_st *rootp = NULL;	
	struct stu_st stu, *srch;
	//int id[] = {5,9,3,6,10,15,2,1,7};
	int id[] = {1,2,3,4,5,6,7,8,9,10};
	int i, delid;	

	for (i = 0; i < sizeof(id)/sizeof(*id); i++) {
		stu.id = id[i];	
		snprintf(stu.name, NAMESIZE, "stu%d", i+1);	
		tree_insert(&rootp, stu);
	}

	tree_draw(rootp);
	getchar();

	tree_balance(&rootp);

#if 0
	//tree_traval_mid(rootp);
	stu.id = 4;
	strncpy(stu.name, "newnode", NAMESIZE);
	tree_insert_r(&rootp, stu);
	printf("after insert\n");
	tree_draw(rootp);

	printf("input the id which you want to delete\n");
	scanf("%d", &delid);
	tree_delete(&rootp, delid);
	tree_draw(rootp);

	printf("which to find(id)\n");
	scanf("%d", &delid);
	srch = tree_search(rootp, delid);
	if (srch == NULL)
		printf("not found\n");
	else
		printf("found it: %d %s\n", srch->id, srch->name);
#endif

	return 0;
}

