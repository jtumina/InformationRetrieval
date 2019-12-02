/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Represents a hashtable data structure.
***************************************************************************************/

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

    // Check for allocation errors
    if (ht == NULL) {
        printf("Error: unable to allocate memory for hashtable\n");
        return NULL;
    }

    // Initialize fields of hashtable
    ht->map = (struct wordNode**) malloc (num_buckets * sizeof (struct wordNode*));
	ht->num_buckets = num_buckets;
	ht->num_elements = 0;
    ht->docIDs = NULL;
    ht->num_docs = 0;

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
    docPtr->doc_id = doc_id;
    docPtr->tf = 1;
    docPtr->next = NULL;
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
 * @param ht      pointer to the hashtable
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
		// If we've found the word already in hashtable, we only need to update its fields
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

    // If wordPtr==NULL, word is not in hashtable, call init_wordNode
    wordPtr = (struct wordNode*) malloc (sizeof (struct wordNode));
    init_wordNode (wordPtr, word, doc_id);

	// Set node before this one's next to this node
	lastPtr->next = wordPtr;

	// Increment num_elements
	ht->num_elements++;
}

/**
 * Deallocate the given list of docNodes.
 * @param docPtr pointer to head of the list
 */
void destroy_docList (struct docNode* docPtr) {
    // Temporary ptr
    struct docNode* temp = NULL;

    // Loop through the docNodes
    while (docPtr != NULL) {
        // Free the char* to doc_id
        free (docPtr->doc_id);

        // Free current node while maintaining access to the rest of list
        temp = docPtr;
        docPtr = docPtr->next;
        free (temp);
    }
}

/**
 * Deallocate the given list of wordNodes.
 * @param wordPtr pointer to head of the list
 */
void destroy_wordList (struct wordNode* wordPtr) {
    // Temporary ptr
    struct wordNode* temp = NULL;

    // Loop through the wordNodes
    while (wordPtr != NULL) {
        // Free list of docs at each word
        destroy_docList (wordPtr->docHead);

        // Free the char* to word
        free (wordPtr->word);

        // Free current node while maintaining access to the rest of list
        temp = wordPtr;
        wordPtr = wordPtr->next;
        free (temp);
    }
}

/**
 * Deallocate this hashtable.
 * @param ht pointer to hashtable
 */
void ht_destroy (struct hashtable* ht) {
	// Loop through each bucket, destroying the list of words in each one
	for (int i = 0; i < ht->num_buckets; i++) {
        destroy_wordList (ht->map[i]);
	}

    // Free list of docs
    for (int i = 0; i < ht->num_docs; i++) {
        free (ht->docIDs[i]);
    }
    free (ht->docIDs);

    // Finally, free hashtable struct
	free (ht);
}

/**
 * Hashing function to determine which bucket this word belongs in.
 * @param ht      pointer to the hashtable
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

/**
 * Searches the given hashtable for this word..
 * @param  ht      pointer to the hashtable to search in
 * @param  word    char* to the word to search for
 * @return pointer to the wordNode this word belongs
 */
struct wordNode* get_word (struct hashtable* ht, char* word) {
    // Get the bucket this word would belong to
    int hashCode = hash_code (ht, word);

    // Set pointer to head of word list at this bucket
    struct wordNode* wordPtr = ht->map[hashCode];

    // If the word at this bucket is NULL, word does not exist in the list
    if (wordPtr->word == NULL) {
        return NULL;
    }

    // Loop through word list searching for word
    while (wordPtr != NULL && strcmp (wordPtr->word, word) != 0) {
        wordPtr = wordPtr->next;
    }

    // If we reached NULL, word does not exist in this hashtable
    if (wordPtr == NULL) {
        return NULL;
    }

    // Else, we have found the word
    return wordPtr;
}
