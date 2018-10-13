#include <stdio.h>

struct node_st {
	struct node_st *prev;
	struct node_st *next;
};

struct test_st {
	int a;
	char b;
	char *c;
	struct node_st node;
};

int main(void)
{
	printf("%ld\n", (long)(&((struct test_st *)0)->node));	


	return 0;
}

