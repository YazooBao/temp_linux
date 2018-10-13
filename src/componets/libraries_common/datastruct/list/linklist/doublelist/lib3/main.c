#include <stdio.h>

#include "llist.h"

#define NAMESIZE	32

struct stu_st {
	int id;
	char name[NAMESIZE];
};

static int id_cmp(const void *data, const void *key)
{
	struct stu_st *d = (void *)data;
	int *k = (void *)key;

	return d->id - *k;
}

static void print_fun(const void *data)
{
	const struct stu_st *d = data;

	printf("%d %s\n", d->id, d->name);
}

int main(void)
{
	llist_t *mylist;		
	struct stu_st stu, *find;
	int delid;
	int i;

	if ((mylist = llist_init(sizeof(struct stu_st))) == NULL) {
		fprintf(stderr, "list_init(): failed\n");
		goto ERROR;
	}

	for (i = 0; i < 10; i++) {
		stu.id = i+1;
		snprintf(stu.name, NAMESIZE, "%c%c%chaha", 'a'+rand()%26, 'A'+rand()%26, 'a'+rand()%26);
		llist_insert(mylist, &stu, TAIL);
	}
	
	llist_traval(mylist, print_fun);

#if 0
	delid = 10;
	llist_delete(mylist, &delid, id_cmp);
	llist_traval(mylist, print_fun);

	delid = 8;
	find = (void *)llist_search(mylist, &delid, id_cmp);
	if (find == NULL)
		printf("not have!!!\n");
	else
		printf("find id: %d %s\n", find->id, find->name);
#endif
	
	delid = 5;
	if (llist_fetch(mylist, &delid, id_cmp, &stu) < 0) {
		printf("not have\n");
	} else {
		printf("fetch it:%d %s\n", stu.id, stu.name);
	}
	llist_traval(mylist, print_fun);

	llist_destroy(mylist);

	return 0;
ERROR:
	return 1;
}

