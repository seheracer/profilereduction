#ifndef _STACK_H_
#define _STACK_H_

struct stack_data {

	int size;
	int maxsize;

	int *items;

};

int stack_init(struct stack_data *st, int _max_size);
int stack_push(struct stack_data *st, int item);
int stack_pop(struct stack_data *st);


#endif
