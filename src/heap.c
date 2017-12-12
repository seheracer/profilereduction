#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int heap_init(struct heap_data *heap, int _heap_size, int _max_size, Heap_Type _type) {

	if(_heap_size < 0 || _max_size < 0) {
	
		printf("Error: heap_init: improper parameter value\n");
		return 0;
	}


	heap -> heapsize = _heap_size;
	heap -> pointers = (int *)calloc(sizeof(int), _heap_size);

	int i;
	for(i = 0; i < _heap_size; i++)
		heap->pointers[i] = -1;

	heap -> maxsize = _max_size;
	heap -> items = (int *)calloc(sizeof(int), _max_size);
	heap -> keys = (double *)calloc(sizeof(double), _max_size);
	heap -> size = 0;

	heap -> type = _type;
	
	return 1;
}

int heap_insert(struct heap_data *heap, int item, int key) {

	if(!heap) {

		printf("Error: heap_insert: null parameter\n");
		return 0;
	}

	if(item < 0 || item >= heap->heapsize) {

		printf("Error: heap_insert: improper parameter value\n");
		return 0;
	}

	if(heap -> pointers[item] != -1) {
		
		if(!heap_update(heap, item, key))
			return 0;

		return 1;
	}

	if(heap -> size == heap -> maxsize) {

		printf("Error: heap_insert: capacity limit reached\n");
		return 0;
	}

	heap -> items[heap -> size] = item;
	heap -> keys[heap -> size] = key;
	heap -> pointers[item] = heap->size;
	heap -> size ++;

	if(!heap_upheap(heap, item, key))
		return 0;
		
	return 1;
}

int heap_update(struct heap_data *heap, int item, int key) {

	if(!heap) {

		printf("Error: heap_update: null parameter\n");
		return 0;
	}

	if(item < 0 || item >= heap->heapsize) {

		printf("Error: heap_update: improper parameter value\n");
		return 0;
	}

	if(heap->pointers[item] == -1) {
	
		printf("Error: heap_update: item not in heap %d\n", item);
		return 0;	
	}
	
	if(!heap_downheap(heap, item, key))
		return 0;

	if(!heap_upheap(heap, item, key))
		return 0;

	return 1;
}

int heap_downheap(struct heap_data *heap, int item, int key) {

	if(!heap) {

		printf("Error: heap_downheap: null parameter\n");
		return 0;
	}

	if(heap->heapsize && (item < 0 || item >= heap->heapsize)) {

		printf("Error: heap_downheap: improper parameter value\n");
		return 0;
	}

	if(heap->heapsize && heap->pointers[item] == -1) {
	
		printf("Error: heap_downheap: item not in heap\n");
		return 0;	
	}


	int sign = (heap -> type == HEAP_MAX)? 1: -1;
		
	int i = heap -> pointers[item];	
	while(2*i+1 < heap->size) {
	
		int m = 2*i+1;
		if(heap->size > 2*i+2 
			&& sign*heap->keys[2*i+1] < sign*heap->keys[2*i+2])
			m = 2*i+2;
	
		if(sign * key < sign * heap -> keys[m]) {
			
			heap -> items[i] = heap -> items[m];
			heap -> keys[i] = heap -> keys[m];
			if(heap->heapsize)
				heap -> pointers[heap -> items[i]] = i;
			i = m;
		}else
			break;
	}

	heap -> items[i] = item;
	heap -> keys[i] = key;
	if(heap->heapsize)
		heap -> pointers[heap -> items[i]] = i;
	
	return 1;
}

int heap_upheap(struct heap_data *heap, int item, int key) {

	if(!heap) {

		printf("Error: heap_upheap: null parameter\n");
		return 0;
	}

	if(heap->heapsize && (item < 0 || item >= heap->heapsize)) {

		printf("Error: heap_upheap: improper parameter value\n");
		return 0;
	}

	if(heap->heapsize && heap->pointers[item] == -1) {
	
		printf("Error: heap_upheap: item not in heap\n");
		return 0;	
	}


	int sign = (heap -> type == HEAP_MAX)? 1: -1;
	
	int i = heap -> pointers[item];
	while(i > 0) {

		int m = (i-1)/2;
		if(sign * key > sign * heap -> keys[m]) {
			
			heap -> items[i] = heap -> items[m];
			heap -> keys[i] = heap -> keys[m];
			heap -> pointers[heap -> items[i]] = i;
			i = m;
		}else
			break;
	}

	heap -> items[i] = item;
	heap -> keys[i] = key;
	heap -> pointers[heap -> items[i]] = i;
	
	return 1;
}

int heap_extractopt(struct heap_data *heap) {

	if(!heap) {

		printf("Error: heap_extractopt: null parameter\n");
		return -1;
	}

	if(!heap->size) {

		printf("Error: heap_extractopt: heap is empty\n");
		return -1;
	}

	int item = heap -> items[0];
	heap -> pointers[item] = -1;
	heap -> size --;
	if(!heap->size)
		return item;

	heap -> items[0] = heap -> items[heap -> size];
	heap -> keys[0] = heap -> keys[heap -> size];
	heap -> pointers[heap -> items[heap -> size]] = 0;

	if(!heap_downheap(heap, heap->items[0], heap->keys[0]))
		return -1;

	return item;
}

int heap_delete(struct heap_data *heap, int item) {

	if(!heap) {

		printf("Error: heap_delete: null parameter\n");
		return 0;
	}

	if(item < 0 || item >= heap->heapsize) {

		printf("Error: heap_delete: improper parameter value\n");
		return 0;
	}
		
	if(heap -> pointers[item] == -1) {
	
		printf("Error: heap_delete: item not in heap\n");
		return 0;
	}
		
	int i = heap -> pointers[item];
	heap -> pointers[item] = -1;
	
	heap -> size --;
	if(heap -> size == i)
		return 1;
		
	heap -> items[i] = heap -> items[heap->size];
	heap -> keys[i] = heap -> keys[heap->size];
	heap -> pointers[heap -> items[i]] = i;
	
	if(!heap_update(heap, heap->items[i], heap->keys[i]))
		return 0;
		
	return 1;
}

int heap_print(struct heap_data *heap) {

	if(!heap) {

		printf("Error: heap_print: null parameter\n");
		return 0;
	}

	int i;
	for(i=0; i<10; i++)
		printf("%2d ", heap->pointers[i]+1);
	printf("\n");

	for(i=0; i<10; i++)
		printf("(%2d,%.2lf) ", heap->items[i]+1, heap->keys[i]);
	//for(i=heap->size; i<heap->maxsize; i++)
	//	printf("- ");
	printf("\n");

	printf("\n");
	
	return 1;
}

/*
U_Return heap_reset(Heap *heap) {

	if(!heap) {

		printf("Error: heap_reset: null parameter\n");
		return U_FAILURE;
	}

	heap -> size = 0;
	U_VEC_SET(heap->pointers, heap -> heapsize, -1);

	return U_SUCCESS;
}





U_Return heap_free(Heap *heap) {

	if(!heap) {

		printf("Error: heap_free: null parameter\n");
		return U_FAILURE;
	}

	free(heap -> pointers);
	free(heap -> items);
	free(heap -> keys);
	free(heap);
	
	return U_SUCCESS;
}



U_Bool heap_is_empty(Heap *heap) {

	if(!heap) {

		printf("Error: heap_is_empty: null parameter\n");
		return U_TRUE;
	}

	return heap->size? U_FALSE: U_TRUE;
}*/
