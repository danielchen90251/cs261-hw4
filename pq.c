/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Chiu-Chun, Chen
 * Email: chenchiu@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* array;
	int* priority;
	int cap_pq;
};

struct dynarray {
	void** data;
	int size;
	int capacity;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
	pq->array = dynarray_create();
	pq->cap_pq = 8;
	pq->priority = malloc(pq->cap_pq * sizeof(int));
	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	dynarray_free(pq->array);
	free(pq->priority);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	int pqSize = dynarray_size(pq->array);
	if (pqSize == 0){
		return 1;
	}else{
		return 0;
	}
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */

void update_cap_pq(struct pq* pq){
	int cap = pq->cap_pq;
	pq->cap_pq = pq->array->capacity;
	int* new_priority = malloc(pq->cap_pq * sizeof(int));
	for (int i = 0; i < cap; i++){
		new_priority[i] = pq->priority[i];
	}
	free(pq->priority);
	pq->priority = new_priority;
}

int find_parent(int i){
	return (i - 1) / 2;
}

void swap(struct pq* pq, int pos, int posParent){
	int temp = pq->priority[pos];
	pq->priority[pos] = pq->priority[posParent];
	pq->priority[posParent] = temp;

	void* tempValPos = dynarray_get(pq->array, pos);
	void* tempValPosParent = dynarray_get(pq->array, posParent);

	dynarray_set(pq->array, pos, tempValPosParent);
	dynarray_set(pq->array, posParent, tempValPos); 
}

void pq_insert(struct pq* pq, void* value, int priority) {
	dynarray_insert(pq->array, value);
	int pos = dynarray_size(pq->array) - 1;

	if (pq->array->capacity != pq->cap_pq){
		update_cap_pq(pq);
	}
	
	pq->priority[pos] = priority;
	while (pos > 0){
		int posParent = find_parent(pos);
		if (pq->priority[pos] >= pq->priority[posParent]){
			return;
		}
		swap(pq, pos, posParent);
		pos = posParent;
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	if (pq_isempty(pq) == 1){
		return NULL;
	}else{
		return dynarray_get(pq->array, 0);
	}
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	return pq->priority[0];
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */

int find_left_child(int i){
	return i * 2 + 1;
}

int find_right_child(int i){
	return i * 2 + 2;
}

void* pq_remove_first(struct pq* pq) {
	int lastPos = dynarray_size(pq->array) - 1;
	int pos = 0;
	void* temp = dynarray_get(pq->array, 0);

	swap(pq, 0, lastPos);

	dynarray_remove(pq->array, lastPos);
	int pqSize = dynarray_size(pq->array) - 1;

	while (pos <= pqSize){
		int leftChildIndex = find_left_child(pos);
		if (leftChildIndex > pqSize){
			break;
		}
		int rightChildIndex = find_right_child(pos);
		if (pq->priority[pos] > pq->priority[leftChildIndex]){
			if (rightChildIndex < pqSize){
				if (pq->priority[rightChildIndex] < pq->priority[leftChildIndex]){
					swap(pq, pos, rightChildIndex);
					pos = rightChildIndex;
					continue;
				}
			}
			swap(pq, pos, leftChildIndex);
			pos = leftChildIndex;
		}else{
			if (rightChildIndex > pqSize){
				break;
			}
			if (pq->priority[pos] > pq->priority[rightChildIndex]){
				swap(pq, pos, rightChildIndex);
				pos = rightChildIndex;
			}else{
				break;
			}
		}
	}
	return temp;
}
