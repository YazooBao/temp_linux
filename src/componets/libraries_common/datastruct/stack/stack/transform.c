#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
static int is_number(char ch)
{
	return ch >= '0' && ch <= '9';
}

static int is_operator(char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

static int is_left(char ch)
{
	return ch == '(';
}

static int is_right(char ch)
{
	return ch == ')';
}

static int associ_ch(char ch)
{
	int ret = 0;
	
	if (ch == '+' || ch == '-')
		ret = 1;
	else if (ch == '*' || ch == '/')
		ret = 2;
	
	return ret;
}

//中缀---》前缀
char *midtopre(const char *mid)
{
	char *pre;
	int i;
	char *ret;
	char top;
	stack_t *result, *tmp;

	pre = malloc(strlen(mid)+1);
	if (NULL == pre)
		return NULL;
	memset(pre, '0', strlen(mid)+1);

	result = stack_init(sizeof(char));
	//if error
	tmp = stack_init(sizeof(char));
	//if error
	
	for (i = strlen(mid)-1; i >= 0; i--) {
		if (is_number(mid[i])) {
			stack_push(result, mid+i);	
		} else if (is_operator(mid[i])) {
			while (1) {
				if(stack_empty(tmp)) {
					stack_push(tmp, mid+i);
					break;
				}
				stack_pop(tmp, &top);	
				if (associ_ch(mid[i]) < associ_ch(top)) {//?
					stack_push(result, &top);	
				} else {
					stack_push(tmp, &top);
					stack_push(tmp, mid+i);
					break;
				} 
			}
		} else if (is_left(mid[i])) {
			while (1) {
				if(stack_empty(tmp))
					break;
				stack_pop(tmp, &top);
				if (top != ')') {
					stack_push(result, &top);
				} else
					break;
			}
		} else if (is_right(mid[i])) {
			stack_push(tmp, mid+i);	
		} else {
			break;	
		}
	}	

	if (i >= 0) 
		ret = NULL;	
	else {
		while (!stack_empty(tmp)) {
			stack_pop(tmp, &top);
			stack_push(result, &top);
		}
		for (i = 0; !stack_empty(result); i++) {
			stack_pop(result, &top);
			pre[i] = top;
		}
		pre[i] = 0;
		ret = pre;
	}
	stack_destroy(tmp);
	stack_destroy(result);

	return ret;
}

int main(int argc, char **argv)
{
	char *res = NULL;

	if (argc < 2)
		return 1;

	res = midtopre(argv[1]);
	printf("%s--->%s\n", argv[1], res);

	return 0;
}

