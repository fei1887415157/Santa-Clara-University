/*
        Term Project - Loony Lists

        Variable array size - doubles every node (size = initial size * 2^list->count. Because O(log n) instead of O(n).
        Search for an item at a given index from either first or last node in the list.

        Data Structure:
        Doubly-linked list with head and tail pointer non circular. Each node in the list holds a circular queue. We can add to the array even if last index is full, since there may be spots open in the front.
*/





#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"





/*
	Circular queue.
	FIFO: First in, first out.
*/

typedef struct node {

	int slots;	// total number of slots
	int fill;	// filled slots
	NODE *next, *prev;
	int first; 	// index of the first element in the queue
	int last; 	// index of the last element in the queue
	void **data; 	// circular queue containing generic data

} NODE;





/*
	Doubly linked list with head and tail pointer non circular.
*/

typedef struct list {

	int totalItems; // total number of elements in all of the nodes
	int count; 	// number of nodes in the list
	NODE *head, *tail;

} LIST;





/*
	O(1)
	Return a pointer to a new list.
*/

LIST *createList(void){

	LIST *lp =  malloc(sizeof(LIST));
	assert(lp != NULL);

	lp->totalItems = 0;
	lp->count = 0;

	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	
	lp->head->next = NULL;
	lp->head->prev = NULL;	
	
	lp->head->first = 0;
	lp->head->last = 0;
	
	lp->head->fill = 0;
	lp->head->slots = 4;

	// allocate a void* array with as many indices as their are "slots"
	lp->head->data = malloc(sizeof(void*) * 4);
	lp->tail = lp->head;
	
	return lp;

}





/*
	O(log n) because the number of nodes grows logarithmically
	Deallocate memory associated with the list pointed by lp.
*/

void destroyList(LIST *lp){

	assert(lp != NULL);
	NODE *pCur = lp->head;
	NODE *pDel;	

	// iterate through the linked list and deallocate nodes
	while(pCur != NULL){
		pDel = pCur->next;
		free(pCur->data);
		free(pCur);
		pCur = pDel;
	}

	free(lp);
	
	return;

}





/*
	O(1)
	Return the number of items in the list pointed by lp.
*/

int numItems(LIST *lp){
	
	assert (lp != NULL);
	
	return lp->totalItems;	

}





/*
	O(1)
	Add item as the first item in the list pointed to by lp.
	first node = head
	1. Go to the first node in the linked list.
	2. If the array in first node is full, then allocate a new node with an empty array and make it the new first node.
	3. Add the item to the front of the array in the first node.
*/

