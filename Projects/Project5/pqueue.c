// Project 5, week 1 - Being up a Tree, You're in a Heap of Trouble



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"
#define Initial_Length 10



struct pqueue {
	int count, length;
	void **data;	// Pointer to the array.
	int (*compare) ();
};



// O(1).
// Return a pointer to a new priority queue using compare as its comparison function.
PQ *createQueue(int (*compare)()) {

	assert (compare != NULL);

	PQ *pq = malloc (sizeof (PQ));             
    assert (pq != NULL);                                           
    
    pq->length = Initial_Length;
    pq->data = malloc (sizeof (void *) * pq->length); // allocate memory for data pointer
    assert (pq->data != NULL);
    
    pq->compare = compare;
	pq->count = 0;   
	
	return pq;
}



// O(n).
// Deallocate memory associated with the priority queue pointed to by pq.
void destroyQueue(PQ *pq) {

    assert (pq != NULL);
    
    free (pq->data); 
    free (pq);
    
    return;
}



// O(1).
// Return the number of entries in the priority queue pointed to by pq.
int numEntries(PQ *pq) {

	assert (pq != NULL);
	
	return pq->count;
}



// O(log n).
// Min-Heap: If entry is higher than parent, swap them.
// Add entry to the priority queue pointed to by pq.
void addEntry(PQ *pq, void *entry) {

	/*
	Parent = (i - 1) / 2)
	Left child = i * 2 + 1
	Right child = i * 2 + 2
	*/
	
	assert(pq != NULL);
	
    if(pq->count == pq->length) {                   // If we reach the length cap
        pq->length *= 2;                            // Double the array length and reallocated memory
       // printf("pq->length: %d\n", pq->length);
        pq->data = realloc (pq->data, sizeof (void *) * pq->length);
        assert (pq->data != NULL);
    }
    
    pq->data [pq->count] = entry;                    // Copy data we want to input
    int child = pq->count;                          // Set node at child
    int parent = (pq->count - 1) / 2;                   // Find parent of child
    while ((*pq->compare) (pq->data [child], pq->data [parent]) == -1) { // Compare the value of the child to the parent
        void *temp = pq->data[parent];              // We want to save the parent's data when we swap
        pq->data[parent] = pq->data[child];         // Child's data -> Parent's data
        pq->data[child] = temp;                     // Child's data -> Parent's data (from temp)
        child = parent;                             // Then swap the indices
        parent = (parent-1)/2;                      // Keep doing this until we hit root
    }
    
    pq->count++;                                    
	
	return;
}





// O(log n).
// Min_Heap: Replace the root (smallest value) (first element in the array) with the far right node (last element in the array). Keep swapping the root with the smallest children.
// Remove and return the smallest entry from the priority queue pointed to by pq.
void *removeEntry(PQ *pq) {

	
	/*	
		
	
	
	pq->data [0] = pq->data [count - 1];
	void *child = pq->data [0];
	
	while (true) {
	if (pq->data [i * 2 + 1] < pq->data [i * 2 + 2]) {
		new_root = pq->data [i * 2 + 1];
		pq->data [i] = pq->data [(i - 1) / 2];;
		i = (i - 1) / 2;
		pq->data [i] = entry;
	} else {
		new_root = pq->data [i * 2 + 2];
		pq->data [i] = pq->data [(i - 1) / 2];;
		i = (i - 1) / 2;
		pq->data [i] = entry;
	}
	*/
	
	
	/*
	Parent = (i - 1) / 2)
	Left child = i * 2 + 1
	Right child = i * 2 + 2
	*/

	assert(pq != NULL);    

    PQ *pDel; //malloc(sizeof(PQ));                  // Make pDel our struct to delete
    //assert(pDel != NULL);
    
    pDel = pq->data [0];                             // We will always want to delete first
    pq->data [0] = pq->data [pq->count-1];            // Go to the last node in the tree
    int index = 0;                                  // Start at the root
    while((index*2)+1 <= pq->count-1) {             // While the index is not at/past the last node 
        int child = (index*2+1);
        if((index*2+2) <= pq->count-1) {            // If the right child exists
            if((*pq->compare)(pq->data[index*2+2], pq->data[index*2+1]) < 0) {  // If the right child is smaller than left child
                child = index*2+2;                  // Then use right child as main
            }
        }
        if((*pq->compare)(pq->data[index], pq->data[child]) < 0) { // If the parent is smaller than the children, break
            break;
        }
        else {                                      // Else (left child is smaller than parent)
            void *temp = pq->data[index];           // Save parent data into temp
            pq->data[index] = pq->data[child];      // Child's data -> Parent's data
            pq->data[child] = temp;                 // Parent's data -> Child's data
        }
        index = child;                              // Then continue down the tree
    }
    
    pq->count--;                                    
    
    return pDel;                                    
	
}

