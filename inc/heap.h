#ifndef _HEAP_H_
#define _HEAP_H_

typedef enum heap_type {

	HEAP_MIN,
	HEAP_MAX
} Heap_Type;

struct heap_data {

	int heapsize;
	int *pointers;

	int maxsize;
	int size;
	int *items;
	double *keys;

	Heap_Type type;
};

int heap_init(struct heap_data *h, int _heap_size, int _max_size, Heap_Type _type);
int heap_insert(struct heap_data *h, int item, int key);
int heap_update(struct heap_data *h, int item, int key);
int heap_downheap(struct heap_data *h, int item, int key);
int heap_upheap(struct heap_data *h, int item, int key);
int heap_extractopt(struct heap_data *h);
int heap_delete(struct heap_data *h, int item);
int heap_print(struct heap_data *h);


/*U_Return heap_reset(Heap *heap);




U_Return heap_free(Heap *heap);
U_Return heap_delete(Heap *heap, int item);
U_Bool heap_is_empty(Heap *heap);
*/

#endif
