#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define NAMESIZE	1024

struct stu_st {
	int id;
	char name[NAMESIZE];
	struct list_head node;
};

int main(void)
{
	int i;
	struct stu_st *new;
	struct list_head *pos, *next;
	struct stu_st *cur;
	int delid;

	LIST_HEAD(list);

	for (i = 0; i < 10; i++) {
		new = malloc(sizeof(*new));
		if (new == NULL)
			break;
		new->id = i+1;
		snprintf(new->name, NAMESIZE, "bendan%d", i+1);
		list_add_tail(&new->node, &list);
	}

	list_for_each(pos, &list) {
		cur = list_entry(pos, struct stu_st, node);	
		printf("%d %s\n", cur->id, cur->name);
	}

	//删除
	delid = 8;
	list_for_each(pos, &list) {
		cur = list_entry(pos, struct stu_st , node);
		if (cur->id == delid) {
			list_del_init(&cur->node);
			free(cur);
			break;
		}
	}
	list_for_each(pos, &list) {
		cur = list_entry(pos, struct stu_st, node);	
		printf("%d %s\n", cur->id, cur->name);
	}

	list_for_all(pos, next, &list) {
		cur = list_entry(pos, struct stu_st, node);	
		free(cur);		
	}

	return 0;
}
