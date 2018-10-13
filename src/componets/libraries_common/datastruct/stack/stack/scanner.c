#include <stdio.h>

#include "stack.h"

static int is_left(char ch)
{
	return ch == '(' || ch == '{' || ch == '[' || ch == '<';
}

static int is_right(char ch)
{
	return ch == ')' || ch == '}' || ch == ']' || ch == '>';
}

static int is_match(char left, char right)
{
	switch (left) {
		case '(':
			return !(right == ')');
		case '[':
			return !(right == ']');
		case '{':
			return !(right == '}');
		case '<':
			return !(right == '>');
		default:
			return -1;	
	}
}

static int scanner(const char *str)
{
	stack_t *me = NULL;
	char popch;
	int ret = 0;

	me = stack_init(sizeof(char));
	if (NULL == me) {
		return -1;		
	}

	while (*str) {
		if (is_left(*str)) {
			stack_push(me, str);		
			//if error
		} else if(is_right(*str)) {
			if (stack_pop(me, &popch) < 0) 
				break;
			if (is_match(popch, *str)) 
				break;
		}
	
		str++;	
	}						
	
	if (*str != '\0' || !stack_empty(me)) {
		ret = 1;
	}
	stack_destroy(me);	

	return ret;
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;
	
	if (!scanner(argv[1])) {
		printf("match!!!\n");
	} else {
		printf("no match!!!\n");
	}

	return 0;
}

