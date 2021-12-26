//Project 2 - Sets and Arrays
//Sicheng Fei



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"



struct set {
    int count;    //Number of elements.
    int length;   //Max number of elements.
    char **data;  //Double pointer. First pointer points to the set (sp), second pointer points to the array (sp->data).
};



//Search for words in the array. If not found, return -1.
//Time Complexity: O(n).
static int search (SET *sp, char *target) {
	int i;
	for (i = 0; i < sp->count; i++) {
		if (strcmp (sp->data [i], target) == 0)		//string compare
			return i;
	}
	return -1;
}



//Return a pointer to a new set with a maximum capacity of maxElts.
//Time Complexity: O(1).
SET *createSet (int maxElts) {
    SET *sp;        //points to junk
    sp = malloc (sizeof (sp));
    assert (sp != NULL);
    sp->count = 0;
    sp->length = maxElts;
    sp->data = malloc (sizeof (char *) * maxElts);	//Memory allocate. Allocate the size of a character pointer multiply by the maxElts. Return the pointer to that memory.
    assert (sp->data != NULL);
    return sp;
}



//Deallocate memory associated with the set (sp).
//Deallocate in the reversed order: elements in the array -> array -> set.
//Time Complexity: O(n).
void destroySet (SET *sp) {
    assert (sp != NULL);

    int i;
    for (i = 0; i < sp->count; i++)
        free (sp->data [i]);

    free (sp->data);
    free (sp);
}



//Return the number of elements in the set (sp).
//Time Complexity: O(1).
int numElements (SET *sp) {
    assert (sp != NULL);
    return sp->count;
}



//Add an element to the set (sp).
//Time Complexity: O(n).
void addElement (SET *sp, char *elt) {
	assert (sp != NULL);
	assert (elt != NULL);

	if (search(sp, elt) == -1) {	//Do not add an element when there is the same already in the set, since set can not have duplicate.
		assert (sp->count < sp->length);

		char *new = strdup (elt);	//String duplicate.
		assert (new != NULL);
		sp->data [sp->count] = new;
		sp->count ++;
	}
}



//Remove an element from the set (sp).
//Time Complexity: O(n).
void removeElement (SET *sp, char *elt) {
	assert (sp != NULL);
	assert (elt != NULL);

	int i = search (sp, elt);
	if (i != -1) {	//Element not found.
		free (sp->data[i]);
		sp->count --;
		sp->data [i] = sp->data [sp->count];
	}
}



//If an element is present in the set (sp) then return the matching element, otherwise return null.
//Time Complexity: O(n).
char *findElement (SET *sp, char *elt) {
	assert (sp != NULL);
	assert (elt != NULL);

	if (search(sp, elt) == -1) {	//Element not found.
		assert (sp->count < sp->length);
		return NULL;
	}

	return sp->data [search (sp, elt)];
}



//Allocate and return an array of elements in the set pointed to by sp.
//Time Complexity: O(n).
char **getElements (SET *sp) {
	assert (sp != NULL);

	void *p = sp;
	p = malloc (sizeof (char *) * sp->count);	//Memory allocate. Allocate the size of a character pointer multiply by the size of the array. Return the pointer to that memory.
	assert (p != NULL);
	return memcpy (p, sp->data, sizeof (char *) * sp->count);	// O(n). Memory copy. Copy the array to the newly allocated memory with the current array size.
}
