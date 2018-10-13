#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "seqlist.h"

#define NAMESIZE	32

struct stu_st {
	int id;
	char name[NAMESIZE];
};

static int id_cmp(const void *data1, const void *data2)
{
	const struct stu_st *d1 = data1;
	const struct stu_st *d2 = data2;

	return d1->id - d2->id;
}

static int name_cmp(const void *data1, const void *data2)
{
	const struct stu_st *d1 = data1;
	const struct stu_st *d2 = data2;

	return strcmp(d2->name, d1->name);
}

static void print_fun(const void *data)
{
	const struct stu_st *d = data;

	printf("%d %s\n", d->id, d->name);
}

int main(void)
{
	struct slist_st *list1, *list2;
	struct stu_st stu;
	int i;

	list1 = seqlist_init(sizeof(struct stu_st));
	//if error
	list2 = seqlist_init(sizeof(struct stu_st));
	//if error

	srand(time(NULL));

	for (i = 0; i < 10; i++) {
		stu.id = rand()%100;
		snprintf(stu.name, NAMESIZE, "%c%c%chaha", 'a'+rand()%26, 'A'+rand()%26, 'a'+rand()%26);
		seqlist_sortinsert(list2, &stu, name_cmp);	
		seqlist_insert(list1, &stu);	
	}

	printf("无序的\n");
	seqlist_traval(list1, print_fun);

	printf("有序的\n");
	seqlist_traval(list2, print_fun);

	seqlist_destroy(list1);
	seqlist_destroy(list2);

	return 0;
}
