#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <assert.h>
#include <stdbool.h>





typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} NODE;





typedef struct list {
    int count;
    NODE *head;
    int (*compare)();
} LIST;





// O(1)
// Return a pointer to a new list using compare as its comparison function, which may be NULL.
LIST *createList (int (*compare)()) {
    LIST *pList = malloc (sizeof (LIST));             
    assert (pList != NULL);                          
    
    pList->head = malloc(sizeof(NODE));             
    assert (pList->head != NULL);                    
    
    pList->count = 0;
    pList->compare = compare;                       
    pList->head->next = pList->head;	// point next to head (dummy)
    pList->head->prev = pList->head;    // point prev to head (dummy)
    
    return pList;
}





// O(n)
// Deallocate memory associated with the list pointed to by lp. Do this in reversing order.
void destroyList (LIST *lp) {
    assert (lp != NULL);

    NODE *pDel;                                     
    NODE *pPrev = lp->head->prev;	// start at the tail of list
    
    while (pDel != lp->head) {           // go through everything (stops at dummy)
        pDel = pPrev;                   // begin at tail
        pPrev = pDel->prev;             // then begin moving backwards
        free (pDel);                                 
    }
    
    free (lp);                                       
}





// O(1)
// Return the number of items in the list pointed to by lp.
int numItems (LIST *lp) {
    assert (lp != NULL);
    
    return lp->count;
}





// O(1)
// Add item as the first item in the list pointed to by lp.
void addFirst (LIST *lp, void *item) {
    assert (lp != NULL);
    
    NODE *pAdd = malloc (sizeof (NODE));              
    assert (pAdd != NULL);
    
    pAdd->data = item;
    
    lp->head->next->prev = pAdd;                    
    pAdd->next = lp->head->next;                    
    pAdd->prev = lp->head;                          
    lp->head->next = pAdd;                          
    
    lp->count++;                                    
    
    return;
}





// O(1)
// Add item as the first item in the list pointed to by lp.
void addLast (LIST *lp, void *item) {
    assert(lp != NULL);

    NODE *pAdd = malloc (sizeof (NODE));              
    assert (pAdd != NULL);
    pAdd->data = item;

    lp->head->prev->next = pAdd;	// put pAdd after last element
    pAdd->next = lp->head;                          
    pAdd->prev = lp->head->prev;                    
    lp->head->prev = pAdd;                          
    
    lp->count++;                                    
    
    return;
}





// O(1)
// Remove and return the first item in the list pointed to by lp; the list must not be empty.
void *removeFirst (LIST *lp) {
    assert (lp != NULL);
    
    NODE *pDel = lp->head->next;
    void *temp = pDel->data;
    
    lp->head->next = pDel->next;	// point to first node
    pDel->next->prev = lp->head;                    
    
    free (pDel);                                     
    lp->count--;
    
    return temp;
}





// O(1)
// Remove and return the last item in the list pointed to by lp; the list must not be empty.
void *removeLast (LIST *lp) {
    assert (lp != NULL && lp->count > 0);
    
    NODE *pDel = lp->head->prev;
    void *temp = pDel->data;
    
    lp->head->prev = pDel->prev;
    pDel->prev->next = lp->head;
    
    free (pDel);
    lp->count--;
    
    return temp;
}





// O(1)
// Return, but do not remove, the first item in the list pointed to by lp; the list must not be empty.
void *getFirst (LIST *lp) {
    assert (lp != NULL && lp->count > 0);
    
    return lp->head->next->data;
}





// O(1)
// Return, but do not remove, the last item in the list pointed to by lp ; the list must not be empty.
void *getLast (LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    
    return lp->head->prev->data;
}





// O(n)
// If item is present in the list pointed to by lp then remove it; the comparison function must not be NULL.
void removeItem (LIST *lp, void *item) {
    assert (lp != NULL);
    
    if(lp->count > 0) {
        NODE *pDel = lp->head->next;
   
	while (pDel != lp->head) {				// while we have not go through everything
            if ((*lp->compare) (pDel->data, item) == 0) {		// found it
                pDel->prev->next = pDel->next;
                pDel->next->prev = pDel->prev;
                
		free (pDel);
                lp->count--;
                break;
            }
            else {
                pDel = pDel->next;
            }
        }
    }
    
    return;
}





// O(n)
// If item is present in the list pointed to by lp then return the matching item, otherwise return NULL; the comparison function must not be NULL.
void *findItem (LIST *lp, void *item) {
    assert (lp != NULL);
    
    if (lp->count > 0) {
        NODE *node = lp->head->next;
        while (node != lp->head) {				// while we have not go through everything
            if ((*lp->compare) (node->data, item) == 0)		// found it
                return node->data;
            else
                node = node->next;
        }
    }
    
    return NULL;
}





// O(n)
// Allocate and return an array of items in the list pointed to by lp.
// Return an array containing node->data.
void *getItems (LIST *lp) {
    assert (lp != NULL);
    
    int i = 0;
    void **temp;	// array
    NODE *node = lp->head->next;
    temp = malloc (sizeof (void *) * lp->count);
    assert (temp != NULL);
    
    while(i < lp->count && node != lp->head) {		// while we have not go through everything
        temp [i] = node->data;
        node = node->next;
        i++;
    }
    
    return temp;
}
