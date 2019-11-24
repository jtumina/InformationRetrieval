/*****************************************
* Jack Umina
* Created Nov, 2019
* Represents a hashtable data structure.
*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/**
 * Creates a new hashtable.
 * @param  num_buckets Number of buckets this hashtable should have
 * @return pointer to this hashtable
 */
struct hashtable* ht_create (int num_buckets) {
    // Initialize space for ht
	struct hashtable* ht = (struct hashtable*) malloc (sizeof (struct hashtable));

    ht->map = (struct wordNode**) malloc (num_buckets * sizeof (struct wordNode*));
	ht->num_buckets = num_buckets;
	ht->num_elements = 0;
    ht->docs;
    ht->num_docs;

    // Initialize space for a wordNode pointer in each bucket
	for (int i = 0; i < num_buckets; i++) {
		ht->map[i] = (struct wordNode*) malloc (sizeof (struct wordNode));

		// Initialize variables in wordNode to NULL/0
		ht->map[i]->word = NULL;
		ht->map[i]->df = 0;
        ht->map[i]->docHead = NULL;
		ht->map[i]->next = NULL;
	}

    return ht;
}

/**
 * Initialize a new docNode with the given parameters.
 * @param docPtr  pointer to the docNode to edit
 * @param doc_id  char* to doc_id word belongs to
 */
void init_docNode (struct docNode* docPtr, char* doc_id) {
    wordPtr->docHead->doc_id = doc_id;
    wordPtr->docHead->tf = 1;
    wordPtr->docHead->next = NULL;
}

/**
 * Initialize a new word node with the given parameters.
 * @param wordPtr pointer to the wordNode to edit
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void init_wordNode (struct wordNode* wordPtr, char* word, char* doc_id) {
    // Initialize fields of wordNode
    wordPtr->word = word;
    wordPtr->df = 1;
    wordPtr->docHead = (struct docNode*) malloc (sizeof (struct docNode));
    wordPtr->next = NULL;

    // Initialize fields of docNode
    init_docNode (wordPtr->docHead, doc_id);
}

/**
 * (1) Inserts this word and doc_id pair into hashtable along with the
 *     corresponding docNode for which this word occurs in.
 * (2) If the word and doc_id pair already exists, update its tf.
 * (3) If the word exists but in a different document,
 *     add new doc_id and update its df.
 * @param ht      pointer to the hashmap
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void ht_insert (struct hashtable* ht, char* word, char* doc_id) {
    // Get this bucket this word belongs in
	int hashCode = hash_code (ht, word);

    // Set pointer to point to correct bucket
	struct wordNode* wordPtr = ht->map[hashCode];

    // If the first word in the bucket is NULL, then call init_wordNode() to fill it
	if (wordPtr->word == NULL) {
        init_wordNode (wordPtr, word, doc_id);
        // Increment num_elements
    	ht->num_elements++;
        return;
	}

	// Temporary pointer to track node before the one we are looking at,
	// this way we can set the wordPtr->next to the correct node
	struct wordNode* lastPtr = NULL;

    // While the word is not NULL continue on through list
	while (wordPtr != NULL) {
		// If we've found the word already in hashmap, we only need to update its fields
		if (strcmp (wordPtr->word, word) == 0) {
            // Continue through docNodes until we find the right doc
            struct docNode* docPtr = wordPtr->docHead;
            struct docNode* lastDocPtr = NULL;
            while (docPtr != NULL) {
                // If the doc already exists, increment its tf
                if (strcmp (docPtr->doc_id, doc_id) == 0) {
                    docPtr->tf += 1;
                    return;
                }
                lastDocPtr = docPtr;
                docPtr = docPtr->next;
            }
            // Else, this word is in a new doc
            docPtr = (struct docNode*) malloc (sizeof (struct docNode));
            init_docNode (docPtr, doc_id);

            // Stitch the last docNode to this one
            lastDocPtr->next = docPtr;

            return;
		}
		// If this is not the word we are trying to insert, continue to next node
		lastPtr = wordPtr;
		wordPtr = wordPtr->next;
	}

    // If wordPtr==NULL, word is not in hashmap, call init_wordNode
    wordPtr = (struct wordNode*) malloc (sizeof (struct wordNode));
    init_wordNode (wordPtr, word, doc_id);

	// Set node before this one's next to this node
	lastPtr->next = wordPtr;

	// Increment num_elements
	ht->num_elements++;
}

/**
 * Removes the key value pair associated with the given key
 * @param hm			pointer to hashmap
 * @param word			to remove
 * @param document_id   this word is in
 */
void hm_remove(struct hashmap* hm, char* word, char* document_id) {
	// Get the bucket this word would be in
	int hashCode = hash (hm, word, document_id);

    // Set pointer to point to correct bucket
	struct llnode* nodePtr = hm->map[hashCode];
	// Pointer to track node before nodePtr (so we can stich list together after removal)
	struct llnode* temp = NULL;

    // If the word is NULL in this bucket, the word does not exist in the list
    if (nodePtr->word == NULL) {
        return;
    }

    // While the word is not NULL and either does not equal the word we are trying to
    // insert or does not equal the document_id, move to next node in list
	while (nodePtr != NULL) {

		// If we've found the word already in hashmap, remove it
		if (strcmp (nodePtr->word, word) == 0 && strcmp (nodePtr->document_id, document_id) == 0) {

			// If this isn't the head of list, stich the adjacent nodes together
			if (temp != NULL) {
				temp->next = nodePtr->next;
			}
			// Now free the node
			free (nodePtr);
			// Decrement num_elements
			hm->num_elements--;

			return;
		}
		temp = nodePtr;
		nodePtr = nodePtr->next;
	}
}

/**
 * Deallocate the hashmap and all of its elements
 * @param hm   pointer to the hashmap
 */
void hm_destroy(struct hashmap* hm){
	// Loop through each bucket
	for (int i = 0; i < hm->num_buckets; i++) {

		// Points to head of the list
		struct llnode* head = hm->map[i];
		// Temporary node pointer
		struct llnode* temp = NULL;

		// Loop through each node in the list by setting head to the next
		// node and then freeing the current one
		while (head != NULL) {
			temp = head;
			head = head->next;
			free (temp);
		}
	}
	free (hm);
}

/**
 * Hashing function to determine which bucket this word belongs in.
 * @param ht      pointer to the hashmap
 * @param word    char* to word we need to add
 * @return the hash code of this word and doc_id pair
 */
int hash_code (struct hashtable* ht, char* word) {
	// Track sum of ASCII values of word
	int sum = 0;
	int i = 0;

	// Loop through, adding ASCII values of each char to sum
	while (*(word+i) != '\0') {
		sum += (int) *(word+i);
		i++;
	}

	// Return the sum % num_buckets to keep range consitent
	return sum % ht->num_buckets;
}
