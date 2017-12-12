#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int stack_init(struct stack_data *st, int _max_size) {

	if( _max_size < 0) {
	
		printf("Error: stack_init: improper parameter value\n");
		return 0;
	}


	st -> size = 0;
	st -> items = (int *)calloc(sizeof(int), _max_size);

	st -> maxsize = _max_size;

	
	return 1;
}

int stack_push(struct stack_data *st, int item)
{
	st->items[st->size] = item;
	st->size++;

	if(st->size > st->maxsize)
	{
		printf("Error: stack_push: maxsize reached\n");
		return 0;
	}
	
	return 1;
}

int stack_pop(struct stack_data *st)
{
	st->size--;
	if(st->size < 0)
	{
		printf("Error: stack_pop: negative stack size\n");
		return -1;
	}
	return st->items[st->size];

}


