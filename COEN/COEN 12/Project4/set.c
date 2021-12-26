#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include <assert.h>
#include <stdbool.h>
#include "list.h"
#define alpha 20





static int search(SET *sp, void *elt, bool *found);





typedef struct set {
    LIST **data;	// Pointer to the array.
    int length;
    int count;
    int (*compare)();
    unsigned (*hash)();
} SET;





// O(n)
SET *createSet (int maxElts, int (*compare)(), unsigned (*hash)()) {
    
    SET *sp = malloc (sizeof(SET));                  
    assert (sp != NULL);                             
    
    sp->length = maxElts / alpha;                   // set max elements
    sp->count = 0;                                  
    sp->compare = compare;                          
    sp->hash = hash;                                
    
    sp->data = malloc (sizeof (LIST *) * sp->length); // allocate memory for data pointer
    assert (sp->data != NULL);                       
    
    int i;
    for(i = 0; i < sp->length; i++)                  // make a list
        sp->data [i] = createList (sp->compare);     // each data point
    
    return sp;
}





// O(n)
// Destroy in reverse order.
void destroySet (SET *sp) {
    
    assert (sp != NULL);
    
    int i;
    for (i = 0; i < sp->length; i++)                 
        destroyList (sp->data [i]);               
    
    free (sp->data);                                 
    free (sp);                                       
    
    return;
}





// O(1)
int numElements (SET *sp) {
    
    assert (sp != NULL);
    
    return sp->count;
}





// O(n)
void addElement (SET *sp, void *elt) {
    
    assert (sp != NULL && elt != NULL);
    
    bool found = false;
    int index = search (sp, elt, &found);           
    
    if (!found) {	                            
        addLast (sp->data [index], elt);            // add to end of the list
        sp->count++;                                
    }
    
    return;
}





//O (n)
void removeElement (SET *sp, void *elt) {
    
    assert (sp != NULL && elt != NULL);
    
    bool found = false;
    int index = search (sp, elt, &found);           
    
    if (!found) 		                    
        return;                                     
    else {
        removeItem (sp->data [index], elt);         // if found, remove it
        sp->count--;                                
	return;
    }
}





// O(n^2)
void *findElement (SET *sp, void *elt) {
    
    assert (sp != NULL && elt != NULL);
    
    bool found = false;
    int index = search (sp, elt, &found);           
    
    if (!found)                             	    
        return NULL;                                
    else                                            
        return findItem (sp->data [index], elt);     // return element
}





// O(n^3)
void *getElements (SET *sp) {
    
    assert (sp != NULL);
    
    void **temp;
    void **list;
    int i, j, counter = 0;
    temp = malloc (sizeof(void *) * sp->count);
    assert (temp != NULL);
    
    for(i = 0; i < sp->length; i++) {                            // loop through everything
        list = malloc (sizeof (void *) * numItems (sp->data [i]));  // create a secondary list array
        list = getItems (sp->data [i]);                           // call get elements on everything
        for(j = 0; j < numItems (sp->data [i]); j++) {            // loop through list size
            temp [counter] = list [j];                           // copy elements in list
            counter++;                                           // to the temp array
        }
    }
    
    return temp;                                                 // return copied array
}





// O(n^2)
static int search (SET *sp, void *elt, bool *found) {
    
    assert(sp != NULL && elt != NULL);
    
    int index = (*sp->hash) (elt) % sp->length;                  // use hash function for max size
    
    if(findItem (sp->data [index], elt) != NULL)
        *found = true;                                              
    else
        *found = false;                                         
        
    return index;
}
