#include <stdio.h>

#include "lstack.h"

#define NUM 10	

struct hano_st {
	lstack_t *zhuz;	
};

//有三个柱子
static struct hano_st arr[3];
static int cnt = 0;

static void print_id(const void *data)
{
	const int *n = data;

	printf("%d ", *n);
}

static void print_arr(void)
{
	int i;

	for (i = 0; i < 3; i++) {
		printf("%c:", 'A'+i);
		lstack_traval(arr[i].zhuz, print_id);
		printf("\n");
	}
}

static void zhuz_init(int n)
{
	int i, j;

	for (j = 0; j < 3; j++) {
		arr[j].zhuz = lstack_init(sizeof(int));
		//if error
		if (j == 0) {
			for (i = n; i > 0; i--) {
				lstack_push(arr[j].zhuz, &i);		
			}
		}
	}
}

static void move(int src, int dest)
{
	int n;

	lstack_pop(arr[src].zhuz, &n);
	lstack_push(arr[dest].zhuz, &n);

	cnt ++;

	print_arr();
	//usleep(500000);
}

//hanoi(num, src, dest, tmp)
static void hanoi(int n, int src, int dest, int tmp)
{
	if (n < 1)
		return ; 
	if (n == 1) {
		move(src, dest);
		return ;
	}
	hanoi(n-1, src, tmp, dest);	
	move(src, dest);
	hanoi(n-1, tmp, dest, src);
}

int main(void)
{
	//初始化三个柱子
	zhuz_init(NUM);

	print_arr();
	getchar();

	hanoi(NUM, 0, 1, 2);

	printf("total: %ld\n", cnt);

	return 0;
}

