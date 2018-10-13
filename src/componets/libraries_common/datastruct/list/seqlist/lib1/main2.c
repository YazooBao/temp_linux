#include <stdio.h>

#include "seqlist.h"

static int int_cmp(const void *data, const void *key)
{
	const int *d = data;
	const int *k = key;

	return *k-*d;
}

static void traval_int(const void *data)
{
	const int *d = data;

	printf("%d ", *d);
}	

int main(void)
{
	struct slist_st *mylist;
	int i, num;
	int delid;

	mylist = seqlist_init(sizeof(int));
	//if error
	
	srand(time(NULL));
	for (i = 0; i < 10; i++) {
		num = rand() % 100;	
		seqlist_insert(mylist, &num);
	}

	seqlist_traval(mylist, traval_int);
	printf("\n");

	seqlist_sort(mylist, int_cmp);
	printf("after sort\n");
	seqlist_traval(mylist, traval_int);
	printf("\n");

	delid = 1;
	seqlist_delete(mylist, &delid, int_cmp);

	printf("after delete\n");
	seqlist_traval(mylist, traval_int);
	printf("\n");

	seqlist_destroy(mylist);

	return 0;
}
