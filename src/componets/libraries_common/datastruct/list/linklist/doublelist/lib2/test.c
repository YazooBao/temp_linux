#include <stdio.h>

//变长结构体
struct data_st {
	int id;
	int age;
	float score;
	char name[1];
}__attribute__((packed));

int main(void)
{
	struct data_st *point;

	point = malloc(sizeof(*point)+100);

	printf("%d\n",sizeof(*point));

	printf("point: %p\n", point);
	printf("point->name: %p\n", point->name);


	return 0;
}

