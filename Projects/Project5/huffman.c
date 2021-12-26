// Project 5: Being up a Tree, You’re in a Heap of Trouble
// Week 2



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pqueue.h"
#include "pack.h"
#include <ctype.h>
#include <assert.h>

#define NODE struct node



// O(1).
// Set the parent of the left and right node passed in equal to the newly created node.
static NODE *MakeNode (int count, NODE *left, NODE *right) {

    NODE *root = malloc(sizeof(NODE));                          
    assert(root != NULL);     

    root->count = count;                                        
    root->parent = NULL;		// Since this is root, no parent needed.
    
    if(left != NULL)			// If there is a left child, set root as its parent.
        left->parent = root;

    if(right != NULL)			// If there is a right child, set root as its parent.
        right->parent = root;
        
    return root;
}



// O(1).
// Compare between two leaves.
static int NodeCompare (NODE *node1, NODE *node2) {

    assert(node1 != NULL && node2 != NULL);

	if (node1->count < node2->count)
		return -1;
	else if (node1->count > node2->count)
		return 1;
	else
		return 0;
}
//return (node1->count < node2->count) ? -1: (node1->count > node2->count);		// This ternary conditional operator checks if node1 or node2 is larger.



// O(h).
static int Depth (NODE *node) {

    if(node->parent == NULL)				
        return 0;                           
    else
        return 1 + Depth (node->parent);    // Recursion: 1 + depth of parent (until hits root).
}



/*    Main function that needs five basic steps:

	1. Count the number of occurrences of each character in the file. Keep track of these counts in a count array.

	2. Create a binary tree consisting of just a leaf for each character with a nonzero count. The data for the tree is the character’s count. Also, create a tree with a zero count for the end of file marker. Insert all trees into a priority queue, and also store each node in an array similar to the count array.

	3. While the priority queue has more than one tree in it, remove the two lowest priority trees. Create a new tree with each of these trees as subtrees. The count for the new tree is the sum of the counts of the two subtrees. Insert the new tree into the priority queue.

	4. Eventually, there will be only one tree remaining in the priority queue. This is the Huffman tree. Incidentally,the data at the root of this tree should equal the number of characters in the file.

	5. Print out information about all the characters and call pack().
*/

int main (int argc, char *argv[]) {

    // Step 1.
    int ascii [257];                                 // Count array. A 257 size array for all printable ascii characters.
    NODE *leaves [257];                              // Initialize an array for each possible ascii character.
    int i, j, k, l;

    for (i = 0; i < 257; i++) {                      
        ascii [i] = 0;                               
        leaves [i] = NULL;                           
    }

    if (argc != 3) {                                 // Check if user input two text files input (existing), output (new one).
        printf ("Need two file names, input: existing, output: new one. \n");
        return 0;
    }

    FILE *fp = fopen (argv [1], "rb");       // Open the input file on read-binary.
    assert (fp != NULL);
    while ((j = getc (fp)) != EOF) {         // While it is not EndOfFile.
        ascii [j] ++;                        // Increment number of occurrences of j.
    }
    fclose (fp);                             // Close file pointer.



    // Step 2.
    PQ *pq = createQueue (*NodeCompare);                             
    for (k = 0; k < 257; k++) {                                 	 
        if(ascii [k] > 0) {                                    		 // If there are nonzero amount of that character.
            leaves [k] = MakeNode (ascii [k], NULL, NULL);           // Create a node for that character.
            addEntry (pq, leaves [k]);                               // Add it into the leaves[] array.
        }
    }

    NODE *endfile = malloc (sizeof (NODE));		// Keep one counter for the EOF character.
    assert (endfile != NULL);

    endfile->count = 0;                         
    endfile->parent = NULL;
    leaves [256] = endfile;
    addEntry (pq, leaves [256]);	// Add it as the last leaf.



    // Step 3.
    while (numEntries(pq) > 1) {                                		     // Now looking at the priority queue we've made through addEntry function calls.
        NODE *left = removeEntry (pq);                         			     // "Connect" leaves of the same size.
        NODE *right = removeEntry (pq);                           			 // Choose a left node and right node.
        NODE *parent = MakeNode (left->count + right->count, left, right);   // And add them together and sum the occurrence count.
        addEntry (pq, parent);                                  			 // New combined parent.
    }



    // Step 4.
    int ndepth, counter;	// Depth and bit counter.
    for(l = 0; l < 257; l++) {		// Loop through the whole array.
        if(leaves[l] != NULL) {		// And check if the leaf is not empty.
            ndepth = Depth (leaves [l]);	// Find the depth on a leaf with depth().
            counter = ndepth * leaves [l]->count;	// Counter = depth * # of occurrences.
            
			if (isprint (l))	// Can we print l ?
				printf ("%c", l);	// Print l as an English character.
			else
				printf ("%03o",l);	// Print l as an octal value.
            printf(": %d x %d bits = %d bits\n", ascii[l], ndepth, counter);    // Print character, depth, counter.
        }
    }



    // Step 5.
    pack(argv [1], argv [2], leaves);		// pack (input, output, nodes array);
    return 0;
}