void addFirst(LIST *lp, void *item){

	assert(lp != NULL);

	// if the head node is full then allocate a new node to accomodate new data
	if((lp->head->fill) >= (lp->head->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		// put the new node in the linked list by reassigning pointers
		newNode->next = lp->head;
		newNode->prev = NULL;
		lp->head->prev = newNode;

		// initialize the number of slots for the new node
		newNode->fill = 0;
		int newSlots = 2 * lp->head->slots;
	
		// set the newNode as the head of the linked list
		lp->head = lp->head->prev;

		// update slots
		lp->head->slots = newSlots;

		// allocate an array of void* twice the size
		lp->head->data = malloc(sizeof(void*) * (newSlots));
		assert(lp->head-> data != NULL);

		// increment the counter of the number of nodes
		lp->count ++;

		lp->head->first = 0;
	
	}

	// make sure there is still room in the queue to add an item (count<length)
	assert((lp->head->fill) < (lp->head->slots));

	// decrease the index of the "first index"
	lp->head->first--;
	if(lp->head->first < 0)
		lp->head->first = lp->head->slots - 1;

	// put item in the first index slot in the head node
	lp->head->data[lp->head->first] = item;
	
	lp->head->fill ++;
	lp->totalItems ++;

	return;

}





/*
        O(1)
        Add item as the last item in the list pointed to by lp.
	last node = tail
        1. Go to the last node in the linked list.
        2. If the array in last node is full, then allocate a new node with an empty array and make it the new last node.
        3. Add the item to the end of the array in the last node.
*/

void addLast(LIST *lp, void *item){

	assert(lp != NULL);
	
	int index;


	// if the tail node is full then allocate a new node to accomodate new data
	if((lp->tail->fill) >= (lp->tail->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		// put the new node in the linked list by reassigning pointers
		newNode->next = NULL;
		newNode->prev = lp->tail;
		lp->tail->next = newNode;

		// initialize the number of slots for the new node
		newNode->fill = 0;
		int newSlots = 2 * lp->tail->slots;

		// set the newNode as the tail of the linked list
		lp->tail = lp->tail->next;

		// update slots
		lp->tail->slots = newSlots;
	
		// allocate an array of void* twice the size of the array in the node to the right
		lp->tail->data = malloc(sizeof(void*) * (newSlots));
		assert(lp->tail->data != NULL);

		lp->count ++;
		lp->tail->first = 0;
	
	}

	// make sure there is still room in the queue to add an item (count<length)
	assert((lp->tail->fill) < (lp->tail->slots));

	// find the index where the last element should go
	index = lp->tail->first + lp->tail->fill;
	if(index >= lp->tail->slots)
		index = index - lp->tail->slots;

	

	// insert the item
	lp->tail->data[index] = item;
	
	lp->tail->fill ++;
	lp->totalItems ++;

	// update the "last" index number	
	lp->tail->last = (lp->tail->first + lp->tail->fill) % lp->tail->slots;

	return;

}





/*
	O(1)
	Remove and return the first item in the list pointed to by lp; the list must not be empty.
	first node = head
	1. Go to the first node in the linked list.
	2. If the array in the first node is empty, then deallocate the node and make the next node the new first node.
	3. Remove the item from the front of the array in the first node.
*/

void *removeFirst(LIST *lp){

	assert(lp != NULL);

	// check that there is an item to remove and return
	assert(lp->totalItems > 0);

	// remove the head node if it is empty	
	if(lp->head->fill == 0){

		NODE *delNode = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(delNode->data);
		free(delNode);
	
	}

	// store the first value in a temporary variable
	void* temp = lp->head->data[lp->head->first];
	
	// update the "first" index
	if(++lp->head->first >= lp->head->slots)
		lp->head->first = 0;

	lp->totalItems --;
	lp->head->fill --;

	return temp;

}





/*
        O(1)
        Remove and return the first item in the list pointed to by lp; the list must not be empty.
        last node = tail
	1. Go to the last node in the linked list.
        2. If the array in the last node is empty, then deallocate the node and make the next node the new last node.
        3. Remove the item from the back of the array in the last node.
*/

void *removeLast(LIST *lp){

	assert(lp != NULL);
	
	// check that there is an item to remove and return
	assert(lp->totalItems > 0);

	// remove the tail node if it is no longer needed
	if(lp->tail->fill == 0){
		
		NODE *delNode = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(delNode->data);
		free(delNode);
	
	}

	// create a temporary value to store the last element
	void *temp = lp->tail->data[(lp->tail->fill + lp->tail->first -1) % lp->tail->slots];

	if(--lp->tail->last < 0)
		lp->tail->last = lp->tail->slots - 1;

	lp->totalItems --;
	lp->tail->fill --;

	return temp;		

}





/*
	worst case O(log n) runtime if the index we are looking for is at the end of the list we will have to iterate through the entire list but the nodes have varying size
	Return the item at position index in the list pointed to by lp (the index must be within range).
*/

void *getItem(LIST *lp, int index){

	assert(lp != NULL);
	assert(index >= 0);
	assert(lp->totalItems > 0);
	
	// make sure the index is less than the total number of elements
	assert(index < lp->totalItems);

	// traverse linked list looking for the node that has the
	// array with the index that we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->fill){

		index = index - pCur->fill;
		pCur = pCur->next;
	
	}
	
	return pCur->data[(pCur->first + index) % pCur->slots];

}





/*
	worst case O(log n) runtime because we may have to iterate through the whole list but as the list grows the number of nodes increases logarithmically
	Update the item at position index in the list pointed by lp (the index must be within range).
*/

void setItem(LIST *lp, int index, void *item){

	assert(lp != NULL);	

	// make sure the index is less than the total number of elements
	assert(index >= 0 && index < lp->totalItems);

	// like above, traverse the linked list looking for the node that has the array with the index we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->fill){

		index = index - (pCur->fill);
		pCur = pCur->next;
	
	}

	// update the item at the given index
	pCur->data[(pCur->first + index) % pCur->slots] = item;

	return;

}
