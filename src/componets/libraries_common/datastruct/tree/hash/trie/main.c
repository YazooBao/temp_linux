#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE		1024
#define WORDSIZE 	32
#define DESTSIZE	512
#define NR	26

struct node_st {
	struct node_st *arr[NR];
	char *str;
};

static int pos_hash(char ch)
{
	return ch - 'a';
}

static int read_file(FILE *fp, char *word, char *dest)
{
	char buf[BUFSIZE] = {};	

	if (fgets(buf, BUFSIZE, fp) == NULL)
		return -1;
	buf[strlen(buf)-1] = 0;

	sscanf(buf, "%s %s", word, dest);

	return 0;
}

static void tree_insert(struct node_st **root, const char *word, const char *dest)
{
	struct node_st *new;
	int i;

	if (*root == NULL) {
		new = malloc(sizeof(*new));	
		//if error
		for (i = 0; i < NR; i++)
			(new->arr)[i] = NULL;
		new->str = NULL;
		*root = new;	
	}
	if (*word == '\0') {
		(*root)->str = strdup(dest);
		return;
	}
	tree_insert(&((*root)->arr)[pos_hash(*word)], word+1, dest);
}

static char *tree_search(struct node_st *root, char *word)
{
	if (root == NULL)
		return NULL;	
	if (*word == '\0')
		return root->str;
	tree_search((root->arr)[pos_hash(*word)], word+1);
}

int main(int argc, char **argv)
{
	FILE *fp;
	struct node_st *root = NULL;
	char word[WORDSIZE] = {};
	char dest[DESTSIZE] = {};
	char *ret;

	if (argc < 3)
		return 1;

	fp = fopen(argv[1], "r");
	//if error

	while (1) {
		if (read_file(fp, word, dest) < 0)
			break;

		tree_insert(&root, word, dest);
		
		memset(word, '\0', WORDSIZE);
		memset(dest, '\0', DESTSIZE);
	}

#if 1
	if ((ret = tree_search(root, argv[2])) == NULL)
		printf("the word is wrong\n");
	else
		printf("%s: %s\n", argv[2], ret);
#endif

	return 0;
}

