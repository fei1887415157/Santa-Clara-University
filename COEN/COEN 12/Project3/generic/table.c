// Project 3 - Hash Table and Generic ADT
// generic
// Sicheng Fei



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"
#define EMPTY 0		// Can use either EMPTY or 0 to compare.
#define FILLED 1	// ''
#define DELETED 2	// ''



struct set {

    int count;    // Number of elements.
    int length;   // Max number of elements.
    void **data;  // Double pointer. First pointer points to the set (sp), second pointer points to the array (sp->data).
    char *flags;	  // Array of flags: EMPTY, FILLED, DELETED.
    int (*compare)();		// Function pointer. Need parentheses.
    unsigned (*hash)();	// Function pointer. Need parentheses.
};





//Linear probing.
//Search for hashes in the table. Change boolean found that is passed in depends on found or not.
//Time Complexity: O(n).
static int Search (SET *sp, void *target, bool *found) {

	
	//StrHash may output number bigger than sp->length, therefore we need to mod it.
	int hash = (*sp->hash)(target) % sp->length;
	
	int a = 0;
	int i = 0;
	int deletedI = -1;
	
	while (a < sp->length) {
	
		i = (hash + a) % sp->length;

		if (sp->flags[i] == EMPTY) {	// Slot is empty.
			*found = false;
			
			if (deletedI != -1)
				return deletedI;
				
			else
				return i;
		}
		
		else if (sp->flags [i] == FILLED) {	// Slot is filled.
		
			if ((*sp->compare)(sp->data [i], target) == 0) {
				*found = true;
				return i;
			}
			
			else
				a ++;
		}
		
		else if (sp->flags [i] == DELETED) {	// Slot is deleted.
			
			if (deletedI == -1)
				deletedI = i;
				
			a ++;
		}
	
	}
	
	exit(-1);	// Impossible happens: sp->flags are not EMPTY or FILLED or DELETED.

}



// Return a pointer to a new set with a maximum capacity of maxElts.
// Time Complexity: O(n).
SET *createSet (int maxElts, int (*compare)(), unsigned (*hash)()) {

    SET *sp;        // Points to junk.

    sp = malloc (sizeof (SET));
    assert (sp != NULL);
	assert (compare != NULL);
	assert (hash != NULL);

    
    sp->count = 0;
    sp->length = maxElts;
    sp->data = malloc (sizeof (void *) * maxElts);	// Memory allocate. Allocate the size of a void pointer multiply by the maxElts. Return the pointer to that memory.
    sp->flags = malloc (sizeof (char) * maxElts);
    

    // Assign pointers of functions passed in to the set created here.
    sp->compare = compare;
    sp->hash = hash;

    
    int i;
    for (i = 0; i < sp->length; i++){
    	sp->flags [i] = EMPTY;
    }


    assert (sp->data != NULL);
    return sp;
}



//Deallocate memory associated with the set (sp).
//Deallocate in the reversed order: elements in the array -> array -> set.
//Time Complexity: O(1).
void destroySet (SET *sp) {

    assert (sp != NULL);
	
	free (sp->flags);
    free (sp->data);
    free (sp);
}



//Return the number of elements in the set (sp).
//Time Complexity: O(1).
int numElements (SET *sp) {

    assert (sp != NULL);
    return sp->count;
}



//Add an element to the set (sp). Does not allocate new memory.
//Time Complexity: O(n).
void addElement (SET *sp, void *elt) {

	assert (sp != NULL);
	assert (elt != NULL);

	bool found;
	int index = Search(sp, elt, &found);

	if (!found) {	//Do not add an element when there is the same already in the set, since set can not have duplicate. Pass by reference.
		assert (sp->count < sp->length);



		sp->data [index] = elt;
		sp->flags [index] = FILLED;
		sp->count ++;	//Increment.
		
	}
}



//Remove an element from the set (sp).
//Time Complexity: O(n).
void removeElement (SET *sp, void *elt) {

	assert (sp != NULL);
	assert (elt != NULL);

	bool found;
	int index;
	index = Search(sp, elt, &found);

	if (found) {
		
		sp->count --;	//Decrement.
		sp->flags [index] = DELETED;
	}
}



//If an element is present in the set (sp) then return the matching element, otherwise return null.
//Time Complexity: O(n).
void *findElement (SET *sp, void *elt) {

	assert (sp != NULL);
	assert (elt != NULL);

	bool found;
	int index = Search(sp, elt, &found);
	if (!found) {	//Element not found.
		//assert (sp->count < sp->length);
		return NULL;
	}

	return sp->data [index];
}



//Allocate and return an array of elements in the set pointed to by sp.
//Time Complexity: O(n).
void *getElements (SET *sp) {

	assert (sp != NULL);

	
	void **arr;		// Store filled elements.
	arr = malloc (sizeof (void *) * sp->count);		// Memory allocate. Allocate the size of a void pointer multiply by the size of the array. Return the pointer to that memory.
	assert (arr != NULL);
	
	int arrI = 0;
	int i;
	// Only add element when they are filled.
	for (i = 0; i < sp->length; i++) {
		if (sp->flags [i] == FILLED) {
			arr [arrI] = sp->data [i];
			arrI ++;
		}
		
	}
	
	return arr;
}
